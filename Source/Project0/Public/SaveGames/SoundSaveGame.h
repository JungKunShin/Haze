// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SoundSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API USoundSaveGame : public USaveGame
{
	GENERATED_BODY()
	
public:

    USoundSaveGame();

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float MasterVolume;

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float BGMVolume;

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float EffectVolume;

    UPROPERTY(VisibleAnywhere, Category = "Sound")
    float OriginalUIVolume;
};
