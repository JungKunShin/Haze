// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SonAnimInstance.h"
#include "Characters/PlayerCharacters/SonCharacter.h"

void USonAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ASonCharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
}
