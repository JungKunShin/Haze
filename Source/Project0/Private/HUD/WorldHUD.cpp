// Fill out your copyright notice in the Description page of Project Settings.


#include "HUD/WorldHUD.h"
#include "Widget/ConversationWidget.h"
#include "Widget/DeathWidget.h"
#include "Widget/PicturePuzzleWidget.h"
#include "Widget/InstructionWidget.h"
#include "Widget/ObjectDescriptionWidget.h"
#include "Widget/PutDownWidget.h"
#include "Widget/MedicineMixWidget.h"
#include "Widget/HintBookWidget.h"
#include "Widget/EndingCreditWidget.h"
#include "Widget/PauseWidget.h"
#include "Widget/LoadingWidget.h"

#include "Widget/VolumeWidget.h"//삭제요망

#include "Quest/BaseQuest.h"
#include "Kismet/GameplayStatics.h"

void AWorldHUD::BeginPlay()
{
	Super::BeginPlay();
}

void AWorldHUD::SetConversationWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ConversationWidget || !ConversationWidget->IsInViewport())
		{
			if (ConversationWidgetClass)
			{
				ConversationWidget = Cast<UConversationWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), ConversationWidgetClass));
			}
			if (ConversationWidget)
			{
				ConversationWidget->AddToViewport();
				ConversationWidget->SetVisibility(ESlateVisibility::Visible);
			}
			SetInputMode(EInputMode::UIOnly, true);
		}
	}
	else
	{
		if (ConversationWidget)
		{
			ConversationWidget->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
		}
	}

}

void AWorldHUD::SetConversationText(ABaseQuest* Quest)
{
	if (Quest)
	{
		ConversationWidget->SetText(Quest);
	}
}

void AWorldHUD::SetSkipText()
{
	if (!ConversationWidget|| !ConversationWidget->IsInViewport())return;

	ConversationWidget->OnNextButtonClicked();
}



void AWorldHUD::SetDeathWidgetVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!DeathWidget || !DeathWidget->IsInViewport())
		{
			if (DeathWidgetClass)
			{
				DeathWidget = Cast<UDeathWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), DeathWidgetClass));
			}
			if (DeathWidget)
			{
				DeathWidget->AddToViewport();
				DeathWidget->SetVisibility(ESlateVisibility::Visible);
			}
			SetInputMode(EInputMode::UIOnly, true);
		}
	}
	else
	{
		if (DeathWidget)
		{
			DeathWidget->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
		}
	}
}
void AWorldHUD::SetPicturePuzzleVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!PicturePuzzleWidget || !PicturePuzzleWidget->IsInViewport())
		{
			if (PicturePuzzleWidgetClass)
			{
				PicturePuzzleWidget = Cast<UPicturePuzzleWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), PicturePuzzleWidgetClass));
			}
			if (PicturePuzzleWidget)
			{
				PicturePuzzleWidget->AddToViewport();
				PicturePuzzleWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (PicturePuzzleWidget)
		{
			PicturePuzzleWidget->RemoveFromViewport();
		}
	}
}

void AWorldHUD::SetMedicineMixVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!MedicineMixWidget || !MedicineMixWidget->IsInViewport())
		{
			if (MedicineMixWidgetClass)
			{
				MedicineMixWidget = Cast<UMedicineMixWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), MedicineMixWidgetClass));
			}
			if (MedicineMixWidget)
			{
				MedicineMixWidget->AddToViewport();
				MedicineMixWidget->SetVisibility(ESlateVisibility::Visible);
				SetInputMode(EInputMode::GameAndUI, true);
			}
		}
	}
	else
	{
		if (MedicineMixWidget)
		{
			MedicineMixWidget->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
		}
	}
}

void AWorldHUD::SetMedicineCorrectAnswer(AMixGasStove* _CorrectAnswer)
{
	if (!MedicineMixWidget)return;

	MedicineMixWidget->SetCorrectAnswer(_CorrectAnswer);
}

