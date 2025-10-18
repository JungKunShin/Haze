// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/GFAttackBTTask.h"
#include "AI/AIController/GrandFatherAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

UGFAttackBTTask::UGFAttackBTTask()
{
	bNotifyTick = true; // �� �ʿ�! ƽ Ÿ��ũ�� ����ϱ����ؼ�
}

EBTNodeResult::Type UGFAttackBTTask::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AActor* Owner = OwnerComp.GetOwner();
	if (!Owner) return EBTNodeResult::Failed;


	AGrandFatherAIController* GFAIController = Cast<AGrandFatherAIController>(Owner);
	if (!GFAIController) return EBTNodeResult::Failed;

	BBComp = GFAIController->GetBlackboardComponent();

	UObject* SelfActorObj = GFAIController->GetBlackboardComponent()->GetValueAsObject(KeySelfActor.SelectedKeyName);
	if (!SelfActorObj) return EBTNodeResult::Failed;

	SelfActor = Cast<AGrandFatherCharacter>(SelfActorObj);
	if (!SelfActor) return EBTNodeResult::Failed;

	UObject* TargetObj = GFAIController->GetBlackboardComponent()->GetValueAsObject(KeyTarget.SelectedKeyName);
	if (!TargetObj) return EBTNodeResult::Failed;

	Target = Cast<APlayerCharacter>(TargetObj);
	if (!Target) return EBTNodeResult::Failed;

	SelfActor->CharacterPlayMontage(SelfActor->AttackMontage);

	SelfActor->HitPlayer(Target);

	return EBTNodeResult::InProgress;
}

void UGFAttackBTTask::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	if (!SelfActor || !SelfActor->AttackMontage)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}


	UAnimInstance* AnimInstance = SelfActor->GetMesh()->GetAnimInstance();

	if (!AnimInstance) return;

	// �ٸ� ��Ÿ�ְ� ��� ���̶�� ���
	if (AnimInstance->GetCurrentActiveMontage() != nullptr) return;

	SelfActor->KillConversation(Target);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
