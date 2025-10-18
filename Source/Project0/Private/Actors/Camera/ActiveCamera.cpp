// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Camera/ActiveCamera.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

// Sets default values
AActiveCamera::AActiveCamera()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	USceneComponent* Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));	// ī�޶� �� â��
	CameraSpring->SetupAttachment(RootComponent);										// RootComponent ������ �ֱ�
	
	
	CameraSpring->TargetArmLength = 700.0f;											// ī�޶� �� ���� �ʱ� ����
	CameraSpring->bUsePawnControlRotation = false;

	ActiveCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ActiveCamera"));
	ActiveCamera ->SetupAttachment(CameraSpring);
	ActiveCamera-> bUsePawnControlRotation = false;

	InterpSpeed = 8.f;
	RotInterpSpeed = 10.f;
	bSnapOnStart = true;
}

void AActiveCamera::SetFollowPaused(bool bPaused)
{
	if (bFollowPaused == bPaused) return;
	bFollowPaused = bPaused;

	if (bFollowPaused)
	{
		// ���� ������ ��ġ/ȸ���� ��Ƽ� ����
		PausedLoc = GetActorLocation();
		PausedRot = GetActorRotation();
	}
}

// Called when the game starts or when spawned
void AActiveCamera::BeginPlay()
{
	Super::BeginPlay();

	//PlayerCharacter�� ī�޶�
	if (!TargetCamera)
	{
		if (ACharacter* PCChar = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			TargetCamera = PCChar->FindComponentByClass<UCameraComponent>();
		}
	}
	//������ Ʀ ����
	if (TargetCamera && bSnapOnStart)
	{
		const FTransform T = TargetCamera->GetComponentTransform();

		//���� �������� ȸ�� ����
		const FVector SnappedLoc = T.TransformPosition(Offset);
		const FRotator SnappedRot = bFollowRotation ? T.Rotator() : GetActorRotation();

		SetActorLocationAndRotation(SnappedLoc, SnappedRot, false, nullptr, ETeleportType::TeleportPhysics);
	}

}

// Called every frame
void AActiveCamera::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bFollowPaused) {
	
		SetActorLocationAndRotation(PausedLoc, PausedRot, false, nullptr, ETeleportType::None);
		return;
	}

	if (!TargetCamera) return;

	//���� ī�޶�->Active Camera ���󰡰� �� ��ȭ ���� ����
	const FTransform T = TargetCamera->GetComponentTransform();

	//Ÿ�� ��ġ = Ÿ�� Ʈ������ * Offset
	const FVector TargetLoc = T.TransformPosition(Offset);

	//��ġ ���󰡱�
	const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, InterpSpeed);

	//ȸ�� ���󰡱�
	FRotator NewRot = GetActorRotation();
	if (bFollowRotation)
	{
		const FRotator TargetRot = T.Rotator();
		NewRot = FMath::RInterpTo(NewRot, TargetRot, DeltaTime, RotInterpSpeed);
	}

	SetActorLocationAndRotation(NewLoc, NewRot, false, nullptr, ETeleportType::None);

}

