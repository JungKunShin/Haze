// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/ConversationWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "Quest/BaseQuest.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Manager/SoundManager.h"


void UConversationWidget::NativeConstruct()
{
	Super::NativeConstruct();

	NextButton->OnClicked.AddDynamic(this, &UConversationWidget::OnNextButtonClicked);

	TEXT_Next->SetVisibility(ESlateVisibility::Hidden);
	TEXT_Conversation->SetWrapTextAt(PIXEL_SCALE);

	if (NextButton)
	{
		NextButton->SetIsEnabled(true);
		NextButton->SetFocus();
	}

	//������ ó���� �Ⱥ��̰�
	if (ChoiceBox) ChoiceBox->SetVisibility(ESlateVisibility::Collapsed);

	SetIsFocusable(true);


	PlayAnimation(Click,0.f,0);
}

FReply UConversationWidget::NativeOnKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{

	if (ShouldBlockAdvance())
	{
		const FKey Key = InKeyEvent.GetKey();
		if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom)
		{
			return FReply::Handled();
		}
	}


	if (bChoiceMode)
	{
		const FKey Key = InKeyEvent.GetKey();
		if (Key == EKeys::Up || Key == EKeys::W)
		{
			Selected = FMath::Clamp(Selected - 1, 0, 1);
			UpdateChoiceVisual();
			return FReply::Handled();
		}
		if (Key == EKeys::Down || Key == EKeys::S)
		{
			Selected = FMath::Clamp(Selected + 1, 0, 1);
			UpdateChoiceVisual();
			return FReply::Handled();
		}
		if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom)
		{
			ConfirmChoice();
			return FReply::Handled();
		}
	}
	return Super::NativeOnKeyDown(MyGeometry, InKeyEvent);
}

FReply UConversationWidget::NativeOnPreviewKeyDown(const FGeometry& MyGeometry, const FKeyEvent& InKeyEvent)
{
	if (ShouldBlockAdvance())
	{
		const FKey Key = InKeyEvent.GetKey();
		if (Key == EKeys::Enter || Key == EKeys::SpaceBar || Key == EKeys::Gamepad_FaceButton_Bottom)
		{
			return FReply::Handled();
		}
	}
	return Super::NativeOnPreviewKeyDown(MyGeometry, InKeyEvent);
}

void UConversationWidget::SetText(ABaseQuest* Quest)
{
	FullText = Quest->ConversationData.Conversation.ToString();
	if (TEXT_Who)
	{
		TEXT_Who->SetText(Quest->ConversationData.Who);
	}
	if (TEXT_Conversation)
	{
		GetWorld()->GetTimerManager().SetTimer(TypingTH, this, &UConversationWidget::DelayText, TYPING_INTERVAL, true);
	}
}

void UConversationWidget::DelayText()
{
	if (CurrentIndex < FullText.Len())
	{
		CurText += FullText[CurrentIndex];
		CurrentIndex++;

		// �ؽ�Ʈ ������ �ݿ�
		TEXT_Conversation->SetText(FText::FromString(CurText));
		ConversationSoundInit();
	}
	else
	{
		// �� ��µǾ����� Ÿ�̸� ����
		TEXT_Next->SetVisibility(ESlateVisibility::Visible);
		CurrentIndex = 0;
		CurText = nullptr;
		IsSkip = true;
		GetWorld()->GetTimerManager().ClearTimer(TypingTH);
		
	}
}

void UConversationWidget::ConversationSoundInit()
{
	if (ConversationSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PC);

		UGameplayStatics::PlaySound2D(this, ConversationSound, WPC->GetSoundManager()->UIVolume, 1.0f);
	}
}

bool UConversationWidget::ShouldBlockAdvance() const
{

	if (APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwningPlayerPawn()))
	{
		if (Owner->QuestSystem)
		{
			return Owner->QuestSystem->IsSequenceInputBlocked();
		}
	}
	return false;

}


void UConversationWidget::OnNextButtonClicked()
{
	if (ShouldBlockAdvance())
	{
		return; 
	}

	if (bChoiceMode)
	{
		ConfirmChoice();
		return;
	}

	if (IsSkip)
	{
		WidgetSpecialSoundInit();
		TEXT_Next->SetVisibility(ESlateVisibility::Hidden);							// �������� �����
		APlayerCharacter* Owner = Cast<APlayerCharacter>(GetOwningPlayerPawn());
		if (Owner)
		{
			Owner->QuestSystem->Conversation(Owner->QuestSystem->CurQuestState);	// ������� ��ȯ
		}
	}
	else
	{
		TEXT_Conversation->SetText(FText::FromString(FullText));	// ��ü���� ���
		TEXT_Next->SetVisibility(ESlateVisibility::Visible);		// �������� �����ֱ�
		CurrentIndex = 0;
		CurText = nullptr;
		GetWorld()->GetTimerManager().ClearTimer(TypingTH);
	}
	IsSkip = !IsSkip;
}

void UConversationWidget::EnterChoiceMode(const TArray<FConvChoice>& InChoices)
{
	Choices = InChoices;
	Selected = 0;
	bChoiceMode = true;

	//��ȭ �ؽ�Ʈ�� ���� �ΰ� ������ �ڽ��� ���̰�
	if (TEXT_Next) TEXT_Next->SetVisibility(ESlateVisibility::Hidden);
	if (ChoiceBox) ChoiceBox->SetVisibility(ESlateVisibility::Visible);

	if (TEXT_Who) {
		SavedWhoVis = TEXT_Who->GetVisibility();
		TEXT_Who->SetVisibility(ESlateVisibility::Collapsed);
	}
	if (TEXT_Conversation) {
		SavedConvVis = TEXT_Conversation->GetVisibility();
		TEXT_Conversation->SetVisibility(ESlateVisibility::Collapsed);
	}

	if (TEXT_ChoiceA) 
	TEXT_ChoiceA->SetText(Choices.IsValidIndex(0) ? Choices[0].Label : FText());
	
	if (TEXT_ChoiceB) 
	TEXT_ChoiceB->SetText(Choices.IsValidIndex(1) ? Choices[1].Label : FText());
	UpdateChoiceVisual();

	//Ű �Է��� ������ �޵��� ��Ŀ��
	UWidgetBlueprintLibrary::SetInputMode_UIOnlyEx(GetOwningPlayer(), this);
	SetKeyboardFocus();
}

void UConversationWidget::ExitChoiceMode()
{
	bChoiceMode = false;
	if (ChoiceBox) ChoiceBox->SetVisibility(ESlateVisibility::Collapsed);

	if (TEXT_Who) TEXT_Who->SetVisibility(SavedWhoVis);
	if (TEXT_Conversation) TEXT_Conversation->SetVisibility(SavedConvVis);
}

void UConversationWidget::UpdateChoiceVisual()
{
	if (TEXT_ChoiceA) TEXT_ChoiceA->SetOpacity(Selected == 0 ? 1.f : 0.5f);
	if (TEXT_ChoiceB) TEXT_ChoiceB->SetOpacity(Selected == 1 ? 1.f : 0.5f);
}

void UConversationWidget::ConfirmChoice()
{
	if (!Choices.IsValidIndex(Selected)) return;
	OnChoicePicked.Broadcast(Choices[Selected].Tag);
	ExitChoiceMode();
}
