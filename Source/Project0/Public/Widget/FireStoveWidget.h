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
	void OnFireUpButtonClicked(); // ������ �� ��ư
	UFUNCTION()
	void OnFireDownButtonClicked(); // ������ �Ʒ� ��ư
	UFUNCTION()
	void OnTimeUpButtonClicked(); // �ð����� �� ��ư
	UFUNCTION()
	void OnTimeDownButtonClicked(); // �ð����� �Ʒ� ��ư
	UFUNCTION()
	void OnTempUpButtonClicked(); // �µ����� �� ��ư
	UFUNCTION()
	void OnTempDownButtonClicked(); // �µ����� �Ʒ� ��ư

	void ParentJudgement();
private:
	UPROPERTY(meta = (BindWidget))
	UButton* Fire_UpButton;			// ������ �� ��ư

	UPROPERTY(meta = (BindWidget))
	UButton* Fire_DownButton;			// ������ �Ʒ� ��ư

	UPROPERTY(meta = (BindWidget))
	UButton* Time_UpButton;			// �ð����� �� ��ư

	UPROPERTY(meta = (BindWidget))
	UButton* Time_DownButton;			// �ð����� �Ʒ� ��ư

	UPROPERTY(meta = (BindWidget))
	UButton* Temperature_UpButton;			// �µ����� �� ��ư

	UPROPERTY(meta = (BindWidget))
	UButton* Temperature_DownButton;			// �µ����� �Ʒ� ��ư

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
