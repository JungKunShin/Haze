// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController/BaseAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void ABaseAIController::OnPossess(APawn* InPawn)
{
    Super::OnPossess(InPawn);

    UBehaviorTree* BTAsset = GetBehaviorTreeAsset();
    UBlackboardData* BBAsset = GetBlackboardAsset();

    if (BTAsset && BBAsset)
    {
        bool bSuccess = UseBlackboard(BBAsset, BBComp);
        if (bSuccess)
        {
            RunBehaviorTree(BTAsset);
        }
    }
}
