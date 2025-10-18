// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Widget/BaseUserWidget.h"
#include "LoadingWidget.generated.h"

/**
 * 
 */
UCLASS()
class 
	ULoadingWidget : public UBaseUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

private:
	void RemoveLoadingWidget();

private:
	UPROPERTY(Transient, meta = (BindWidgetAnim))
	UWidgetAnimation* LoadingAnimation;			//�ε� �ִϸ��̼� Ʋ��

	FTimerHandle RemoveTH;
};
