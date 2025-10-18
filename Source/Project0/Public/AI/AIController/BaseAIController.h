// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BaseAIController.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ABaseAIController : public AAIController
{
	GENERATED_BODY()
	
protected:

    virtual void OnPossess(APawn* InPawn) override;


    // �ڽ��� �������̵��ؼ� �ڽ��� BT/BB ��ȯ
    virtual UBehaviorTree* GetBehaviorTreeAsset() const { return nullptr; }
    virtual UBlackboardData* GetBlackboardAsset() const { return nullptr; }

    UBlackboardComponent* BBComp;
};
