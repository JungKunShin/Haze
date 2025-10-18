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
struct FConvChoice //������ ������ ���� ����
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
	const float TYPING_INTERVAL = 0.05f; // �ؽ�Ʈ ��� ����
	const float PIXEL_SCALE = 1400.f;	 // �ٹٲ� �ȼ��ʺ�
	
public:
	virtual void NativeConstruct() override;

	//������ ������ ���Ʒ� ��ư
	virtual FReply NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override;
	virtual FReply NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent) override; // �� �߰�


	void SetText(ABaseQuest* Quest);

	UFUNCTION()
	void OnNextButtonClicked(); // ���� ��ư

	//QuestComponent���� ������ ��� ����/����
	UFUNCTION(BlueprintCallable)
	void EnterChoiceMode(const TArray<FConvChoice>& InChoices);

	UFUNCTION(BlueprintCallable)
	void ExitChoiceMode();

	//���� Ȯ�� ��������Ʈ: ������ Tag ��ȯ
	UPROPERTY(BlueprintAssignable)
	FOnChoicePicked OnChoicePicked;

private:
	void DelayText();

	void ConversationSoundInit();

	bool ShouldBlockAdvance() const;

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Who;				// ���ϴ� ��

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Conversation;		// ��ȭ ����

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Next;		// ��ȭ ����

	UPROPERTY(meta = (BindWidget))
	class UButton* NextButton;			// ���� ��ư

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Click;			// ���� Ŭ�� �ִϸ��̼�

	//������ �������� ��ư
	UPROPERTY(meta = (BindWidgetOptional)) 
	class UVerticalBox* ChoiceBox;

	UPROPERTY(meta = (BindWidgetOptional)) 
	UTextBlock* TEXT_ChoiceA;

	UPROPERTY(meta = (BindWidgetOptional)) 
	UTextBlock* TEXT_ChoiceB;

	FString FullText;		// ��ü����
	FString CurText;		// ���繮��

	bool IsSkip = false;	// ��ŵ���࿩��

	FTimerHandle TypingTH;

	int32 CurrentIndex = 0;	// ���繮�����


	//������ ����
	bool bChoiceMode = false;

	TArray<FConvChoice> Choices;

	int32 Selected = 0; // 0�� A, 1�� B

	void UpdateChoiceVisual();
	void ConfirmChoice();

	ESlateVisibility SavedWhoVis = ESlateVisibility::Visible;
	ESlateVisibility SavedConvVis = ESlateVisibility::Visible;

	//�Ҹ�����
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ConversationSound;
};
