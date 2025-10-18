// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotify/BaseAnimNotify.h"
#include "GFAttackAnimNotify.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UGFAttackAnimNotify : public UBaseAnimNotify
{
	GENERATED_BODY()

private:
	const float CIRCLE_RADIUS = 750.0f;
public:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
};
