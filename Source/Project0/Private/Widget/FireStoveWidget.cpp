// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/FireStoveWidget.h"
#include "Widget/MedicineMixWidget.h"
#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

UFireStoveWidget::UFireStoveWidget()
{
	CurAnswerState.Fire = EFireState::Weak;
	CurAnswerState.Time = EUseTime::Half;
	CurAnswerState.Temperature = 90;
}

void UFireStoveWidget::NativeConstruct()
{
	Super::NativeConstruct();

	Fire_UpButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnFireUpButtonClicked);
	Fire_DownButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnFireDownButtonClicked);
	Time_UpButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnTimeUpButtonClicked);
	Time_DownButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnTimeDownButtonClicked);
	Temperature_UpButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnTempUpButtonClicked);
	Temperature_DownButton->OnClicked.AddDynamic(this, &UFireStoveWidget::OnTempDownButtonClicked);

	InitFireStove();
}

void UFireStoveWidget::InitFireStove()
{
	CurAnswerState.Fire = EFireState::Weak;
	CurAnswerState.Time = EUseTime::Half;
	CurAnswerState.Temperature = 90;

	WeakFireImage->SetVisibility(ESlateVisibility::Visible);
	MiddleFireImage->SetVisibility(ESlateVisibility::Hidden);
	StrongFireImage->SetVisibility(ESlateVisibility::Hidden);
	WeakFireShadow->SetVisibility(ESlateVisibility::Hidden);
	MiddleFireShadow->SetVisibility(ESlateVisibility::Visible);
	StrongFireShadow->SetVisibility(ESlateVisibility::Visible);

	TEXT_Time->SetText(FText::FromString("00 : 30"));
	TEXT_Temp->SetText(FText::AsNumber(CurAnswerState.Temperature));
}

void UFireStoveWidget::OnFireUpButtonClicked()
{
	switch (CurAnswerState.Fire)
	{
	case EFireState::Weak:
		CurAnswerState.Fire = EFireState::Middle;
		WeakFireImage->SetVisibility(ESlateVisibility::Hidden);
		WeakFireShadow->SetVisibility(ESlateVisibility::Visible);
		MiddleFireImage->SetVisibility(ESlateVisibility::Visible);
		MiddleFireShadow->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EFireState::Middle:
		CurAnswerState.Fire = EFireState::Strong;
		MiddleFireImage->SetVisibility(ESlateVisibility::Hidden);
		MiddleFireShadow->SetVisibility(ESlateVisibility::Visible);
		StrongFireImage->SetVisibility(ESlateVisibility::Visible);
		StrongFireShadow->SetVisibility(ESlateVisibility::Hidden);
		break;
	case EFireState::Strong:
		CurAnswerState.Fire = EFireState::Weak;
		StrongFireImage->SetVisibility(ESlateVisibility::Hidden);
		StrongFireShadow->SetVisibility(ESlateVisibility::Visible);
		WeakFireImage->SetVisibility(ESlateVisibility::Visible);
		WeakFireShadow->SetVisibility(ESlateVisibility::Hidden);
		break;
	}
	ParentJudgement();
}

