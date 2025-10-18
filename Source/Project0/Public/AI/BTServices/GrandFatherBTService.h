// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "GrandFatherBTService.generated.h"

class AGrandFatherCharacter;
class APlayerCharacter;

/**
 * 
 */
UCLASS()
class PROJECT0_API UGrandFatherBTService : public UBTService
{
	GENERATED_BODY()

private:
	const float RESET_TARGET_TIME = 5.0f;
	const float DISTANCE_ERROR_RANGE = 300.0f;
	const float TRACE_DISTANCE = 1000.0f;
	const float FAN_ANGLE_DEGREES = 90.0f;
	const float NUM_TRACES = 45.0f;
	const float RUN_SPEED = 600.0f;
	const float WALK_SPEED = 160.f;

public:

	UGrandFatherBTService();

protected:

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UFUNCTION()
	void DetectTargetEvent(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds);

	UFUNCTION()
	void UpdatePatrolLoc(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds);

	UFUNCTION()
	void QuestNum(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds);

	UFUNCTION()
	APlayerCharacter* FindTargetByTrace(AGrandFatherCharacter* SelfActor);

	UFUNCTION()
	void ResetTarget(AGrandFatherCharacter* SelfActor);

	UFUNCTION()
	void PatternOnePatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos);

	UFUNCTION()
	void PatternTwoPatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos);

	UFUNCTION()
	void PatternThreePatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyName")
	FBlackboardKeySelector KeyTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyName")
	FBlackboardKeySelector KeyPatrolPos;

private:

	FTimerHandle ResetTargetTH;

	APlayerCharacter* NewTarget = nullptr;

	bool IsChasing = false;
};
