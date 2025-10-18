// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerInteractableComponent.h"
#include "PlayerMixComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UPlayerMixComponent : public UPlayerInteractableComponent
{
	GENERATED_BODY()
	
public:
	UPlayerMixComponent();

	virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;

private:
	bool IsOpen = false;
};
