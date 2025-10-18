// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"

#include "StageActor.generated.h"

/**
 * 
 */


class IIStageDependent;

UCLASS()
class PROJECT0_API AStageActor : public ABaseActor
{
	GENERATED_BODY()
	
public:
	AStageActor();

	UFUNCTION(BlueprintCallable)
	void MakeThisCurrentStage();

	UFUNCTION(BlueprintCallable)
	void ResetStage();

	UFUNCTION(BlueprintCallable) 
	void RegisterDependent(AActor* Dep);

	UFUNCTION(BlueprintCallable) 
	void UnregisterDependent(AActor* Dep);

	UFUNCTION(BlueprintPure)
	FORCEINLINE FVector GetPlayerResetLocation() const {
		return GetTransform().TransformPosition(PlayerResetLocation);
	};

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	TArray<AActor*> StageDependents;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, meta = (MakeEditWidget = "true"))
	FVector PlayerResetLocation;
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly)
	bool bUseSideView = true;

};
