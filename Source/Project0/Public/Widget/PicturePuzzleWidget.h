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
	UTextBlock* TEXT_Left;				// 왼쪽

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Right;				// 오른쪽

	UPROPERTY(meta = (BindWidget))
	UTextBlock* TEXT_Exit;				// 나가기

	UPROPERTY(meta = (BindWidget))
	UImage* LeftImage;				// 왼쪽이미지

	UPROPERTY(meta = (BindWidget))
	UImage* RightImage;				// 오른쪽이미지

	UPROPERTY(meta = (BindWidget))
	UImage* ExitImage;				// 나가기
	
};
