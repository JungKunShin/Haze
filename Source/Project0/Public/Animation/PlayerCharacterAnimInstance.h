// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API UPlayerCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	
	class APlayerCharacter* Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	class UCharacterMovementComponent* Movement;

	//ĳ���� �ȱ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Character)
	float OwnerSpeed;


	//ĳ���� �յ� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float ForwardSpeed = 0.f;

	//ĳ���� �¿� �̵� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float RightSpeed = 0.f;

	//ĳ���� ���� ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	bool OnFalling;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	bool bIsCrouched = false;

	//crouh ����
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	float Direction = 0.0f;

};
