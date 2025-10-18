// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/StageActor.h"
#include "ActorComponents/QuestSystemComponent.h" 
#include "PushStageActor.generated.h"


/**
 * 
 */
UCLASS()
class PROJECT0_API APushStageActor : public AStageActor
{
	GENERATED_BODY()
	
public:
      APushStageActor();

protected:
    UPROPERTY(VisibleAnywhere) class UBoxComponent* Trigger;

    UPROPERTY(EditAnywhere, Category = "Stage")
    TArray<EQuestState> AllowedStates{ EQuestState::DaughterBook, EQuestState::MomTalking };

    UPROPERTY(EditAnywhere, Category = "Stage")
    float CooldownSeconds = 0.5f;   
    double NextAllowedTime = 0.0;

    UFUNCTION()
    void OnTriggerBegin(UPrimitiveComponent* OverlappedComp, AActor* Other,
        UPrimitiveComponent* OtherComp, int32 BodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);
	
};
