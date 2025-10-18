// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameInstances/BaseGameInstance.h"
#include "StageGameInstance.generated.h"

/**
 * 
 */

class AStageActor;

UCLASS()
class PROJECT0_API UStageGameInstance : public UBaseGameInstance
{
	GENERATED_BODY()

public:
	UStageGameInstance();

	UFUNCTION(BlueprintPure)
	FORCEINLINE AStageActor* GetCurrentStageActor() const {
		return CurrentStage;
	};

	UFUNCTION(BlueprintPure)
	FORCEINLINE AStageActor* GetPrevStageActor() const {
		return PrevStage;
	};

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bPlayOpeningSequence = false;

	UFUNCTION(BlueprintCallable)
	void ChangeCurrentStage(AStageActor* TargetStage);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ResetStage(bool bShouldPlayerLocationReset = true);

	UFUNCTION(BlueprintCallable)
	bool TrySaveGame();
	UFUNCTION(BlueprintCallable)
	bool TryLoadGame();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool bNeedToLoadGame = false;

protected:
	AStageActor* CurrentStage;
	AStageActor* PrevStage;

	UPROPERTY(EditDefaultsOnly)
	FString StageSaveSlotString = TEXT("StageSaveSlot");

};
