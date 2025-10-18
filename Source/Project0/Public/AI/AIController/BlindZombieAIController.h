// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AI/AIController/EnemyAIController.h"
#include "BlindZombieAIController.generated.h"

/**
 * 
 */

class UBehaviorTree;
class UBlackboardData;
class UAIPerceptionComponent;

UCLASS()
class PROJECT0_API ABlindZombieAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
public:
	ABlindZombieAIController();

public:
	virtual UBehaviorTree* GetBehaviorTreeAsset() const { return GrantedBehaviorTree; }
	virtual UBlackboardData* GetBlackboardAsset() const { return GrantedBlackboard; }

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBehaviorTree* GrantedBehaviorTree = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UBlackboardData* GrantedBlackboard = nullptr;

private:

};
