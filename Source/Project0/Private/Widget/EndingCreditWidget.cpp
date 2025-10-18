// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/EndingCreditWidget.h"
#include "Components/ScrollBox.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Manager/SoundManager.h"

void UEndingCreditWidget::NativeConstruct()
{
    Super::NativeConstruct();

    APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
    AWorldPlayerController* WPC = Cast<AWorldPlayerController>(PC);

    WPC->GetSoundManager()->PlayBGM(WPC->GetSoundManager()->BGM_EndingCredit); // ���ϴ� BGM ����
}

void UEndingCreditWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    WaitTime += InDeltaTime;
    if (WaitTime < DOWN_START) return;

    if (CreditsScrollBox)
    {
        // ������ ���� (�Ʒ����� ���� ��ũ��)
        CurrentOffset += ScrollSpeed * InDeltaTime;

        // ��ũ�� ����
        CreditsScrollBox->SetScrollOffset(CurrentOffset);
    }

    if (CurrentOffset >= CreditsScrollBox->GetScrollOffsetOfEnd())
    {
        if (!IsFinished)
        {
            IsFinished = true;
            GetWorld()->GetTimerManager().SetTimer(LevelMoveTH, this, &UEndingCreditWidget::DelayTitleLevel, 1.4f, false);
        }
        // ������ ����
    }
}

void UEndingCreditWidget::DelayTitleLevel()
{
    UGameplayStatics::OpenLevel(GetWorld(), FName(TEXT("/Game/Level/Title.umap")));
}
