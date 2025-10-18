// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/NonPlayerCharacterAnimInstance.h"
#include "Characters/NonPlayerCharacters/NonPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UNonPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ANonPlayerCharacter>(GetOwningActor());		//NonPlayerCharacter�� ����ȯ
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();			//Owner�� Movement����
	}
}

void UNonPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		OwnerSpeed = UKismetMathLibrary::VSize(Movement->Velocity); //OwnerSpeed�� Movement�� �ӵ� ��������
		OnFalling = Movement->IsFalling();							//Movement�� isfalling�� ����
	}
}
