// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "GrandFatherBTDecorator.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UGrandFatherBTDecorator : public UBTDecorator
{
	GENERATED_BODY()

public:
	UGrandFatherBTDecorator();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyName")
	FBlackboardKeySelector KeyTarget;
};
