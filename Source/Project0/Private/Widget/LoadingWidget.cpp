// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LoadingWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Manager/SoundManager.h"

void ULoadingWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayAnimation(LoadingAnimation, 0.f,0.f);
	GetWorld()->GetTimerManager().SetTimer(RemoveTH, this, &ULoadingWidget::RemoveLoadingWidget, 4.5f, false);
}

void ULoadingWidget::RemoveLoadingWidget()
{
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AWorldPlayerController* WPC = Cast<AWorldPlayerController>(PC);
	AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());

	WPC->GetSoundManager()->PlayBGM(WPC->GetSoundManager()->BGM_World); // 원하는 BGM 지정

	WH->SetLoadingVisible(false);
}
