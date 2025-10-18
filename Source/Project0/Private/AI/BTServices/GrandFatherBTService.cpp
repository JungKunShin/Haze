// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTServices/GrandFatherBTService.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UGrandFatherBTService::UGrandFatherBTService()
{
	// 서비스가 트리에 포함될 때 자동으로 실행되도록 설정
	bNotifyBecomeRelevant = true;
	// Tick 함수가 주기적으로 실행되도록 설정
	bNotifyTick = true;
	// Tick 주기 설정 (0.1초마다 실행)
	Interval = 0.5f;
}

void UGrandFatherBTService::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);


	AAIController* AIController = OwnerComp.GetAIOwner();
	if (!AIController) return;

	APawn* ControlledPawn = AIController->GetPawn();
	if (!ControlledPawn) return;

	UBlackboardComponent* BBComp = OwnerComp.GetBlackboardComponent();
	if (!BBComp) return;


	DetectTargetEvent(AIController, ControlledPawn, BBComp, DeltaSeconds);
	QuestNum(AIController, ControlledPawn, BBComp, DeltaSeconds);
	UpdatePatrolLoc(AIController, ControlledPawn, BBComp, DeltaSeconds);
}

void UGrandFatherBTService::DetectTargetEvent(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds)
{
	if (!AIController || !ControlledPawn || !BBComp)
		return;

	AAIController* GFAIController = Cast<AAIController>(AIController);
	AGrandFatherCharacter* SelfActor = Cast<AGrandFatherCharacter>(ControlledPawn);
	if (!GFAIController || !SelfActor) return;

	APlayerCharacter* FoundTarget = FindTargetByTrace(SelfActor); // 이것도 수정하기
	
	BBComp->SetValueAsObject(KeyTarget.SelectedKeyName, FoundTarget);

}

void UGrandFatherBTService::UpdatePatrolLoc(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds)
{
	if (!AIController || !ControlledPawn || !BBComp)
		return;

	if (BBComp->GetValueAsObject(KeyTarget.SelectedKeyName)) return;

	AAIController* GFAIController = Cast<AAIController>(AIController);
	AGrandFatherCharacter* SelfActor = Cast<AGrandFatherCharacter>(ControlledPawn);
	if (!GFAIController || !SelfActor) return;

	EPosition CurPatrolPos;

	for (const TPair<EPosition, AActor*>& Pair : SelfActor->PatrolPos)
	{
		if (Pair.Value->GetActorLocation() == BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName))
		{
			CurPatrolPos = Pair.Key;
			break;
		}
	}

	

	switch (UQuestSystemComponent::CurQuestState)
	{
	case EQuestState::None:
		break;
	case EQuestState::FirstConversation:
		PatternOnePatrol(AIController, ControlledPawn, BBComp, DeltaSeconds,CurPatrolPos);
		break;
	case EQuestState::DaughterBook:
	case EQuestState::MomTalking:
	case EQuestState::MomBarn:
		PatternTwoPatrol(AIController, ControlledPawn, BBComp, DeltaSeconds, CurPatrolPos);
		break;
	case EQuestState::MomMix:
	case EQuestState::MazeBefore:
		PatternThreePatrol(AIController, ControlledPawn, BBComp, DeltaSeconds, CurPatrolPos);
		break;
	case EQuestState::MazeMiddle:
	case EQuestState::MazeAfter:
	case EQuestState::SequenceOne:
	case EQuestState::SequenceTwo:
	case EQuestState::End:
	case EQuestState::GrandFatherDeath:
	case EQuestState::ZombieDeath:
	default:
		break;
	}

	//퀘스트 컴포넌트 만들고 나서 현재 게임 진행상황에 따라 패트롤 패턴 바꾸기
}

void UGrandFatherBTService::QuestNum(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds)
{
	if (!AIController || !ControlledPawn || !BBComp)
		return; // 퀘스트 컴포넌트 만들고 나서 최신화하기
}

