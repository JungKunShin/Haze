// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerControllers/BasePlayerController.h"
#include "GameInstances/StageGameInstance.h"
#include "Manager/SoundManager.h"
#include "SaveGames/SoundSaveGame.h"

#include "Kismet/GameplayStatics.h"

void ABasePlayerController::BeginPlay()
{
	Super::BeginPlay();

	InitSound();

	LoadSoundSettings();

	LoadGameIfNeeded();

	
}

void ABasePlayerController::InitSound()
{
	SoundManager = GetWorld()->SpawnActor<ASoundManager>(SoundManagerClass);

	if (SoundManager)
	{
		SoundManager->InitSoundManager();
	}
}

void ABasePlayerController::LoadGameIfNeeded()
{
	UStageGameInstance* StageGameInstance = GetGameInstance<UStageGameInstance>();
	if (StageGameInstance->bNeedToLoadGame) {
		StageGameInstance->TryLoadGame();
	}
}

void ABasePlayerController::SaveSoundSettings()
{
	USoundSaveGame* SaveGameInstance = Cast<USoundSaveGame>(UGameplayStatics::CreateSaveGameObject(USoundSaveGame::StaticClass()));

	SaveGameInstance->MasterVolume = SoundManager->MasterVolume;
	SaveGameInstance->BGMVolume = SoundManager->BGMVolume;
	SaveGameInstance->EffectVolume = SoundManager->EffectVolume;
	SaveGameInstance->OriginalUIVolume = SoundManager->OriginalUIVolume;

	UGameplayStatics::SaveGameToSlot(SaveGameInstance, TEXT("SoundSlot"), 0);
}

void ABasePlayerController::LoadSoundSettings()
{
	if (UGameplayStatics::DoesSaveGameExist(TEXT("SoundSlot"), 0))
	{
		USoundSaveGame* LoadGameInstance = Cast<USoundSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("SoundSlot"), 0));

		SoundManager->MasterVolume = LoadGameInstance->MasterVolume;
		SoundManager->BGMVolume = LoadGameInstance->BGMVolume;
		SoundManager->EffectVolume = LoadGameInstance->EffectVolume;
		SoundManager->OriginalUIVolume = LoadGameInstance->OriginalUIVolume;

		SoundManager->InitSoundManager();
	}
}

ASoundManager* ABasePlayerController::GetSoundManager()
{
	if (SoundManager) {
		return SoundManager;
	}
	else {
		InitSound();
		return SoundManager;
	}
}
