// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/TitleWidget.h"
#include "GameInstances/StageGameInstance.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Styling/SlateBrush.h"
#include "Components/WidgetSwitcher.h"
#include "PlayerControllers/BasePlayerController.h"
#include "Manager/SoundManager.h"

void UTitleWidget::NativeConstruct()
{
    Super::NativeConstruct();

    //버튼 등록
    RegisterButton(Button_GameStart, Text_GameStart);
    RegisterButton(Button_LoadGame, Text_LoadGame);
    RegisterButton(Button_Settings, Text_Settings);
    RegisterButton(Button_Quit, Text_Quit);
    RegisterButton(Button_TitleBack, Text_TitleBack);
    RegisterButton(Button_Display, Text_Display);
    RegisterButton(Button_Volume, Text_Volume);

    if (Button_GameStart)
    {
        Button_GameStart->OnClicked.AddDynamic(this, &UTitleWidget::OnStartClicked);
    }

    if (Button_LoadGame)
    {
        Button_LoadGame->OnClicked.AddDynamic(this, &UTitleWidget::OnLoadGameClicked);
    }

    if (Button_Quit)
    {
        Button_Quit->OnClicked.AddDynamic(this, &UTitleWidget::OnQuitClicked);
    }

    if (Button_TitleBack)
    {
        Button_TitleBack->OnClicked.AddDynamic(this, &UTitleWidget::OnTitleBackClicked);
    }
    if (Button_Display)
    {
        Button_Display->OnClicked.AddDynamic(this, &UTitleWidget::OnDisplayClicked);
    }
    if (Button_Volume)
    {
        Button_Volume->OnClicked.AddDynamic(this, &UTitleWidget::OnVolumeClicked);
    }
    if (Button_Settings)
    {
        Button_Settings->OnClicked.AddDynamic(this, &UTitleWidget::OnDetailOptionClicked);
    }
}

void UTitleWidget::SetOptionSwticherIndex(int32 _CurSwitchNum)
{
    OptionWidgetSwitcher->SetActiveWidgetIndex(_CurSwitchNum);
}

void UTitleWidget::SetDetailSettingSwitcherIndex(int32 _CurSwitchNum)
{
    DetailSettingWidgetSwitcher->SetActiveWidgetIndex(_CurSwitchNum);
}

void UTitleWidget::WidgetTitleButtonSoundInit()
{
    if (ButtonSound)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);

        UGameplayStatics::PlaySound2D(this,ButtonSound,BPC->GetSoundManager()->UIVolume,1.0f);
    }
}

void UTitleWidget::HoverButtonSoundInit()
{
    if (HoverButtonSound)
    {
        APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
        ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);

        UGameplayStatics::PlaySound2D(this, HoverButtonSound, BPC->GetSoundManager()->UIVolume, 1.0f);
    }
}

void UTitleWidget::OnStartClicked()
{
    WidgetTitleButtonSoundInit();

    UStageGameInstance* StageGameInstance = GetGameInstance<UStageGameInstance>();
    StageGameInstance->bNeedToLoadGame = false;
    StageGameInstance->bPlayOpeningSequence = true;

    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/Level/HorrorMap.umap")));
}

void UTitleWidget::OnLoadGameClicked()
{
    WidgetTitleButtonSoundInit();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->SaveSoundSettings(); // 음량 정보 저장해서 월드레벨로 이동

    UStageGameInstance* StageGameInstance = GetGameInstance<UStageGameInstance>();
    if (UGameplayStatics::DoesSaveGameExist(StageSaveSlotString, 0)) {
        StageGameInstance->bNeedToLoadGame = true;
    }
    else {
        StageGameInstance->bNeedToLoadGame = false;
    }

    StageGameInstance->bPlayOpeningSequence = false;
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/Level/HorrorMap.umap")));

}

