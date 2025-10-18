// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/GFTeleport.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/NonPlayerCharacters/EnemyCharacter.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

AGFTeleport::AGFTeleport()
{
    BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
    BoxComp->SetupAttachment(RootComponent);
	InnerBoxComp= CreateDefaultSubobject<UBoxComponent>(TEXT("InnerBoxComp"));
	InnerBoxComp->SetupAttachment(BoxComp);

    BoxComp->SetBoxExtent(FVector(700.f, 500.f, 250.f)); // 크기 설정
	InnerBoxComp->SetBoxExtent(FVector(700.f, 100.f, 250.f)); // 크기 설정
    BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	InnerBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerBoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void AGFTeleport::BeginPlay()
{
    Super::BeginPlay();

	InnerBoxComp->OnComponentBeginOverlap.AddDynamic(this, &AGFTeleport::OnOverlapBegin);
    BoxComp->OnComponentEndOverlap.AddDynamic(this, &AGFTeleport::OnOverlapEnd);
}

void AGFTeleport::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!Cast<APlayerCharacter>(OtherActor))return;

	if (IsIn)return;

	PlayerCh = Cast<APlayerCharacter>(OtherActor);

	IsIn = true;

	OriginalLength = PlayerCh->CameraBoom->TargetArmLength;
	PlayerCh->CameraBoom->TargetArmLength = CLOSE_LENGTH;

	OriginalLoc = PlayerCh->ThirdPersonCamera->GetRelativeLocation();
	FVector NewLoc = OriginalLoc;
	NewLoc.Z -= DOWN_LENGTH;

	PlayerCh->ThirdPersonCamera->SetRelativeLocation(NewLoc);

}

void AGFTeleport::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!Cast<APlayerCharacter>(OtherActor))return;

	if (IsOut)return;

	if (!IsIn)return;

	TArray<AActor*> EnemyCh;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Chasing"), EnemyCh);

	FVector CamForward = PlayerCh->ThirdPersonCamera->GetForwardVector();
	CamForward.Z = 0.f; // 수평만 고려

	FVector SpawnLocation = GetActorLocation() + CamForward.GetSafeNormal() * 300.f+FVector(0.f,0.f,-50.f);
	
	FVector ToPlayer = (PlayerCh->GetActorLocation() - SpawnLocation).GetSafeNormal();
	FRotator LookAtRot = ToPlayer.Rotation();

	for (AActor* MyActor : EnemyCh)
	{
		AGrandFatherCharacter* GrandFather = Cast<AGrandFatherCharacter>(MyActor);
		if (GrandFather)
		{
			IsOut = true;
			GrandFather->SetActorLocation(SpawnLocation);
			GrandFather->SetActorRotation(LookAtRot);
			InteractSoundInit();
			break;
		}
	}

	PlayerCh->ThirdPersonCamera->SetRelativeLocation(OriginalLoc);
	GetWorldTimerManager().SetTimer(OutTH, this, &AGFTeleport::DelayOverlapEnd, 0.01f, true);
}

void AGFTeleport::DelayOverlapEnd()
{
	float NewLength = PlayerCh->CameraBoom->TargetArmLength + ZOOM_SPEED * 0.01f;
	PlayerCh->CameraBoom->TargetArmLength = FMath::Clamp(NewLength, 0.0f, OriginalLength);

	if (PlayerCh->CameraBoom->TargetArmLength >= OriginalLength)
	{
		GetWorldTimerManager().ClearTimer(OutTH);
	}
}
