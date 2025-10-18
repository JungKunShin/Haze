// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacters/EnemyCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "AI/AIController/EnemyAIController.h"

AEnemyCharacter::AEnemyCharacter()
{
}

void AEnemyCharacter::HitPlayer(APlayerCharacter* _Player)
{
	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(_Player->GetController());
	WPC->RemoveGameIMC();
	WPC->SetViewTargetWithBlend(WPC->GetCharacter(), 1.0f);
	TargetCharacter = _Player;
}

void AEnemyCharacter::KillConversation(APlayerCharacter* _Player)
{
	CurAI = Cast<AEnemyAIController>(GetController());
	GetController()->UnPossess();

	OriginalZoomLength = TargetCharacter->CameraBoom->TargetArmLength;
	_Player->BeforeDeath(DeathType);

}

void AEnemyCharacter::KillPlayer()
{
	CharacterPlayMontage(AttackMontage, "Eat");
	CameraZoomIn();
}

void AEnemyCharacter::CameraZoomIn()
{
	//TargetCharacter->CameraBoom->SetUsingAbsoluteRotation(true);
	//TargetCharacter->CameraBoom->bUsePawnControlRotation = true;
	//TargetCharacter->ThirdPersonCamera->SetUsingAbsoluteRotation(true);
	//TargetCharacter->ThirdPersonCamera->bUsePawnControlRotation = true; 

	GetWorldTimerManager().SetTimer(ZoomTH, this, &AEnemyCharacter::DelayZoomOut, 0.01f, true);
}

void AEnemyCharacter::DelayZoomOut()
{
	

	float NewLength = TargetCharacter->CameraBoom->TargetArmLength - (ZOOM_SPEED+GeometricalSum) * 0.01f;
	TargetCharacter->CameraBoom->TargetArmLength = FMath::Clamp(NewLength, TARGET_LENGTH, 1000.f);

	GeometricalSum +=1.5f;

	if (TargetCharacter->CameraBoom->TargetArmLength <= TARGET_LENGTH)
	{
		GetWorldTimerManager().ClearTimer(ZoomTH);
	}
}

void AEnemyCharacter::ResetEnemyLocation()
{
}

void AEnemyCharacter::ResetEnemy()
{
	GeometricalSum = 0.f;
	ResetEnemyLocation();
	CurAI->Possess(this);
	TargetCharacter->CameraBoom->TargetArmLength = OriginalZoomLength;
	TargetCharacter->StopAnimMontage();
	this->StopAnimMontage();
	AWorldPlayerController* WPC = Cast< AWorldPlayerController>(TargetCharacter->GetController());
	WPC->AddGameIMC();
}
