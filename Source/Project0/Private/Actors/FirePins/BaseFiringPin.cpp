// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FirePins/BaseFiringPin.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"

ABaseFiringPin::ABaseFiringPin()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	InnerBoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("InnerBoxComp"));
	InnerBoxComp->SetupAttachment(BoxComp);

	BoxComp->SetBoxExtent(FVector(700.f, 500.f, 250.f)); // ũ�� ����
	InnerBoxComp->SetBoxExtent(FVector(700.f, 100.f, 250.f)); // ũ�� ����
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
	InnerBoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	InnerBoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ABaseFiringPin::BeginPlay()
{
	Super::BeginPlay();

	InnerBoxComp->OnComponentBeginOverlap.AddDynamic(this, &ABaseFiringPin::OnOverlapStart);
	BoxComp->OnComponentEndOverlap.AddDynamic(this, &ABaseFiringPin::OnOverlapFinish);
}

void ABaseFiringPin::OnOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);

	if (!PlayerCh)return;

	if (IsIn)return;

	IsIn = true;

	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PlayerCh->GetController());

	if (WPC)
	{
		AWorldHUD* WH = Cast<AWorldHUD>(WPC->GetHUD());
		if (WH)
		{
			WH->SetInstructionKeep();	// ���� ���� Ʃ�丮�� ���
		}
	}
}

void ABaseFiringPin::OnOverlapFinish(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);

	if (!PlayerCh)return;

	if (IsOut)return;

	if (!IsIn)return;

	IsOut = true;

	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PlayerCh->GetController());

	if (WPC)
	{
		AWorldHUD* WH = Cast<AWorldHUD>(WPC->GetHUD());
		if (WH)
		{
			WH->SetInstructionEnd();	// ���� ����ǰ� �ִ� Ʃ�丮�� ����
		}
	}
}
