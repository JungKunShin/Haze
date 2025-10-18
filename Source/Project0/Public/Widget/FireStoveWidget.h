// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "../Actors/Mix/MixGasStove.h"
#include "FireStoveWidget.generated.h"

/**
 * 
 */

class UButton;
class UImage;
class UTextBlock;

UCLASS()
class PROJECT0_API UFireStoveWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	UFireStoveWidget();

	virtual void NativeConstruct() override;

	void InitFireStove();

	UFUNCTION()
	void OnFireUpButtonClicked(); // 불조절 위 버튼
	UFUNCTION()
	void OnFireDownButtonClicked(); // 불조절 아래 버튼
	UFUNCTION()
	void OnTimeUpButtonClicked(); // 시간조절 위 버튼
	UFUNCTION()
	void OnTimeDownButtonClicked(); // 시간조절 아래 버튼
	UFUNCTION()
	void OnTempUpButtonClicked(); // 온도조절 위 버튼
	UFUNCTION()
	void OnTempDownButtonClicked(); // 온도조절 아래 버튼

	void ParentJudgement();
private:
	UPROPERTY(meta = (BindWidget))
	UButton* Fire_UpButton;			// 불조절 위 버튼

	UPROPERTY(meta = (BindWidget))
	UButton* Fire_DownButton;			// 불조절 아래 버튼

	UPROPERTY(meta = (BindWidget))
	UButton* Time_UpButton;			// 시간조절 위 버튼

	UPROPERTY(meta = (BindWidget))
	UButton* Time_DownButton;			// 시간조절 아래 버튼

	UPROPERTY(meta = (BindWidget))
	UButton* Temperature_UpButton;			// 온도조절 위 버튼

	UPROPERTY(meta = (BindWidget))
	UButton* Temperature_DownButton;			// 온도조절 아래 버튼

	UPROPERTY(meta = (BindWidget))
	UImage* WeakFireImage;

	UPROPERTY(meta = (BindWidget))
	UImage* MiddleFireImage;

	UPROPERTY(meta = (BindWidget))
	UImage* StrongFireImage;

	UPROPERTY(meta = (BindWidget))
	UImage* WeakFireShadow;

	UPROPERTY(meta = (BindWidget))
	UImage* MiddleFireShadow;

	UPROPERTY(meta = (BindWidget))
	UImage* StrongFireShadow;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Time;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Temp;

public:
	FMixAnswer CurAnswerState;
};
