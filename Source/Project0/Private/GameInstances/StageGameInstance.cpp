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

	// �̹� ����� ������ ������ �ҷ�����
	if (UGameplayStatics::DoesSaveGameExist(StageSaveSlotString, 0)) {
		StageSaveGame = Cast<UStageSaveGame>(
			UGameplayStatics::LoadGameFromSlot(StageSaveSlotString, 0)
		);
	}
	else { // ������ ���̺� ���� ���� �����
		StageSaveGame =
			Cast<UStageSaveGame>(UGameplayStatics::CreateSaveGameObject(UStageSaveGame::StaticClass()));
	}

	// false�� ��ȯ�ϴ� ���� ���̺� ����
	if (!StageSaveGame) return false;

	if (!CurrentStage) {
		UKismetSystemLibrary::PrintString(this, TEXT("CurrentStage Is nullptr"));
		return false;
	}
	else if (CurrentStage->Tags.IsEmpty()) {
		UKismetSystemLibrary::PrintString(this, TEXT("CurrentStage Has No Tag"));
		return false;
	}

	// �Ʒ����ʹ� ���̺� ����
	StageSaveGame->SavedStageName = CurrentStage->Tags[0];
	StageSaveGame->QuestState = UQuestSystemComponent::CurQuestState;

	UGameplayStatics::SaveGameToSlot(
		StageSaveGame, StageSaveSlotString, 0);

	UKismetSystemLibrary::PrintString(this, TEXT("Save Has Successed"));

	return true;
}

bool UStageGameInstance::TryLoadGame()
{
	// ���̺� ���Կ� ����� ����Ǿ������� �ε��ϱ�
	if (UGameplayStatics::DoesSaveGameExist(StageSaveSlotString, 0)) {
		UStageSaveGame* LoadedStage = Cast<UStageSaveGame>(
			UGameplayStatics::LoadGameFromSlot(StageSaveSlotString, 0)
		);
		if (LoadedStage) { // ���̺� ���� �ε� ����
			TArray<AActor*> LoadedTargetStageActors;
			// �ε�� �±׿� �ش��ϴ� �������� ���� ã��
			UGameplayStatics::GetAllActorsOfClassWithTag(GetFirstLocalPlayerController(), AStageActor::StaticClass(), LoadedStage->SavedStageName, LoadedTargetStageActors);
			
			// �ε�� �±׿� �ش��ϴ� �������� ���Ͱ� ������ ���� ��� �ε� ����
			if (LoadedTargetStageActors.IsEmpty()) {
				return false;
			}
			else { // ������ �ش� ���������� ����
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
