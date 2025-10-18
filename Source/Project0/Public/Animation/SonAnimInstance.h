// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/PlayerCharacterAnimInstance.h"
#include "SonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API USonAnimInstance : public UPlayerCharacterAnimInstance
{
	GENERATED_BODY()
	
	
public:
	virtual void NativeInitializeAnimation()override;

	virtual void NativeUpdateAnimation(float DeltaSeconds)override;

};
