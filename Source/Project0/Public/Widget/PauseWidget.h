// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseWidget.generated.h"

class UButton; 
class UTextBlock;

/**
 * 
 */
UCLASS()
class PROJECT0_API UPauseWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
    virtual void NativeConstruct() override;



    UPROPERTY(meta = (BindWidget)) 
    UButton* Button_Continue;

    UPROPERTY(meta = (BindWidget)) 
    UTextBlock* Text_Continue;

    UPROPERTY(meta = (BindWidget)) 
    UButton* Button_SaveGame;

    UPROPERTY(meta = (BindWidget)) 
    UTextBlock* Text_SaveGame;

    UPROPERTY(meta = (BindWidget)) 
    UButton* Button_PauseSettings;

    UPROPERTY(meta = (BindWidget)) 
    UTextBlock* Text_PauseSettings;

    UPROPERTY(meta = (BindWidget)) 
    UButton* Button_PauseQuit;

    UPROPERTY(meta = (BindWidget)) 
    UTextBlock* Text_PauseQuit;
	
private:

    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* OptionWidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    class UWidgetSwitcher* DetailSettingWidgetSwitcher;

    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Display;
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_Volume;
    UPROPERTY(meta = (BindWidget))
    class UButton* Button_TitleBack;


    void RegisterButton(UButton* Button, UTextBlock* Text);
    void ApplyNoHoverBackground(UButton* Button) const;
    void GrowText(UTextBlock* Text, float Scale);
    void RestoreText(UTextBlock* Text);
    void RestoreAllTexts();
    void SaveBaseSize(UTextBlock* Text);

    UFUNCTION() 
    void OnButtonHovered();

    UFUNCTION() 
    void OnButtonUnhovered();

    //클릭 핸들러
    UFUNCTION() 
    void OnContinueClicked();

    UFUNCTION()
    void OnSaveClicked();

    UFUNCTION() 
    void OnSettingsClicked();

    UFUNCTION() 
    void OnQuitClicked();

    void WidgetPauseButtonSoundInit();

    UFUNCTION() void OnDisplayClicked();      
    UFUNCTION() void OnVolumeClicked();       
    UFUNCTION() void OnTitleBackClicked();

    TMap<UButton*, UTextBlock*> ButtonToText;
    TMap<UTextBlock*, int32>    BaseFontSizes;

    UPROPERTY(EditDefaultsOnly, Category = "Sound")
    USoundBase* ButtonSound;
};
