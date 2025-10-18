// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUD/BaseHUD.h"
#include "WorldHUD.generated.h"

/**
 * 
 */
class UQuestSystemComponent;
class ABaseQuest;
class AEnemyCharacter;
class UObjectDescriptionWidget;
class AMixGasStove;
class UConversationWidget;

UCLASS()
class PROJECT0_API AWorldHUD : public ABaseHUD
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay()override;
	
	//--------------´ëÈ­ °ü·Ã À§Á¬----------------------
	void SetConversationWidgetVisible(bool bOnShow);

	void SetConversationText(ABaseQuest* Quest);

	void SetSkipText();

	

	//--------------»ç¸Á °ü·Ã À§Á¬----------------------
	UFUNCTION(BlueprintCallable)
	void SetDeathWidgetVisible(bool bOnShow);

	//--------------¸íÈ­ÆÛÁñ °ü·Ã À§Á¬----------------------
	void SetPicturePuzzleVisible(bool bOnShow);

	//--------------¾àÁ¶ÇÕÆÛÁñ °ü·Ã À§Á¬----------------------
	void SetMedicineMixVisible(bool bOnShow);
	void SetMedicineCorrectAnswer(AMixGasStove* _CorrectAnswer);

	//--------------Á¶ÀÛÅ° °ü·Ã À§Á¬----------------------
	void SetInstructionVisible(bool bOnShow);
	void SetInstructionKeep();
	void SetInstructionEnd();
	void SetPutDownVisible(bool bOnShow);

	void OpenObjectWidget(const FText& Title, const FText& Body, const FText& Desc, const FText& Hint, bool bStartInReading);
	
	UFUNCTION(BlueprintCallable)
	void CloseObjectWidget();

	UFUNCTION(BlueprintPure)
	bool IsObjectWidgetOpen() const;
	
	void ShowObjectReading(bool bReading);           // true=º»¹®ÆÐ³Î, false=ÈùÆ®ÆÐ³Î

	FORCEINLINE UObjectDescriptionWidget* GetNoteWidget() const { return ObjectWidget; }
	//--------------¾àÁ¶ÇÕÆÛÁñ ÈùÆ®Ã¥ °ü·Ã À§Á¬----------------------
	void SetHintBookVisible(bool bOnShow);

	//--------------¿£µùÅ©·¹µ÷ °ü·Ã À§Á¬----------------------
	void SetEndingCreditVisible(bool bOnShow);

	//--------------·Îµù °ü·Ã À§Á¬----------------------
	void SetLoadingVisible(bool bOnShow);

	//ÀÓ½Ã »èÁ¦ ¿ä¸Á
	void SetVolumeTest();

public:

	UPROPERTY(BlueprintReadWrite)
	UConversationWidget* ConversationWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ConversationWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UDeathWidget* DeathWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UPicturePuzzleWidget* PicturePuzzleWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PicturePuzzleWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UInstructionWidget* InstructionWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> InstructionWidgetClass;	
	
	UPROPERTY(BlueprintReadWrite)
	UObjectDescriptionWidget* ObjectWidget = nullptr;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> ObjectWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UPutDownWidget* PutDownWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> PutDownWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UMedicineMixWidget* MedicineMixWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> MedicineMixWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UHintBookWidget* HintBookWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> HintBookWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UEndingCreditWidget* EndingCreditWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> EndingCreditWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class ULoadingWidget* LoadingWidget;

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> LoadingWidgetClass;

	UPROPERTY(BlueprintReadWrite)
	class UVolumeWidget* VolumeTest;//»èÁ¦¿ä¸Á

	UPROPERTY(EditAnywhere, Category = "UI")
	TSubclassOf<UUserWidget> VolumeTestClass;//»èÁ¦¿ä¸Á

	//--------------Pause °ü·Ã À§Á¬----------------------

	UPROPERTY(EditDefaultsOnly, Category = "UI") 
	TSubclassOf<class UPauseWidget> PauseWidgetClass;

	UPROPERTY() 
	class UPauseWidget* PauseWidget = nullptr;

	UFUNCTION(BlueprintCallable)
	void SetPauseMenuVisible(bool bOnShow);

	UFUNCTION(BlueprintPure)
	bool IsPauseMenuOpen() const;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConversationKey")
	TSubclassOf<UUserWidget> ConversationKeyWidgetClass;   

	UPROPERTY(VisibleInstanceOnly, Category = "ConversationKey")
	bool bConversationKeyAlreadyShown = false;            

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConversationKey", meta = (ClampMin = "0.1"))
	float ConversationKeyDuration = 5.f;                  

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ConversationKey")
	int32 ConversationKeyZOrder = 100;                   

	UFUNCTION(BlueprintCallable, Category = "ConversationKey")
	void ShowConversationKeyOnce(float InDuration = -1.f); // ÇÑ ¹ø¸¸ 5ÃÊ Ç¥½Ã

	UFUNCTION(BlueprintCallable, Category = "ConversationKey")
	void HideConversationKey();                          

private:
	FTimerHandle ConversationKeyTimer;
	UPROPERTY() UUserWidget* ConversationKeyWidget = nullptr;
	void SetConversationKeyVisible(bool bOnShow);

	FTimerHandle ObjectCloseTimer;

	bool Testbool=true;
};
