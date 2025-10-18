// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/PauseWidget.h"
#include "HUD/WorldHUD.h"
#include "GameInstances/StageGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Manager/SoundManager.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Styling/SlateBrush.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerControllers/BasePlayerController.h"


void UPauseWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //버튼 등록
    RegisterButton(Button_Continue, Text_Continue);
    RegisterButton(Button_SaveGame, Text_SaveGame);
    RegisterButton(Button_PauseSettings, Text_PauseSettings);
    RegisterButton(Button_PauseQuit, Text_PauseQuit);

    if (Button_Continue)     Button_Continue->OnClicked.AddDynamic(this, &UPauseWidget::OnContinueClicked);
    if (Button_SaveGame)     Button_SaveGame->OnClicked.AddDynamic(this, &UPauseWidget::OnSaveClicked);
    if (Button_PauseSettings)Button_PauseSettings->OnClicked.AddDynamic(this, &UPauseWidget::OnSettingsClicked);
    if (Button_PauseQuit)    Button_PauseQuit->OnClicked.AddDynamic(this, &UPauseWidget::OnQuitClicked);

    if (Button_Display)        Button_Display->OnClicked.AddDynamic(this, &UPauseWidget::OnDisplayClicked);
    if (Button_Volume)         Button_Volume->OnClicked.AddDynamic(this, &UPauseWidget::OnVolumeClicked);
    if (Button_TitleBack)      Button_TitleBack->OnClicked.AddDynamic(this, &UPauseWidget::OnTitleBackClicked);

    if (OptionWidgetSwitcher)  OptionWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPauseWidget::OnDisplayClicked()
{
    if (DetailSettingWidgetSwitcher) DetailSettingWidgetSwitcher->SetActiveWidgetIndex(0);
}

void UPauseWidget::OnVolumeClicked()
{
    if (DetailSettingWidgetSwitcher) DetailSettingWidgetSwitcher->SetActiveWidgetIndex(1);
}

void UPauseWidget::OnTitleBackClicked()
{
    if (DetailSettingWidgetSwitcher) DetailSettingWidgetSwitcher->SetActiveWidgetIndex(0);
    if (OptionWidgetSwitcher)        OptionWidgetSwitcher->SetActiveWidgetIndex(0);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->SaveSoundSettings(); // 음량 정보 저장
}

//TitleWidget과 동일한 스타일 함수
void UPauseWidget::RegisterButton(UButton* Button, UTextBlock* Text)
{
    if (!Button) return;
    ApplyNoHoverBackground(Button);
    Button->OnHovered.AddDynamic(this, &UPauseWidget::OnButtonHovered);
    Button->OnUnhovered.AddDynamic(this, &UPauseWidget::OnButtonUnhovered);
    if (Text) { ButtonToText.Add(Button, Text); SaveBaseSize(Text); }
}

void UPauseWidget::ApplyNoHoverBackground(UButton* Button) const
{
    if (!Button) return;
    FButtonStyle Style = Button->GetStyle();
    FSlateBrush None; None.DrawAs = ESlateBrushDrawType::NoDrawType;
    Style.Hovered = None;
    Button->SetStyle(Style);
}

void UPauseWidget::SaveBaseSize(UTextBlock* Text)
{
    if (!Text) return;
    BaseFontSizes.FindOrAdd(Text) = Text->GetFont().Size;
}

void UPauseWidget::GrowText(UTextBlock* Text, float Scale)
{
    if (!Text) return;
    const int32* FoundBase = BaseFontSizes.Find(Text);
    const int32  Base = FoundBase ? *FoundBase : Text->GetFont().Size;
    FSlateFontInfo NewFont = Text->GetFont();
    NewFont.Size = FMath::RoundToInt(float(Base) * Scale);
    Text->SetFont(NewFont);
}

void UPauseWidget::RestoreText(UTextBlock* Text)
{
    if (!Text) return;
    if (const int32* Base = BaseFontSizes.Find(Text))
    {
        FSlateFontInfo NewFont = Text->GetFont();
        NewFont.Size = *Base;
        Text->SetFont(NewFont);
    }
}

void UPauseWidget::RestoreAllTexts()
{
    for (const auto& P : ButtonToText) if (P.Value) RestoreText(P.Value);
}

void UPauseWidget::OnButtonHovered()
{
    RestoreAllTexts();
    for (const auto& P : ButtonToText)
        if (P.Key && P.Key->IsHovered()) { GrowText(P.Value, 1.10f); break; }
}
void UPauseWidget::OnButtonUnhovered() { RestoreAllTexts(); }

//클릭 동작
void UPauseWidget::OnContinueClicked()
{
    WidgetPauseButtonSoundInit();
    // 게임 재개 + 위젯 닫기
    if (APlayerController* PC = GetOwningPlayer())
        if (AWorldHUD* HUD = PC->GetHUD<AWorldHUD>())
            HUD->SetPauseMenuVisible(false);
}

void UPauseWidget::OnSaveClicked()
{
    WidgetPauseButtonSoundInit();
    UStageGameInstance* StageGM = GetGameInstance<UStageGameInstance>();
    if (StageGM) StageGM->TrySaveGame();
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("SaveGame"));
}

void UPauseWidget::OnSettingsClicked()
{
    if (OptionWidgetSwitcher)        OptionWidgetSwitcher->SetActiveWidgetIndex(1);
    if (DetailSettingWidgetSwitcher) DetailSettingWidgetSwitcher->SetActiveWidgetIndex(1);

    WidgetPauseButtonSoundInit();
    GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, TEXT("Settings"));
}

void UPauseWidget::OnQuitClicked()
{
    WidgetPauseButtonSoundInit();
    if (APlayerController* PC = GetOwningPlayer())
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
}

void UPauseWidget::WidgetPauseButtonSoundInit()
{
    if (ButtonSound)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        AWorldPlayerController* WPC = Cast< AWorldPlayerController>(PC);

        UGameplayStatics::PlaySound2D(this, ButtonSound, WPC->GetSoundManager()->UIVolume, 1.0f);
    }
}
