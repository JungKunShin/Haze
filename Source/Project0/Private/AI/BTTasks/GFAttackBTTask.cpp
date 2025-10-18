// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTasks/GFAttackBTTask.h"
#include "AI/AIController/GrandFatherAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

UGFAttackBTTask::UGFAttackBTTask()
{
	bNotifyTick = true; // 꼭 필요! 틱 타스크를 사용하기위해서
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

	// 다른 몽타주가 재생 중이라면 대기
	if (AnimInstance->GetCurrentActiveMontage() != nullptr) return;

	SelfActor->KillConversation(Target);
	FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}
