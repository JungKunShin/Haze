// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "EndingCreditWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UEndingCreditWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
private:
    const float DOWN_START = 1.0f;

public:
    virtual void NativeConstruct() override;

    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* CreditsScrollBox;

    void DelayTitleLevel();

private:
    float ScrollSpeed = 150.f; // 초당 픽셀 단위 스크롤 속도
    float CurrentOffset = 0.f;

    float WaitTime = 0.f;
    bool IsFinished = false;
    FTimerHandle LevelMoveTH;
};
