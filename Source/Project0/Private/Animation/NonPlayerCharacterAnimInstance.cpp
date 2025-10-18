// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NonPlayerCharacterAnimInstance.h"
#include "Characters/NonPlayerCharacters/NonPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UNonPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ANonPlayerCharacter>(GetOwningActor());		//NonPlayerCharacter로 형변환
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();			//Owner의 Movement저장
	}
}

void UNonPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		OwnerSpeed = UKismetMathLibrary::VSize(Movement->Velocity); //OwnerSpeed는 Movement의 속도 가져오기
		OnFalling = Movement->IsFalling();							//Movement의 isfalling값 저장
	}
}
