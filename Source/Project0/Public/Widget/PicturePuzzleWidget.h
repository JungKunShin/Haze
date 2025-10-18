// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "PicturePuzzleWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UImage;

UCLASS()
class PROJECT0_API UPicturePuzzleWidget : public UBaseUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void SetVisibleLeftAndRight();

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Left;				// ����

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Right;				// ������

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Exit;				// ������

	UPROPERTY(meta = (BindWidget))
	UImage* LeftImage;				// �����̹���

	UPROPERTY(meta = (BindWidget))
	UImage* RightImage;				// �������̹���

	UPROPERTY(meta = (BindWidget))
	UImage* ExitImage;				// ������
	
};
