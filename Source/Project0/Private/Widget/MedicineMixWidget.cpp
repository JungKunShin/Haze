// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/MedicineMixWidget.h"
#include "Widget/FireStoveWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"
#include "Components/VerticalBox.h"
#include "Components/Image.h"

void UMedicineMixWidget::NativeConstruct()
{
	Super::NativeConstruct();

	FadeOutVerticalBox->SetVisibility(ESlateVisibility::Hidden);
	FadeOutImage->SetVisibility(ESlateVisibility::Hidden);

	if (AMixGasStove::FindMedicine != 3)
	{
		TotalVerticalBox->SetVisibility(ESlateVisibility::Hidden);
		FadeOutVerticalBox->SetVisibility(ESlateVisibility::Visible);
		FadeOutImage->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMedicineMixWidget::SetCorrectAnswer(AMixGasStove* _CorrectAnswer)
{
	CorrectAnswer = _CorrectAnswer;
}

void UMedicineMixWidget::AnswerJudgement()
{
	IsFirst = false;
	IsSecond = false;
	IsThird = false;
	if (FirstStoveWidget->CurAnswerState.Fire == CorrectAnswer->FirstAnswer.Fire &&
		FirstStoveWidget->CurAnswerState.Time == CorrectAnswer->FirstAnswer.Time &&
		FirstStoveWidget->CurAnswerState.Temperature == CorrectAnswer->FirstAnswer.Temperature)
	{
		IsFirst = true;
	}
	if (SecondStoveWidget->CurAnswerState.Fire == CorrectAnswer->SecondAnswer.Fire &&
		SecondStoveWidget->CurAnswerState.Time == CorrectAnswer->SecondAnswer.Time &&
		SecondStoveWidget->CurAnswerState.Temperature == CorrectAnswer->SecondAnswer.Temperature)
	{
		IsSecond = true;
	}
	if (ThirdStoveWidget->CurAnswerState.Fire == CorrectAnswer->ThirdAnswer.Fire &&
		ThirdStoveWidget->CurAnswerState.Time == CorrectAnswer->ThirdAnswer.Time &&
		ThirdStoveWidget->CurAnswerState.Temperature == CorrectAnswer->ThirdAnswer.Temperature)
	{
		IsThird = true;
	}
	if (IsFirst && IsSecond && IsThird)
	{
		TotalVerticalBox->SetVisibility(ESlateVisibility::Hidden);
		FadeOutVerticalBox->SetVisibility(ESlateVisibility::Visible);
		FadeOutImage->SetVisibility(ESlateVisibility::Visible);
		PlayAnimation(SuccessFadeOut);

		AMixGasStove::FindMedicine++;
	}
	
}