APlayerCharacter* UGrandFatherBTService::FindTargetByTrace(AGrandFatherCharacter* SelfActor)
{
	FVector Start = SelfActor->GetActorLocation();

	float HalfFan = FAN_ANGLE_DEGREES / 2.0f;
	FRotator ActorRotation = SelfActor->GetActorRotation();

	for (int32 i = 0; i < NUM_TRACES; ++i)
	{
		float AngleStep = FAN_ANGLE_DEGREES / (NUM_TRACES - 1);
		float YawOffset = -HalfFan + AngleStep * i;

		FRotator TraceRot = ActorRotation + FRotator(0, YawOffset, 0);
		FVector Direction = TraceRot.Vector();
		FVector End = Start + Direction * TRACE_DISTANCE;

		FHitResult HitResult;

		FCollisionObjectQueryParams ObjectQueryParams;
		ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn); // 예: Pawn
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
		ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);// 움직이는 물체

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(SelfActor);

		bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, QueryParams);

		// 디버그용 라인
		FColor LineColor = bHit ? FColor::Red : FColor::Green;
		DrawDebugLine(GetWorld(), Start, End, LineColor, false, 1.0f, 0, 1.5f);

		if (bHit)
		{
			AActor* HitActor = HitResult.GetActor();
			if (HitActor && Cast<APlayerCharacter>(HitActor))
			{
				NewTarget = Cast<APlayerCharacter>(HitActor);

				if (!IsChasing)
				{
					IsChasing = true;
					SelfActor->ChasingSoundStart();
				}

				if (NewTarget->IsDeskIn)
				{
					ResetTarget(SelfActor);
					return NewTarget;
				}

				GetWorld()->GetTimerManager().ClearTimer(ResetTargetTH);
				//GetWorld()->GetTimerManager().SetTimer(ResetTargetTH, this, &UGrandFatherBTService::ResetTarget, RESET_TARGET_TIME, false);

				FTimerDelegate TimerDel;
				TimerDel.BindUFunction(this, FName("ResetTarget"), SelfActor);
				GetWorld()->GetTimerManager().SetTimer(ResetTargetTH, TimerDel, RESET_TARGET_TIME, false);
				SelfActor->GetCharacterMovement()->MaxWalkSpeed = RUN_SPEED;


				break;
			}
		}

	}

	return NewTarget;
}

void UGrandFatherBTService::ResetTarget(AGrandFatherCharacter* SelfActor)
{
	IsChasing = false;
	SelfActor->ChasingSoundEnd();
	SelfActor->GetCharacterMovement()->MaxWalkSpeed = WALK_SPEED;
	NewTarget = nullptr;
	GetWorld()->GetTimerManager().ClearTimer(ResetTargetTH);
}

void UGrandFatherBTService::PatternOnePatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos)
{
	AAIController* GFAIController = Cast<AAIController>(AIController);
	AGrandFatherCharacter* SelfActor = Cast<AGrandFatherCharacter>(ControlledPawn);

	if (SelfActor->PatrolPos[Pos]->GetActorLocation() == BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName) && BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName).Equals(SelfActor->GetActorLocation(), DISTANCE_ERROR_RANGE))
	{
		switch (Pos)
		{
		case EPosition::Kitchen:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::DaughtersRoom]->GetActorLocation());
			break;
		case EPosition::DaughtersRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::MomsRoom]->GetActorLocation());
			break;
		case EPosition::MomsRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		case EPosition::RoofTop:
		case EPosition::BarnFirst:
		case EPosition::BarnSecond:
		default:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		}
	}
}

void UGrandFatherBTService::PatternTwoPatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos)
{
	AAIController* GFAIController = Cast<AAIController>(AIController);
	AGrandFatherCharacter* SelfActor = Cast<AGrandFatherCharacter>(ControlledPawn);

	if (SelfActor->PatrolPos[Pos]->GetActorLocation() == BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName) && BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName).Equals(SelfActor->GetActorLocation(), DISTANCE_ERROR_RANGE))
	{
		switch (Pos)
		{
		case EPosition::Kitchen:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::MomsRoom]->GetActorLocation());
			break;
		case EPosition::DaughtersRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::RoofTop]->GetActorLocation());
			break;
		case EPosition::MomsRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::DaughtersRoom]->GetActorLocation());
			break;
		case EPosition::RoofTop:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		case EPosition::BarnFirst:
		case EPosition::BarnSecond:
		default:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		}
	}
}

void UGrandFatherBTService::PatternThreePatrol(AAIController* AIController, APawn* ControlledPawn, UBlackboardComponent* BBComp, float DeltaSeconds, EPosition Pos)
{
	AAIController* GFAIController = Cast<AAIController>(AIController);
	AGrandFatherCharacter* SelfActor = Cast<AGrandFatherCharacter>(ControlledPawn);

	if (SelfActor->PatrolPos[Pos]->GetActorLocation() == BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName) && BBComp->GetValueAsVector(KeyPatrolPos.SelectedKeyName).Equals(SelfActor->GetActorLocation(), DISTANCE_ERROR_RANGE))
	{
		switch (Pos)
		{
		case EPosition::Kitchen:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::BarnSecond]->GetActorLocation());
			break;
		case EPosition::DaughtersRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::MomsRoom]->GetActorLocation());
			break;
		case EPosition::MomsRoom:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		case EPosition::RoofTop:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::DaughtersRoom]->GetActorLocation());
			break;
		case EPosition::BarnFirst:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::RoofTop]->GetActorLocation());
			break;
		case EPosition::BarnSecond:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::BarnFirst]->GetActorLocation());
			break;
		default:
			BBComp->SetValueAsVector(KeyPatrolPos.SelectedKeyName, SelfActor->PatrolPos[EPosition::Kitchen]->GetActorLocation());
			break;
		}
	}
}
