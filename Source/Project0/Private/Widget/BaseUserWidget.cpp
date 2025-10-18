// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/BaseUserWidget.h"
#include "Components/AudioComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Manager/SoundManager.h"

void UBaseUserWidget::NativeConstruct()
{
	Super::NativeConstruct();

	WidgetBeginSoundInit();
}

void UBaseUserWidget::WidgetBeginSoundInit()
{
	if (BeginSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PC);

		UGameplayStatics::PlaySound2D(this, BeginSound, WPC->GetSoundManager()->UIVolume, 1.0f);
	}
}

void UBaseUserWidget::WidgetSpecialSoundInit()
{
	if (SpecialSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PC);

		UGameplayStatics::PlaySound2D(this, SpecialSound, WPC->GetSoundManager()->UIVolume, 1.0f);
	}
}

void UBaseUserWidget::WidgetButtonSoundInit()
{
	if (ButtonSound)
	{
		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PC);

		UGameplayStatics::PlaySound2D(this, ButtonSound, WPC->GetSoundManager()->UIVolume, 1.0f);
	}
}
