// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/TitlePlayerController.h"
#include "Manager/SoundManager.h"

void ATitlePlayerController::BeginPlay()
{
	GetWorld()->GetTimerManager().ClearTimer(SoundTH);
	GetWorld()->GetTimerManager().SetTimer(SoundTH, this, &ATitlePlayerController::DelayTitleSound, 0.2f, false);
}

void ATitlePlayerController::InitSound()
{
	Super::InitSound();

	if (SoundManager)
	{
		SoundManager->PlayBGM(SoundManager->BGM_Title); // 원하는 BGM 지정
	}
}

void ATitlePlayerController::DelayTitleSound()
{
	Super::BeginPlay();
	GetWorld()->GetTimerManager().ClearTimer(SoundTH);
}
