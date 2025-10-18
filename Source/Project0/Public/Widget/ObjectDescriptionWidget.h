// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ObjectDescriptionWidget.generated.h"

/**
 * 
 */
class UTextBlock;
class UWidget;
class UWidgetAnimation;

UCLASS()
class PROJECT0_API UObjectDescriptionWidget : public UUserWidget
{
	GENERATED_BODY()
	
	public:
    UFUNCTION(BlueprintCallable)
    void SetTexts(const FText& InTitle, const FText& InBody);

    UFUNCTION(BlueprintCallable)
    void SetIntroTexts(const FText& InTitle, const FText& InDesc, const FText& InHint);

    UFUNCTION(BlueprintCallable) 
    void SetInspectHUDVisible(bool bVisible);

    UFUNCTION(BlueprintCallable) 
    void SetObjectPanelVisible(bool bVisible);



  

 protected:
    virtual void NativeOnInitialized() override;

    //힌트 패널
    UPROPERTY(meta = (BindWidget))           
    UWidget* InspectHUDPanel;   

    //본문 표기 패널
    UPROPERTY(meta = (BindWidget))           
    UWidget* ObjectCanvasPanel;  

    //제목
    UPROPERTY(meta = (BindWidget))  
    UTextBlock* ObjectTitle;     

    //오브젝트 본문(노트에 써있는 내용)
    UPROPERTY(meta = (BindWidget))   
    UTextBlock* ObjectText;       

    //오브젝트 설명
    UPROPERTY(meta = (BindWidget))   
    UTextBlock*  ObjectDescription; 

    UPROPERTY(meta = (BindWidget))   
    UTextBlock* HintText;        

    //애니메이션 바인딩(안씀)
    UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
    UWidgetAnimation* PickUpObject = nullptr;
	
};
