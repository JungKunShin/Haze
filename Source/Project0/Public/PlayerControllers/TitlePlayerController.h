// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerControllers/BasePlayerController.h"
#include "TitlePlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ATitlePlayerController : public ABasePlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay()override;

	virtual void InitSound()override;

	void DelayTitleSound();

private:
	FTimerHandle SoundTH;
};
