// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/AIController/GrandFatherAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"

void AGrandFatherAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	AGrandFatherCharacter* PossessCharacter = Cast< AGrandFatherCharacter>(GetCharacter());

	BBComp->SetValueAsVector(FName("PatrolPos"), PossessCharacter->PatrolPos[EPosition::MomsRoom]->GetActorLocation());
}
