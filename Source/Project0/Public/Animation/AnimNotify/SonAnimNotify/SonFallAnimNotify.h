// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotify/BaseAnimNotify.h"
#include "SonFallAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API USonFallAnimNotify : public UBaseAnimNotify
{
	GENERATED_BODY()
	
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
};
