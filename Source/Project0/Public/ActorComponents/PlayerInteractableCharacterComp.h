// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerInteractableComponent.h"
#include "PlayerInteractableCharacterComp.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class PROJECT0_API UPlayerInteractableCharacterComp : public UPlayerInteractableComponent
{
	GENERATED_BODY()

public:

public:
	virtual void BeginFocus_Implementation() override;
	virtual void EndFocus_Implementation() override;
	virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;
	virtual void EndInteraction_Implementation() override;
	
};
