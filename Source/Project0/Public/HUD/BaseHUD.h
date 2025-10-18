// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "BaseHUD.generated.h"

/**
 * 
 */
UENUM()
enum class EInputMode : uint8 //플레이어 직업
{
	UIOnly UMETA(DisplayName = "UIOnly"),

	GameAndUI    UMETA(DisplayName = "GameAndUI"),

	GameOnly  UMETA(DisplayName = "GameOnly")
};

UCLASS()
class PROJECT0_API ABaseHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;
	void SetInputMode(EInputMode InputMode, bool ShowMouseCurSor);
};
