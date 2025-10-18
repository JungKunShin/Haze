// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstances/StageGameInstance.h"
#include "Actors/StageActor.h"
#include "SaveGames/StageSaveGame.h"

#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"

#include "HUD/WorldHUD.h"

UStageGameInstance::UStageGameInstance()
{

}

void UStageGameInstance::ChangeCurrentStage(AStageActor* TargetStage)
{
	PrevStage = CurrentStage;
	CurrentStage = TargetStage;
}

void UStageGameInstance::ResetStage_Implementation(bool bShouldPlayerLocationReset)
{
	if (CurrentStage) {
		CurrentStage->ResetStage();

		if (bShouldPlayerLocationReset) {
			if (GetFirstLocalPlayerController() && GetFirstLocalPlayerController()->GetPawn()) {
				GetFirstLocalPlayerController()->GetPawn()->SetActorLocation(CurrentStage->GetPlayerResetLocation());
			}
		}
	}
	
}

bool UStageGameInstance::TrySaveGame()
{
	UStageSaveGame* StageSaveGame;

	// 이미 저장된 정보가 있으면 불러오기
	if (UGameplayStatics::DoesSaveGameExist(StageSaveSlotString, 0)) {
		StageSaveGame = Cast<UStageSaveGame>(
			UGameplayStatics::LoadGameFromSlot(StageSaveSlotString, 0)
		);
	}
	else { // 없으면 세이브 슬롯 새로 만들기
		StageSaveGame =
			Cast<UStageSaveGame>(UGameplayStatics::CreateSaveGameObject(UStageSaveGame::StaticClass()));
	}

	// false를 반환하는 경우는 세이브 실패
	if (!StageSaveGame) return false;

	if (!CurrentStage) {
		UKismetSystemLibrary::PrintString(this, TEXT("CurrentStage Is nullptr"));
		return false;
	}
	else if (CurrentStage->Tags.IsEmpty()) {
		UKismetSystemLibrary::PrintString(this, TEXT("CurrentStage Has No Tag"));
		return false;
	}

	// 아래부터는 세이브 성공
	StageSaveGame->SavedStageName = CurrentStage->Tags[0];
	StageSaveGame->QuestState = UQuestSystemComponent::CurQuestState;

	UGameplayStatics::SaveGameToSlot(
		StageSaveGame, StageSaveSlotString, 0);

	UKismetSystemLibrary::PrintString(this, TEXT("Save Has Successed"));

	return true;
}

bool UStageGameInstance::TryLoadGame()
{
	// 세이브 슬롯에 제대로 저장되어있으면 로딩하기
	if (UGameplayStatics::DoesSaveGameExist(StageSaveSlotString, 0)) {
		UStageSaveGame* LoadedStage = Cast<UStageSaveGame>(
			UGameplayStatics::LoadGameFromSlot(StageSaveSlotString, 0)
		);
		if (LoadedStage) { // 세이브 게임 로딩 성공
			TArray<AActor*> LoadedTargetStageActors;
			// 로드된 태그에 해당하는 스테이지 액터 찾기
			UGameplayStatics::GetAllActorsOfClassWithTag(GetFirstLocalPlayerController(), AStageActor::StaticClass(), LoadedStage->SavedStageName, LoadedTargetStageActors);
			
			// 로드된 태그에 해당하는 스테이지 액터가 레벨에 없을 경우 로드 실패
			if (LoadedTargetStageActors.IsEmpty()) {
				return false;
			}
			else { // 있으면 해당 스테이지로 가기
				AStageActor* SelectedStageActor = Cast<AStageActor>(LoadedTargetStageActors[0]);
				if (SelectedStageActor) {

					APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
					AWorldHUD* WH = Cast< AWorldHUD>(PC->GetHUD());
					if (WH)
					{
						WH->SetLoadingVisible(true);
					}
				

					CurrentStage = SelectedStageActor;


					UQuestSystemComponent::CurQuestState = LoadedStage->QuestState;

					ResetStage();
					return true;
				}
				else {
					return false;
				}
			}
		}
	}

	return false;
}
