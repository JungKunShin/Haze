// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FirePins/DirectionKeysFiringPin.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"

void ADirectionKeysFiringPin::OnOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* PlayerCh = Cast< APlayerCharacter>(OtherActor);

	if (!PlayerCh)return;

	if (IsIn)return;

	IsIn = true;

	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PlayerCh->GetController());

	if (WPC)
	{
		AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
		if (WH)
		{
			WH->SetInstructionVisible(true);
			WH->SetInstructionKeep();
		}
	}
}
