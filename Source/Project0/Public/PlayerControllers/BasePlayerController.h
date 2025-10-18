// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "BasePlayerController.generated.h"

/**
 * 
 */

class ASoundManager;

UCLASS()
class PROJECT0_API ABasePlayerController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

	virtual void InitSound();

	void LoadGameIfNeeded();

public:
	void SaveSoundSettings();

	void LoadSoundSettings();

	UFUNCTION(BlueprintCallable)
	ASoundManager* GetSoundManager();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager")
	TSubclassOf<ASoundManager> SoundManagerClass;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Manager")
	ASoundManager* SoundManager;

};
