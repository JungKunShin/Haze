// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "DeathWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UDeathWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	void ResetReBirthLocation();

protected:
	UPROPERTY(BlueprintReadWrite)
	bool bShouldResetThisWiget = true;

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* Gradation;			// ȭ�� ���� ��ο����� �ִϸ��̼�

	FTimerHandle ResetTH;
};