void UFireStoveWidget::OnFireDownButtonClicked()
{
	switch (CurAnswerState.Fire)
	{
	case EFireState::Weak:
		CurAnswerState.Fire = EFireState::Strong;
		StrongFireImage->SetVisibility(ESlateVisibility::Visible);
		StrongFireShadow->SetVisibility(ESlateVisibility::Hidden);
		WeakFireImage->SetVisibility(ESlateVisibility::Hidden);
		WeakFireShadow->SetVisibility(ESlateVisibility::Visible);
		break;
	case EFireState::Middle:
		CurAnswerState.Fire = EFireState::Weak;
		WeakFireImage->SetVisibility(ESlateVisibility::Visible);
		WeakFireShadow->SetVisibility(ESlateVisibility::Hidden);
		MiddleFireImage->SetVisibility(ESlateVisibility::Hidden);
		MiddleFireShadow->SetVisibility(ESlateVisibility::Visible);
		break;
	case EFireState::Strong:
		CurAnswerState.Fire = EFireState::Middle;
		MiddleFireImage->SetVisibility(ESlateVisibility::Visible);
		MiddleFireShadow->SetVisibility(ESlateVisibility::Hidden);
		StrongFireImage->SetVisibility(ESlateVisibility::Hidden);
		StrongFireShadow->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	ParentJudgement();
}

void UFireStoveWidget::OnTimeUpButtonClicked()
{
	switch (CurAnswerState.Time)
	{
	case EUseTime::Half:
		CurAnswerState.Time = EUseTime::OneHour;
		TEXT_Time->SetText(FText::FromString("01 : 00"));
		break;
	case EUseTime::OneHour:
		CurAnswerState.Time = EUseTime::OneHalf;
		TEXT_Time->SetText(FText::FromString("01 : 30"));
		break;
	case EUseTime::OneHalf:
		CurAnswerState.Time = EUseTime::TwoHour;
		TEXT_Time->SetText(FText::FromString("02 : 00"));
		break;
	case EUseTime::TwoHour:
		CurAnswerState.Time = EUseTime::TwoHalf;
		TEXT_Time->SetText(FText::FromString("02 : 30"));
		break;
	case EUseTime::TwoHalf:
		CurAnswerState.Time = EUseTime::ThreeHour;
		TEXT_Time->SetText(FText::FromString("03 : 00"));
		break;
	case EUseTime::ThreeHour:
		CurAnswerState.Time = EUseTime::Half;
		TEXT_Time->SetText(FText::FromString("00 : 30"));
		break;
	}

	ParentJudgement();
}

void UFireStoveWidget::OnTimeDownButtonClicked()
{
	switch (CurAnswerState.Time)
	{
	case EUseTime::Half:
		CurAnswerState.Time = EUseTime::ThreeHour;
		TEXT_Time->SetText(FText::FromString("03 : 00"));
		break;
	case EUseTime::OneHour:
		CurAnswerState.Time = EUseTime::Half;
		TEXT_Time->SetText(FText::FromString("00 : 30"));
		break;
	case EUseTime::OneHalf:
		CurAnswerState.Time = EUseTime::OneHour;
		TEXT_Time->SetText(FText::FromString("01 : 00"));
		break;
	case EUseTime::TwoHour:
		CurAnswerState.Time = EUseTime::OneHalf;
		TEXT_Time->SetText(FText::FromString("01 : 30"));
		break;
	case EUseTime::TwoHalf:
		CurAnswerState.Time = EUseTime::TwoHour;
		TEXT_Time->SetText(FText::FromString("02 : 00"));
		break;
	case EUseTime::ThreeHour:
		CurAnswerState.Time = EUseTime::TwoHalf;
		TEXT_Time->SetText(FText::FromString("02 : 30"));
		break;
	}
	ParentJudgement();
}

void UFireStoveWidget::OnTempUpButtonClicked()
{
	CurAnswerState.Temperature += 10;
	if (CurAnswerState.Temperature > 200)
	{
		CurAnswerState.Temperature = 90;
	}
	TEXT_Temp->SetText(FText::AsNumber(CurAnswerState.Temperature));
	ParentJudgement();
}

void UFireStoveWidget::OnTempDownButtonClicked()
{
	CurAnswerState.Temperature -= 10;
	if (CurAnswerState.Temperature < 90)
	{
		CurAnswerState.Temperature = 200;
	}
	TEXT_Temp->SetText(FText::AsNumber(CurAnswerState.Temperature));
	ParentJudgement();
}

void UFireStoveWidget::ParentJudgement()
{
	WidgetButtonSoundInit();
	if (UMedicineMixWidget* MyParent = GetTypedOuter<UMedicineMixWidget>())
	{
		MyParent->AnswerJudgement();
	}
}
