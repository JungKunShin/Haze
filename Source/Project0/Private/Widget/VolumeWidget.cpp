// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/VolumeWidget.h"
#include "PlayerControllers/BasePlayerController.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"
#include "Manager/SoundManager.h"
#include "Components/TextBlock.h"

void UVolumeWidget::NativeConstruct()
{
	Super::NativeConstruct();

    AudioVolumeSlider_Master->OnValueChanged.AddDynamic(this, &UVolumeWidget::OnMasterVolumeChanged);
    AudioVolumeSlider_BackGround->OnValueChanged.AddDynamic(this, &UVolumeWidget::OnBGMVolumeChanged);
    AudioVolumeSlider_Effect->OnValueChanged.AddDynamic(this, &UVolumeWidget::OnEffectVolumeChanged);
    AudioVolumeSlider_UI->OnValueChanged.AddDynamic(this, &UVolumeWidget::OnUIVolumeChanged);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    if (Cast<AWorldPlayerController>(PC))
    {
        InitVolumeSound();
    }
    else
    {
        GetWorld()->GetTimerManager().ClearTimer(SoundInitTH);
        GetWorld()->GetTimerManager().SetTimer(SoundInitTH, this, &UVolumeWidget::InitVolumeSound, 0.3f, false);
    }
    //InitVolumeSound();
}

void UVolumeWidget::OnMasterVolumeChanged(float NewValue)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->GetSoundManager()->SetMasterVolume(NewValue);

    TEXT_Master->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->MasterVolume * 100.f)));
}

void UVolumeWidget::OnBGMVolumeChanged(float NewValue)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->GetSoundManager()->SetBGMVolume(NewValue);

    TEXT_BGM->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->BGMVolume * 100.f)));
}

void UVolumeWidget::OnEffectVolumeChanged(float NewValue)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->GetSoundManager()->SetEffectVolume(NewValue);

    TEXT_Effect->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->EffectVolume * 100.f)));
}

void UVolumeWidget::OnUIVolumeChanged(float NewValue)
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->GetSoundManager()->SetUIVolume(NewValue);

    TEXT_UI->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->OriginalUIVolume * 100.f)));
}

void UVolumeWidget::InitVolumeSound()
{
    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);

    UKismetSystemLibrary::PrintString(this, UKismetSystemLibrary::GetDisplayName(BPC));
    UKismetSystemLibrary::PrintString(this, UKismetSystemLibrary::GetDisplayName(BPC->GetSoundManager()));
    UKismetSystemLibrary::PrintString(this, UKismetSystemLibrary::GetDisplayName(AudioVolumeSlider_Master));

    AudioVolumeSlider_Master->SetValue(BPC->GetSoundManager()->MasterVolume);
    AudioVolumeSlider_BackGround->SetValue(BPC->GetSoundManager()->BGMVolume);
    AudioVolumeSlider_Effect->SetValue(BPC->GetSoundManager()->EffectVolume);
    AudioVolumeSlider_UI->SetValue(BPC->GetSoundManager()->OriginalUIVolume);

    TEXT_Master->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->MasterVolume * 100.f)));
    TEXT_BGM->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->BGMVolume * 100.f)));
    TEXT_Effect->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->EffectVolume * 100.f)));
    TEXT_UI->SetText(FText::AsNumber(FMath::RoundToInt(BPC->GetSoundManager()->OriginalUIVolume * 100.f)));

    GetWorld()->GetTimerManager().ClearTimer(SoundInitTH);
}
