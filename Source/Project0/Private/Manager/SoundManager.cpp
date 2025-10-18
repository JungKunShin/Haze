// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/SoundManager.h"

// Sets default values
ASoundManager::ASoundManager()
{
    BGMComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("BGMComponent"));

    BGMComponent->bIsUISound = true;
    BGMComponent->bAutoDestroy = false;
}

// Called when the game starts or when spawned
void ASoundManager::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASoundManager::InitSoundManager()
{
    SetMasterVolume(MasterVolume);
}

void ASoundManager::PlayBGM(USoundBase* BGM)
{
    if (BGMComponent && BGM)
    {
        BGMComponent->SetSound(BGM);
        BGMComponent->Play();
        CurBGM = BGM;
    }
}

void ASoundManager::StopBGM()
{
    if (BGMComponent)
    {
        BGMComponent->Stop();
    }
}

void ASoundManager::SetMasterVolume(float Volume)
{
    MasterVolume = Volume;
    SetBGMVolume(BGMVolume);
    SetEffectVolume(EffectVolume);
    SetUIVolume(OriginalUIVolume);
}

void ASoundManager::SetBGMVolume(float Volume)
{
    if (BGMComponent)
    {
        BGMVolume = Volume;
        float ResultVolume = (MasterVolume * BGMVolume);
        BGMComponent->SetVolumeMultiplier(FMath::Clamp(ResultVolume, 0.001f, 1.0f));
    }
}

void ASoundManager::SetEffectVolume(float Volume)
{
    EffectVolume = Volume;
    for (auto Comp : ActiveEffectComponents)
    {
        float ResultVolume = (MasterVolume * EffectVolume);
        Comp->SetVolumeMultiplier(FMath::Clamp(ResultVolume, 0.001f, 1.0f));
    }

}

void ASoundManager::SetUIVolume(float Volume)
{
    OriginalUIVolume = Volume;
    UIVolume = OriginalUIVolume* MasterVolume;
}

float ASoundManager::GetEffectVolume()
{
    return EffectVolume;
}