void AWorldHUD::SetInstructionVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!InstructionWidget || !InstructionWidget->IsInViewport())
		{
			if (InstructionWidgetClass)
			{
				InstructionWidget = Cast<UInstructionWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), InstructionWidgetClass));
			}
			if (InstructionWidget)
			{
				InstructionWidget->AddToViewport();
				InstructionWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (InstructionWidget)
		{
			InstructionWidget->RemoveFromViewport();
		}
	}
}

void AWorldHUD::SetInstructionKeep()
{
	if (!InstructionWidget)return;

	InstructionWidget->SetWidgetSwitcherIndex();
}

void AWorldHUD::SetInstructionEnd()
{
	if (!InstructionWidget)return;

	InstructionWidget->SetHiddenAnimation();
}
void AWorldHUD::SetPutDownVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!PutDownWidget || !PutDownWidget->IsInViewport())
		{
			if (PutDownWidgetClass)
			{
				PutDownWidget = Cast<UPutDownWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), PutDownWidgetClass));
			}
			if (PutDownWidget)
			{
				PutDownWidget->AddToViewport();
				PutDownWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (PutDownWidget)
		{
			PutDownWidget->RemoveFromViewport();
		}
	}
}

void AWorldHUD::OpenObjectWidget(const FText& Title, const FText& Body, const FText& Desc, const FText& Hint, bool bStartInReading)
{
	if (!ObjectWidget || !ObjectWidget->IsInViewport())
	{
		if (ObjectWidgetClass)
		{
			ObjectWidget = Cast<UObjectDescriptionWidget>(
				CreateWidget<UUserWidget>(GetOwningPlayerController(), ObjectWidgetClass));
		}

		if (ObjectWidget) { ObjectWidget->AddToViewport(); }
	}

	if (ObjectWidget)
	{
		ObjectWidget->SetIntroTexts(Title, Desc, Hint);
		ObjectWidget->SetTexts(Title, Body);

		ObjectWidget->SetInspectHUDVisible(!bStartInReading);
		ObjectWidget->SetObjectPanelVisible(bStartInReading);
	}

	SetInputMode(EInputMode::GameAndUI, true);
}

void AWorldHUD::CloseObjectWidget()
{
	if (ObjectWidget)
	{
		ObjectWidget->RemoveFromParent();
		ObjectWidget = nullptr;

		SetInputMode(EInputMode::GameOnly, false);
	}
}

bool AWorldHUD::IsObjectWidgetOpen() const
{
	return ObjectWidget && ObjectWidget->IsInViewport();
}



void AWorldHUD::ShowObjectReading(bool bReading)
{
	if (!ObjectWidget) return;
	ObjectWidget->SetInspectHUDVisible(!bReading);
	ObjectWidget->SetObjectPanelVisible(bReading);
}

void AWorldHUD::SetHintBookVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!HintBookWidget || !HintBookWidget->IsInViewport())
		{
			if (HintBookWidgetClass)
			{
				HintBookWidget = Cast<UHintBookWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), HintBookWidgetClass));
			}
			if (HintBookWidget)
			{
				HintBookWidget->AddToViewport();
				HintBookWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (HintBookWidget)
		{
			HintBookWidget->RemoveFromViewport();
		}
	}
}
void AWorldHUD::SetPauseMenuVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!PauseWidget || !PauseWidget->IsInViewport())
		{
			if (PauseWidgetClass)
				PauseWidget = Cast<UPauseWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), PauseWidgetClass));


			if (PauseWidget)
			{
				PauseWidget->AddToViewport(100);
				PauseWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}

		//게임 일시정지 + UI 전용 입력
		UGameplayStatics::SetGamePaused(GetWorld(), true);
		SetInputMode(EInputMode::UIOnly, true);
	}
	else
	{
		if (PauseWidget)
		{
			PauseWidget->RemoveFromParent();
			PauseWidget = nullptr;
		}
		//재개 + 게임 전용 입력
		UGameplayStatics::SetGamePaused(GetWorld(), false);
		SetInputMode(EInputMode::GameOnly, false);
	}
}
void AWorldHUD::SetEndingCreditVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!EndingCreditWidget || !EndingCreditWidget->IsInViewport())
		{
			if (EndingCreditWidgetClass)
			{
				EndingCreditWidget = Cast<UEndingCreditWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), EndingCreditWidgetClass));
			}
			if (EndingCreditWidget)
			{
				EndingCreditWidget->AddToViewport();
				EndingCreditWidget->SetVisibility(ESlateVisibility::Visible);
			}
		}
	}
	else
	{
		if (EndingCreditWidget)
		{
			EndingCreditWidget->RemoveFromViewport();
		}
	}
}

