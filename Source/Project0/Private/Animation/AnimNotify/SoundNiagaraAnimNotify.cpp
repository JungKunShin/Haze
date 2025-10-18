// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/SoundNiagaraAnimNotify.h"
//#include "NiagaraFunctionLibrary.h"
//#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
//#include "Sound/SoundAttenuation.h"
//#include "NiagaraComponent.h"
#include "Manager/SoundManager.h"
#include "PlayerControllers/WorldPlayerController.h"
//#include "UObject/WeakObjectPtr.h"
//#include "UObject/WeakObjectPtrTemplates.h"

static float GetVolumeValue(APlayerController*_PC,EVolumeType VolumeType)
{
	
	if (!_PC) return 1.0f;

	auto* SM = Cast<AWorldPlayerController>(_PC)->GetSoundManager();
	if (!SM) return 1.0f;

	switch (VolumeType)
	{
	case EVolumeType::Master: return SM->MasterVolume;
	case EVolumeType::BGM:    return SM->BGMVolume;
	case EVolumeType::Effect: return SM->EffectVolume;
	case EVolumeType::UI:     return SM->UIVolume;
	default: return 1.0f;
	}
}

void USoundNiagaraAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp) return;

	AActor* Owner = MeshComp->GetOwner();
	if (!Owner) return;

	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);

	if (Sound)
	{
		float Volume = GetVolumeValue(PC,VolumeType);

		UGameplayStatics::PlaySoundAtLocation(Owner,Sound,Owner->GetActorLocation(),Volume,	1.0f,0.0f,AttenuationSetting);
	}
}
