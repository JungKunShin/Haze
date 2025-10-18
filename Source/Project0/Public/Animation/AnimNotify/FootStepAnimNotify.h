// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotify/BaseAnimNotify.h"
#include "FootStepAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UFootStepAnimNotify : public UBaseAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundBase* HallSound;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
	USoundAttenuation* AttenuationSetting;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	bool IsLeft=true;
};
