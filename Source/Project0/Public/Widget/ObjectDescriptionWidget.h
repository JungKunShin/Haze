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

    //��Ʈ �г�
    UPROPERTY(meta = (BindWidget))           
    UWidget* InspectHUDPanel;   

    //���� ǥ�� �г�
    UPROPERTY(meta = (BindWidget))           
    UWidget* ObjectCanvasPanel;  

    //����
    UPROPERTY(meta = (BindWidget))  
    UTextBlock* ObjectTitle;     

    //������Ʈ ����(��Ʈ�� ���ִ� ����)
    UPROPERTY(meta = (BindWidget))   
    UTextBlock* ObjectText;       

    //������Ʈ ����
    UPROPERTY(meta = (BindWidget))   
    UTextBlock*  ObjectDescription; 

    UPROPERTY(meta = (BindWidget))   
    UTextBlock* HintText;        

    //�ִϸ��̼� ���ε�(�Ⱦ�)
    UPROPERTY(Transient, meta = (BindWidgetAnim), BlueprintReadOnly)
    UWidgetAnimation* PickUpObject = nullptr;
	
};
