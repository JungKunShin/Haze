// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "ConversationWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UQuestSystemComponent;
class ABaseQuest;
class AEnemyCharacter;

USTRUCT(BlueprintType)
struct FConvChoice //시퀀스 선택지 고르는 영역
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FText Label;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) FName Tag; 
	};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnChoicePicked, FName, ChosenTag);


UCLASS()
class PROJECT0_API UConversationWidget : public UBaseUserWidget
{
	GENERATED_BODY()

private:
	const float TYPING_INTERVAL = 0.05f; // 텍스트 출력 간격
	const float PIXEL_SCALE = 1400.f;	 // 줄바꿈 픽셀너비
	
public:
	virtual void NativeConstruct() override;

	//시퀀스 선택지 위아래 버튼
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override; // ★ 추가


	void SetText(ABaseQuest* Quest);

	UFUNCTION()
	void OnNextButtonClicked(); // 거절 버튼

	//QuestComponent에서 선택지 모드 진입/해제
	UFUNCTION(BlueprintCallable)
	void EnterChoiceMode(const TArray<FConvChoice>& InChoices);

	UFUNCTION(BlueprintCallable)
	void ExitChoiceMode();

	//선택 확정 델리게이트: 선택지 Tag 반환
	UPROPERTY(BlueprintAssignable)
	FOnChoicePicked OnChoicePicked;

private:
	void DelayText();

	void ConversationSoundInit();

	bool ShouldBlockAdvance() const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Who;				// 말하는 이

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Conversation;		// 대화 내용

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Next;		// 대화 내용

	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;			// 다음 버튼

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Click;			// 다음 클릭 애니메이션

	//시퀀스 선택지용 버튼
	UPROPERTY(meta = (BindWidgetOptional)) 
	class UVerticalBox* ChoiceBox;

	UPROPERTY(meta = (BindWidgetOptional)) 
	UTextBlock* TEXT_ChoiceA;

	UPROPERTY(meta = (BindWidgetOptional)) 
	UTextBlock* TEXT_ChoiceB;

	FString FullText;		// 전체문장
	FString CurText;		// 현재문장

	bool IsSkip = false;	// 스킵실행여부

	FTimerHandle TypingTH;

	int32 CurrentIndex = 0;	// 현재문장길이


	//선택지 상태
	bool bChoiceMode = false;

	TArray<FConvChoice> Choices;

	int32 Selected = 0; // 0은 A, 1은 B

	void UpdateChoiceVisual();
	void ConfirmChoice();

	ESlateVisibility SavedWhoVis = ESlateVisibility::Visible;
	ESlateVisibility SavedConvVis = ESlateVisibility::Visible;

	//소리관련
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ConversationSound;
};
