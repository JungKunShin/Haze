// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Actors/TriggerFinal.h"
#include "QuestSystemComponent.generated.h"

class ABaseQuest;
class AEnemyCharacter;

UENUM(BlueprintType)
enum class EQuestState : uint8
{
	None,
	FirstConversation,
	DaughterBook,
	MomTalking,
	MomBarn,
	MomMix,
	MazeBefore,
	MazeMiddle,
	MazeAfter,
	SequenceOne,
	SequenceTwo,
	End,
	GrandFatherDeath,
	ZombieDeath
};

UENUM()
enum class ETalker : uint8 
{ 
	None, Sister, Mom 
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT0_API UQuestSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
// Sets default values for this component's properties
	UQuestSystemComponent();

	void SetTalkerSister() { PendingTalker = ETalker::Sister; }
	void SetTalkerMom() { PendingTalker = ETalker::Mom; }
protected:
// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void QuestProgress(EQuestState _CurQuest);
	
	//블루프린트에서 퀘스트 단계 진행을 위해 만든 함수
	UFUNCTION(BlueprintCallable, Category = "Quest")
	void AdvanceCurrentState();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void Conversation(EQuestState _CurQuest);

	void GrandFatherDeath();

UFUNCTION(BlueprintCallable, Category = "Quest")
	EQuestState CurrentQuestStateJudgement();//이 아래로는 트리거 관리 함수, 변수
	void EnterMazeAfter();	
	void BindFinalTriggersOnce();

	void SetFinalTriggersEnabled(bool bEnable); 

	UFUNCTION() 
	void OnFinalPrompt(ATriggerFinal* Trigger);

	UFUNCTION() void 
	OnFinalChoicePicked(FName Tag);
	



	UPROPERTY() 
	TArray<TObjectPtr<ATriggerFinal>> FinalTriggers;

	UPROPERTY() 
	TObjectPtr<ATriggerFinal> ActiveTrigger = nullptr;


	bool bFinalBound = false;
	bool bChoiceOpen = false;

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceStartDialogueByPick(EFinalPick Pick);          

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceNextLine();

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceJumpToRow(int32 Row);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceFinishDialogue(EQuestState NextStateAfter = EQuestState::End);

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceStartDialogue_SeqA() { SequenceStartDialogueByPick(EFinalPick::PickA); }

	UFUNCTION(BlueprintCallable, Category = "Quest")
	void SequenceStartDialogue_SeqB() { SequenceStartDialogueByPick(EFinalPick::PickB); }
	
private:
	void ChapterOneInit();

	void ChapterTwoInit();

	void ChapterThreeInit();

	void ChapterFourInit();

	void ChapterFiveInit();

	void SequenceOneInit();

	void SequenceTwoInit();

	bool bSeqInputBlocked = false;

	void SetSequenceInputBlocked(bool bBlocked);

public:
	static EQuestState CurQuestState;

	EQuestState HitTempQuestState=EQuestState::None;

	UPROPERTY()
	ABaseQuest* CurQuest;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseQuest> ChapterOne;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseQuest> ChapterTwo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseQuest> ChapterThree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseQuest> ChapterFour;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseQuest> ChapterFive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseQuest> SequenceOneClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ABaseQuest> SequenceTwoClass;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ABaseQuest> GrandFatherDeathState;

	UPROPERTY()
	ABaseQuest* ChapterOneINS=nullptr;

	UPROPERTY()
	ABaseQuest* ChapterTwoINS = nullptr;

	UPROPERTY()
	ABaseQuest* ChapterThreeINS = nullptr;

	UPROPERTY()
	ABaseQuest* ChapterFourINS = nullptr;

	UPROPERTY()
	ABaseQuest* ChapterFiveINS = nullptr;

	UPROPERTY() 
	ABaseQuest* SequenceOneINS = nullptr;

	UPROPERTY() 
	ABaseQuest* SequenceTwoINS = nullptr;

	UPROPERTY()
	ABaseQuest* GrandFatherDeathINS = nullptr;

	UFUNCTION(BlueprintPure, Category = "Quest")
	bool IsSequenceInputBlocked() const { return bSeqInputBlocked; }

private: 
	
	ETalker PendingTalker = ETalker::None;
	EQuestState PendingSequenceState = EQuestState::None;

};