void UTitleWidget::OnQuitClicked()
{
    WidgetTitleButtonSoundInit();
    if (APlayerController* PC = GetOwningPlayer())
    {
        UKismetSystemLibrary::QuitGame(this, PC, EQuitPreference::Quit, false);
    }
}

void UTitleWidget::OnButtonHovered()
{
    //중복 없게 텍스트 복구
    RestoreAllTexts();

    //현재 Hover된 버튼 찾아 해당 텍스트를 키움
    for (const TPair<UButton*, UTextBlock*>& Pair : ButtonToText)
    {
        if (UButton* Button = Pair.Key)
        {
            if (Button->IsHovered())
            {
                GrowText(Pair.Value, 1.10f);
                break;
            }
        }
    }

    HoverButtonSoundInit();

}

void UTitleWidget::OnButtonUnhovered()
{
    RestoreAllTexts();
}

void UTitleWidget::OnTitleBackClicked()
{
    WidgetTitleButtonSoundInit();
    SetDetailSettingSwitcherIndex(0);
    SetOptionSwticherIndex(0);

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    ABasePlayerController* BPC = Cast< ABasePlayerController>(PC);
    BPC->SaveSoundSettings(); // 음량 정보 저장
}

void UTitleWidget::OnDisplayClicked()
{
    WidgetTitleButtonSoundInit();
    SetDetailSettingSwitcherIndex(0);
}

void UTitleWidget::OnVolumeClicked()
{
    WidgetTitleButtonSoundInit();
    SetDetailSettingSwitcherIndex(1);
}

void UTitleWidget::OnDetailOptionClicked()
{
    WidgetTitleButtonSoundInit();
    SetOptionSwticherIndex(1);
}

void UTitleWidget::RegisterButton(UButton* Button, UTextBlock* Text)
{
    if (!Button) return;

    //기본 회색 배경 제거
    ApplyNoHoverBackground(Button);

    Button->OnHovered.AddDynamic(this, &UTitleWidget::OnButtonHovered);
    Button->OnUnhovered.AddDynamic(this, &UTitleWidget::OnButtonUnhovered);


    //버튼 매핑& 기본 폰트 사이즈 저장
    if (Text)
    {
        ButtonToText.Add(Button, Text);
        SavedBaseSize(Text);
    }
}

void UTitleWidget::ApplyNoHoverBackground(UButton* Button) const
{
    if (!Button) return;
    FButtonStyle Style = Button->GetStyle();

    //배경 제거
    FSlateBrush None;
    None.DrawAs = ESlateBrushDrawType::NoDrawType;
    Style.Hovered = None;

    Button->SetStyle(Style);
}

void UTitleWidget::GrowText(UTextBlock* Text, float Scale)
{
    if (!Text) return;
    const int32* FoundBase = BaseFontSizes.Find(Text);
    const int32 Base = FoundBase ? *FoundBase : Text->GetFont().Size;

    // 폰트 사이즈를 가져와 사이즈를 키움
    FSlateFontInfo NewFont = Text->GetFont();
    NewFont.Size = FMath::RoundToInt(float(Base) * Scale);
    Text->SetFont(NewFont);
}

void UTitleWidget::RestoreText(UTextBlock* Text)
{
    if (!Text) return;
    const int32* FoundBase = BaseFontSizes.Find(Text);
    if (!FoundBase) return;

   //폰트 사이즈를 가져와 기본 폰트 지정
    FSlateFontInfo NewFont = Text->GetFont();
    NewFont.Size = *FoundBase;
    Text->SetFont(NewFont);
}

void UTitleWidget::SavedBaseSize(UTextBlock* Text)
{
    if (!Text) return;
    const FSlateFontInfo Current = Text->GetFont();
    BaseFontSizes.FindOrAdd(Text) = Current.Size;
}

void UTitleWidget::RestoreAllTexts()
{
    for (const TPair<UButton*, UTextBlock*>& Pair : ButtonToText)
    {
        if (UTextBlock* Text = Pair.Value)
        {
            RestoreText(Text);
        }
    }
}
