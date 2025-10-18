// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerInteractableComponent.h"
#include "PlayerRotationComponent.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UPlayerRotationComponent : public UPlayerInteractableComponent
{
	GENERATED_BODY()

public:
	UPlayerRotationComponent();
	
	virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;

	void LeftRotationPicture();

	void RightRotationPicture();

	UFUNCTION(BlueprintCallable)
	void ResetRotComp();

private:
	bool IsZoom = false;
};
