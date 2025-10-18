// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GFAttackBTTask.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UGFAttackBTTask : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UGFAttackBTTask();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;

	// TickTask는 InProgress 리턴 시 매 Tick마다 호출됨
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	class AGrandFatherCharacter* SelfActor;
	class APlayerCharacter* Target;
	UBlackboardComponent* BBComp;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyName")
	FBlackboardKeySelector KeySelfActor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "KeyName")
	FBlackboardKeySelector KeyTarget;
};
