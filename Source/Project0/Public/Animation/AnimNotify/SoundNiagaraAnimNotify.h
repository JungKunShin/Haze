// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotify/BaseAnimNotify.h"
#include "SoundNiagaraAnimNotify.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EVolumeType : uint8
{
	Master     UMETA(DisplayName = "Master"),
	BGM        UMETA(DisplayName = "BGM"),
	Effect     UMETA(DisplayName = "Effect"),
	UI         UMETA(DisplayName = "UI")
};

UCLASS()
class PROJECT0_API USoundNiagaraAnimNotify : public UBaseAnimNotify
{
	GENERATED_BODY()

public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;

	//// ����Ʈ �ý���
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//class UNiagaraSystem* NiagaraEffect;

	// ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* Sound;

	// ���� ���� ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* AttenuationSetting;

	// ���� �̸�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	FName SocketName = "head";

	//// ����Ʈ ������
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect")
	//FVector EffectScale = FVector(1.0f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	EVolumeType VolumeType = EVolumeType::Effect;

	//float GetVolumeValue(EVolumeType VolumeType);

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Effect", meta = (ClampMin = "0.0", ClampMax = "10.0"))
	//float NiagaraDestroyDelay = 3.0f;
	
};
