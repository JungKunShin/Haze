// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "NonPlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UNonPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)

	class ANonPlayerCharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UCharacterMovementComponent* Movement;

	//캐릭터 걷기 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float OwnerSpeed;

	//캐릭터 공중 변수
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	bool OnFalling;
};
