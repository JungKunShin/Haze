// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify/GrandFatherAnimNotify/GFAttackAnimNotify.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UGFAttackAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	if (!MeshComp)return;

	AGrandFatherCharacter* GrandPa = Cast< AGrandFatherCharacter>(MeshComp->GetOwner());
	if (!GrandPa)return;

	FVector Start = GrandPa->GetActorLocation();
	FVector End = Start;

	FHitResult HitResult;

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GrandPa);

	bool bHit = GetWorld()->SweepSingleByObjectType(HitResult, Start, End, FQuat::Identity, ObjectQueryParams, FCollisionShape::MakeSphere(CIRCLE_RADIUS),QueryParams);

	if (bHit)
	{
		AActor* HitActor = HitResult.GetActor();
		APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(HitActor);
		if (HitActor && PlayerCharacter)
		{
			FRotator LookAtRotA = UKismetMathLibrary::FindLookAtRotation(PlayerCharacter->GetActorLocation(), GrandPa->GetActorLocation());
			LookAtRotA.Pitch = 0.f;
			LookAtRotA.Roll = 0.f;
			FRotator LookAtRotB = UKismetMathLibrary::FindLookAtRotation(GrandPa->GetActorLocation(), PlayerCharacter->GetActorLocation());
			LookAtRotB.Pitch = 0.f;
			LookAtRotB.Roll = 0.f;
			
			PlayerCharacter->SetActorRotation(LookAtRotA);
			GrandPa->SetActorRotation(LookAtRotB);

			//FRotator CameraRot=PlayerCharacter->ThirdPersonCamera->GetRelativeRotation();
			//CameraRot.Pitch -= 15.f;
			//CameraRot.Normalize();
			//FRotator BoomRot = PlayerCharacter->CameraBoom->GetRelativeRotation();
			//BoomRot.Pitch -= 15.f;
			//BoomRot.Normalize();

			//PlayerCharacter->CameraBoom->SetUsingAbsoluteRotation(false);
			//PlayerCharacter->CameraBoom->bUsePawnControlRotation = false;
			//PlayerCharacter->ThirdPersonCamera->SetUsingAbsoluteRotation(false);
			//PlayerCharacter->ThirdPersonCamera->bUsePawnControlRotation = false;
			//PlayerCharacter->CameraBoom->SetRelativeRotation(BoomRot);
			//PlayerCharacter->ThirdPersonCamera->SetRelativeRotation(CameraRot);

			//PlayerCharacter->CameraBoom->SetUsingAbsoluteRotation(true);
			//PlayerCharacter->CameraBoom->bUsePawnControlRotation = true;
			//PlayerCharacter->ThirdPersonCamera->SetUsingAbsoluteRotation(true);
			//PlayerCharacter->ThirdPersonCamera->bUsePawnControlRotation = true; //나중에 다시 돌릴때 필요한 코드

			PlayerCharacter->CharacterPlayMontage(PlayerCharacter->DeathMontage, "Reaction");
		}
	}

	FColor LineColor = bHit ? FColor::Red : FColor::Green;

	DrawDebugSphere(GetWorld(), Start, CIRCLE_RADIUS, 16, LineColor, false, 1.0f);
}
