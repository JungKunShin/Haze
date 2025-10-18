// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TutorialEnd.h"
#include "Components/BoxComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"

ATutorialEnd::ATutorialEnd()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp"));
	BoxComp->SetupAttachment(RootComponent);
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECR_Overlap);
}

void ATutorialEnd::BeginPlay()
{
	BoxComp->OnComponentBeginOverlap.AddDynamic(this, &ATutorialEnd::OnTutorialOverlapStart);
}

void ATutorialEnd::OnTutorialOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);
	if (!PlayerCh)return;
	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PlayerCh->GetController());
	if (!WPC)return;
	AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
	if (!WH)return;

	WH->SetLoadingVisible(true);

	WPC->FlushPressedKeys();
	PlayerCh->GetCharacterMovement()->StopMovementImmediately();

	PlayerCh->SetActorLocation(TeleportPos->GetActorLocation());
}
