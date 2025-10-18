// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "InstructionWidget.generated.h"

/**
 * 
 */

UCLASS()
class PROJECT0_API UInstructionWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

	void SetWidgetSwitcherIndex();

	void SetHiddenAnimation();

private:
	void RemoveInstructionWidget();

private:
	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* TeachWidgetSwitcher;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* DirectionKeysOn;			//상하좌우 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* DirectionKeysOff;			//상하좌우 애니메이션 끄기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MouseKeyOn;			//마우스 움직임 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MouseKeyOff;			//마우스 움직임 애니메이션 끄기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* JumpKeyOn;			//점프 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* JumpKeyOff;			//점프 애니메이션 끄기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CrounchKeyOn;			//앉기 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CrounchKeyOff;			//앉기 애니메이션 끄기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ThrowKeyOn;			//던지기 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ThrowKeyOff;			//던지기 애니메이션 끄기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractKeyOn;			//상호작용 애니메이션 틀기

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractKeyOff;			//상호작용 애니메이션 끄기

	int32 CurSwitchNum = 0;

	FTimerHandle RemoveTH;
};
