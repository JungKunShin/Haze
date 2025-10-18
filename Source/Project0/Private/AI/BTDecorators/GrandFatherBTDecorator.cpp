// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorators/GrandFatherBTDecorator.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

UGrandFatherBTDecorator::UGrandFatherBTDecorator()
{
}

bool UGrandFatherBTDecorator::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	// ������κ��� Ÿ�� ���� ��������
	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return false;

	AActor* TargetActor = Cast<AActor>(BBComp->GetValueAsObject(KeyTarget.SelectedKeyName));
	if (!TargetActor) return false;

	APlayerCharacter* PlayerCh = Cast<APlayerCharacter>(TargetActor);

	APawn* AIPawn = OwnerComp.GetAIOwner() ? OwnerComp.GetAIOwner()->GetPawn() : nullptr;
	if (!AIPawn) return false;

	FVector AILocation = AIPawn->GetActorLocation();
	FVector TargetLocation = TargetActor->GetActorLocation();

	float Distance = FVector::Dist(AILocation, TargetLocation);

	if (Distance < 250.0f&&!PlayerCh->IsDeskIn)//���� �ʿ� �������� const������ �ٲٱ�
	{
		return true;
	}

	return false;
}
