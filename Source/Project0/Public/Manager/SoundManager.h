// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Sound/SoundBase.h"
#include "Components/AudioComponent.h"
#include "SoundManager.generated.h"

UCLASS()
class PROJECT0_API ASoundManager : public AActor
{
	GENERATED_BODY()
	
public:	
	ASoundManager();

protected:
	virtual void BeginPlay() override;

public:
    UFUNCTION(BlueprintCallable, Category = "Sound")
    void InitSoundManager();

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void PlayBGM(USoundBase* BGM);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void StopBGM();

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetMasterVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetBGMVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetEffectVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    void SetUIVolume(float Volume);

    UFUNCTION(BlueprintCallable, Category = "Sound")
    float GetEffectVolume();

public:
    UPROPERTY()
    USoundBase* CurBGM;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USoundBase* BGM_Title;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USoundBase* BGM_World;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USoundBase* BGM_Tutorial;

    UPROPERTY(BlueprintReadWrite, EditAnywhere)
    USoundBase* BGM_EndingCredit;

    UPROPERTY()
    float MasterVolume = 1.0f; //¸¶½ºÅÍ º¼·ý

    UPROPERTY()
    float BGMVolume = 1.0f; //¹è°æÀ½¾Ç º¼·ý

    UPROPERTY()
    float EffectVolume = 0.5f; //È¿°úÀ½ º¼·ý

    UPROPERTY()
    float UIVolume = 0.5f; //UI º¼·ý

    UPROPERTY()
    float OriginalUIVolume = 0.5f; //¿ø·¡ UI º¼·ý


    UPROPERTY()
    UAudioComponent* BGMComponent;

    UPROPERTY()
    TArray<UAudioComponent*> ActiveEffectComponents;

};
