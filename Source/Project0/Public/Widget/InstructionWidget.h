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
	UWidgetAnimation* DirectionKeysOn;			//�����¿� �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* DirectionKeysOff;			//�����¿� �ִϸ��̼� ����

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MouseKeyOn;			//���콺 ������ �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* MouseKeyOff;			//���콺 ������ �ִϸ��̼� ����

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* JumpKeyOn;			//���� �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* JumpKeyOff;			//���� �ִϸ��̼� ����

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CrounchKeyOn;			//�ɱ� �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* CrounchKeyOff;			//�ɱ� �ִϸ��̼� ����

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ThrowKeyOn;			//������ �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* ThrowKeyOff;			//������ �ִϸ��̼� ����

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractKeyOn;			//��ȣ�ۿ� �ִϸ��̼� Ʋ��

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* InteractKeyOff;			//��ȣ�ۿ� �ִϸ��̼� ����

	int32 CurSwitchNum = 0;

	FTimerHandle RemoveTH;
};
