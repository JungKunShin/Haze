// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemyAIController.h"
#include "GrandFatherAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AGrandFatherAIController : public AEnemyAIController
{
	GENERATED_BODY()
	
protected:

    virtual UBehaviorTree* GetBehaviorTreeAsset() const override { return GFBT; }
    virtual UBlackboardData* GetBlackboardAsset() const override { return GFBB; }

    virtual void OnPossess(APawn* InPawn) override;

    UPROPERTY(EditAnywhere, Category = "AI")
    UBehaviorTree* GFBT;

    UPROPERTY(EditAnywhere, Category = "AI")
    UBlackboardData* GFBB;

};
