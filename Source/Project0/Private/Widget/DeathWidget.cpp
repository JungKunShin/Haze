// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/DeathWidget.h"
#include "Components/Button.h"
#include "GameInstances/StageGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "Manager/SoundManager.h"
#include "Characters/NonPlayerCharacters/EnemyCharacter.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"

void UDeathWidget::NativeConstruct()
{
	Super::NativeConstruct();

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AWorldPlayerController* WPC = Cast<AWorldPlayerController>(PC);
	WPC->GetSoundManager()->StopBGM();

	PlayAnimation(Gradation);

	GetWorld()->GetTimerManager().SetTimer(ResetTH, this, &UDeathWidget::ResetReBirthLocation, 3.f, false);
}

void UDeathWidget::ResetReBirthLocation()
{

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	AWorldPlayerController* WPC = Cast<AWorldPlayerController>(PC);
	AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());

	if (!bShouldResetThisWiget) {
		WPC->GetSoundManager()->PlayBGM(WPC->GetSoundManager()->BGM_World); // 원하는 BGM 지정
		return;
	}

	TArray<AActor*> EnemyCh;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Chasing"), EnemyCh);

	for (AActor* MyActor : EnemyCh)
	{
		AGrandFatherCharacter* GrandFather = Cast<AGrandFatherCharacter>(MyActor);
		if (GrandFather)
		{
			GrandFather->ResetEnemy();
			break;
		}
	}

	UStageGameInstance*StageGameInstance= GetGameInstance<UStageGameInstance>();

	StageGameInstance->ResetStage();

	WPC->GetSoundManager()->PlayBGM(WPC->GetSoundManager()->BGM_World); // 원하는 BGM 지정

	WH->SetDeathWidgetVisible(false);
}
