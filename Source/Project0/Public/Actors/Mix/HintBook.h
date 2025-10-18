// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "HintBook.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AHintBook : public ABaseActor
{
	GENERATED_BODY()

public:
	AHintBook();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerMixComponent* MixComp = nullptr;
	
};
