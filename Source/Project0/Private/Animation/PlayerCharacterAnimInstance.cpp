// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/PlayerCharacterAnimInstance.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"


void UPlayerCharacterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<APlayerCharacter>(GetOwningActor());		//PlayerCharacter로 형변환
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();			//Owner의 Movement저장
	}
}

void UPlayerCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (Movement)
	{
		const FVector Velocity = Movement->Velocity;

		//전체 속도 크기
		OwnerSpeed = Velocity.Size();

		//crouch 팝인현상 방지
		const FVector V = Movement->Velocity;
		const FVector Planar(V.X, V.Y, 0.f);
		const FVector Fwd2D = Owner->GetActorForwardVector().GetSafeNormal2D();
		const FVector Right2D = Owner->GetActorRightVector().GetSafeNormal2D();

		const float Max = Movement->MaxWalkSpeedCrouched;
		float Fwd = FVector::DotProduct(Planar, Fwd2D) / FMath::Max(1.f, Max);
		float Side = FVector::DotProduct(Planar, Right2D) / FMath::Max(1.f, Max);

		Fwd = FMath::Clamp(Fwd, -1.f, 1.f);
		Side = FMath::Clamp(Side, -1.f, 1.f);
		if (FMath::Abs(Fwd) < 0.03f) Fwd = 0.f;
		if (FMath::Abs(Side) < 0.03f) Side = 0.f;

		ForwardSpeed = Fwd;
		RightSpeed = Side;
	
		bIsCrouched = Owner->GetCharacterMovement()->IsCrouching();
		OnFalling = Movement->IsFalling();							//Movement의 isfalling값 저장
	}
}
