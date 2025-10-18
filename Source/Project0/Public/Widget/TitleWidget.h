// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TitleWidget.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UTitleWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

protected:

	virtual void NativeConstruct() override;

	void SetOptionSwticherIndex(int32 _CurSwitchNum);

	void SetDetailSettingSwitcherIndex(int32 _CurSwitchNum);

	void WidgetTitleButtonSoundInit();

	void HoverButtonSoundInit();

	UPROPERTY(meta=(BindWidget))
	class UButton* Button_GameStart;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_LoadGame;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Settings;
	UPROPERTY(meta=(BindWidget))
	class UButton* Button_Quit;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_TitleBack;

	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Display;
	UPROPERTY(meta = (BindWidget))
	class UButton* Button_Volume;

	
	UPROPERTY(meta=(BindWiget))
	class UTextBlock* Text_GameStart;
	UPROPERTY(meta=(BindWiget))
	class UTextBlock* Text_LoadGame;
	UPROPERTY(meta=(BindWiget))
	class UTextBlock* Text_Settings;
	UPROPERTY(meta=(BindWiget))
	class UTextBlock* Text_Quit;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_TitleBack;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Display;
	
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Text_Volume;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* OptionWidgetSwitcher;

	UPROPERTY(meta = (BindWidget))
	class UWidgetSwitcher* DetailSettingWidgetSwitcher;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* ButtonSound;

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	USoundBase* HoverButtonSound;


private:
	UPROPERTY(EditDefaultsOnly)
	FString StageSaveSlotString = TEXT("StageSaveSlot");

	//버튼->텍스트 저장
	TMap<TObjectPtr<class UButton>, TObjectPtr<class UTextBlock>> ButtonToText;

	//기본 글자 크기 저장
	TMap<TObjectPtr<class UTextBlock>, int32> BaseFontSizes;

	UFUNCTION()
	void OnStartClicked();

	UFUNCTION()
	void OnLoadGameClicked();

	UFUNCTION()
	void OnQuitClicked();

	UFUNCTION()
	void OnButtonHovered();

	UFUNCTION()
	void OnButtonUnhovered();

	UFUNCTION()
	void OnTitleBackClicked();

	UFUNCTION()
	void OnDisplayClicked();

	UFUNCTION()
	void OnVolumeClicked();

	UFUNCTION()
	void OnDetailOptionClicked();
	
	void RegisterButton(class UButton* Button, class UTextBlock* Text);
	void ApplyNoHoverBackground(class UButton* Button) const;
	void GrowText(class UTextBlock* Text, float Scale = 1.10f);
	void RestoreText(class UTextBlock* Text);
	void SavedBaseSize(class UTextBlock* Text);
	void RestoreAllTexts();
};
