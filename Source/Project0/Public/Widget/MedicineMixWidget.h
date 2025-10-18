// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "../Actors/Mix/MixGasStove.h"
#include "MedicineMixWidget.generated.h"

/**
 * 
 */
class UButton;
class UFireStoveWidget;

UCLASS()
class PROJECT0_API UMedicineMixWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetCorrectAnswer(AMixGasStove* _CorrectAnswer);

	void AnswerJudgement();
private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* SuccessFadeOut;			// 성공 애니메이션

	AMixGasStove* CorrectAnswer;
	
	UPROPERTY(meta = (BindWidget))
	UFireStoveWidget* FirstStoveWidget;  

	UPROPERTY(meta = (BindWidget))
	UFireStoveWidget* SecondStoveWidget;

	UPROPERTY(meta = (BindWidget))
	UFireStoveWidget* ThirdStoveWidget;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* TotalVerticalBox;

	UPROPERTY(meta = (BindWidget))
	class UImage* FadeOutImage;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox* FadeOutVerticalBox;


	bool IsFirst = false;
	bool IsSecond = false;
	bool IsThird = false;

	FTimerHandle RemoveTH;
};
