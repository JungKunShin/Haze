// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/StageActor.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Interfaces/IStageDependent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GameInstances/StageGameInstance.h"

AStageActor::AStageActor()
{

}

void AStageActor::MakeThisCurrentStage()
{
	UStageGameInstance* StageGM = GetGameInstance<UStageGameInstance>();
	if (StageGM) {
		StageGM->ChangeCurrentStage(this);
	}
	else {
		UKismetSystemLibrary::PrintString(this, TEXT("StageChangeFail... Current GM Is Not UStageGameInstance"));
	}
}

void AStageActor::ResetStage()
{
	APlayerCharacter* PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawn<APlayerCharacter>();
	if (PlayerCharacter) {
		if (bUseSideView) {
			GetWorld()->GetTimerManager().SetTimerForNextTick(PlayerCharacter, &APlayerCharacter::SwitchToSideView);
		}
		else {
			GetWorld()->GetTimerManager().SetTimerForNextTick(PlayerCharacter, &APlayerCharacter::SwitchToThirdPersonView);
		}
	}

	for (AActor* StageDependent : StageDependents) {
		if (StageDependent) IIStageDependent::Execute_OnReset(StageDependent);
	}
}

void AStageActor::RegisterDependent(AActor* Dep)
{
	if (!IsValid(Dep)) return;

	StageDependents.RemoveAll([](AActor* A) { return !IsValid(A); });
	StageDependents.AddUnique(Dep);
}

void AStageActor::UnregisterDependent(AActor* Dep)
{
	StageDependents.Remove(Dep);
}
