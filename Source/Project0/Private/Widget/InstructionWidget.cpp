// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/InstructionWidget.h"
#include "Components/WidgetSwitcher.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"

void UInstructionWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UInstructionWidget::SetWidgetSwitcherIndex()
{
	TeachWidgetSwitcher->SetActiveWidgetIndex(CurSwitchNum);
	
	switch (CurSwitchNum)
	{
		case 0: 
			PlayAnimation(DirectionKeysOn, 0.f);
			break;
		case 1:
			PlayAnimation(MouseKeyOn, 0.f);
			break;
		case 2:
			PlayAnimation(JumpKeyOn, 0.f);
			break;
		case 3:
			PlayAnimation(CrounchKeyOn, 0.f);
			break;
		case 4:
			PlayAnimation(InteractKeyOn, 0.f);
			break;
		case 5:
			PlayAnimation(ThrowKeyOn, 0.f);
			break;
		default:
			break;
	}
	
}

void UInstructionWidget::SetHiddenAnimation()
{
	switch (CurSwitchNum)
	{
	case 0:
		PlayAnimation(DirectionKeysOff, 0.f);
		break;
	case 1:
		PlayAnimation(MouseKeyOff, 0.f);
		break;
	case 2:
		PlayAnimation(JumpKeyOff, 0.f);
		break;
	case 3:
		PlayAnimation(CrounchKeyOff, 0.f);
		break;
	case 4:
		PlayAnimation(InteractKeyOff, 0.f);
		break;
	case 5:
		PlayAnimation(ThrowKeyOff, 0.f);
		GetWorld()->GetTimerManager().SetTimer(RemoveTH, this, &UInstructionWidget::RemoveInstructionWidget, 5.0f, false);
		break;
	default:
		break;
	}

	++CurSwitchNum;
}

void UInstructionWidget::RemoveInstructionWidget()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AWorldHUD* WH = Cast< AWorldHUD>(PC->GetHUD());
	WH->SetInstructionVisible(false);
}