void AWorldHUD::SetLoadingVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!LoadingWidget || !LoadingWidget->IsInViewport())
		{
			if (LoadingWidgetClass)
			{
				LoadingWidget = Cast<ULoadingWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), LoadingWidgetClass));
			}
			if (LoadingWidget)
			{
				LoadingWidget->AddToViewport();
				LoadingWidget->SetVisibility(ESlateVisibility::Visible);
				SetInputMode(EInputMode::UIOnly, false);
			}
		}
	}
	else
	{
		if (LoadingWidget)
		{
			LoadingWidget->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
		}
	}
}

void AWorldHUD::SetVolumeTest()//삭제요망
{
	if (Testbool)
	{
		if (!VolumeTest || !VolumeTest->IsInViewport())
		{
			if (VolumeTestClass)
			{
				VolumeTest = Cast<UVolumeWidget>(CreateWidget<UUserWidget>(GetOwningPlayerController(), VolumeTestClass));
			}
			if (VolumeTest)
			{
				VolumeTest->AddToViewport();
				VolumeTest->SetVisibility(ESlateVisibility::Visible);
				SetInputMode(EInputMode::GameAndUI, true);
			}
		}
	}
	else
	{
		if (VolumeTest)
		{
			VolumeTest->RemoveFromViewport();
			SetInputMode(EInputMode::GameOnly, false);
		}
	}
	Testbool = !Testbool;
}



bool AWorldHUD::IsPauseMenuOpen() const
{
	return PauseWidget && PauseWidget->IsInViewport();
}


void AWorldHUD::SetConversationKeyVisible(bool bOnShow)
{
	if (bOnShow)
	{
		if (!ConversationKeyWidget || !ConversationKeyWidget->IsInViewport())
		{
			if (ConversationKeyWidgetClass)
			{
				APlayerController* PC = GetOwningPlayerController();
				if (!PC) PC = UGameplayStatics::GetPlayerController(this, 0);

				if (PC)
				{
					ConversationKeyWidget = CreateWidget<UUserWidget>(PC, ConversationKeyWidgetClass);
					if (ConversationKeyWidget)
					{
						ConversationKeyWidget->AddToViewport(ConversationKeyZOrder);
					}
				}
			}
		}
	}
	else
	{
		if (ConversationKeyWidget)
		{
			ConversationKeyWidget->RemoveFromParent();
			ConversationKeyWidget = nullptr;
		}
	}
}

void AWorldHUD::ShowConversationKeyOnce(float InDuration)
{
	if (bConversationKeyAlreadyShown) return;

	SetConversationKeyVisible(true);
	bConversationKeyAlreadyShown = true;

	const float Duration = (InDuration > 0.f) ? InDuration : ConversationKeyDuration;

	GetWorldTimerManager().ClearTimer(ConversationKeyTimer);
	GetWorldTimerManager().SetTimer(
		ConversationKeyTimer,
		this, &AWorldHUD::HideConversationKey,
		Duration, false
	);
}

void AWorldHUD::HideConversationKey()
{
	GetWorldTimerManager().ClearTimer(ConversationKeyTimer);
	SetConversationKeyVisible(false);
}