// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "StageSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UStageSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	FName SavedStageName;
	UPROPERTY(SaveGame, VisibleAnywhere, BlueprintReadWrite)
	EQuestState QuestState;

};
