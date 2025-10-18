// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "InteractTestActor.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AInteractTestActor : public ABaseActor
{
	GENERATED_BODY()

public:

	AInteractTestActor();

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh = nullptr;
	

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerPickupComponent* PickupComp = nullptr;
};
