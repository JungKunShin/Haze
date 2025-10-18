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

	CameraSpring = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpring"));	// 카메라 붐 창조
	CameraSpring->SetupAttachment(RootComponent);										// RootComponent 밑으로 넣기
	
	
	CameraSpring->TargetArmLength = 700.0f;											// 카메라 붐 길이 초기 설정
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
		// 정지 시점의 위치/회전을 잡아서 고정
		PausedLoc = GetActorLocation();
		PausedRot = GetActorRotation();
	}
}

// Called when the game starts or when spawned
void AActiveCamera::BeginPlay()
{
	Super::BeginPlay();

	//PlayerCharacter의 카메라
	if (!TargetCamera)
	{
		if (ACharacter* PCChar = UGameplayStatics::GetPlayerCharacter(this, 0))
		{
			TargetCamera = PCChar->FindComponentByClass<UCameraComponent>();
		}
	}
	//프레임 튐 방지
	if (TargetCamera && bSnapOnStart)
	{
		const FTransform T = TargetCamera->GetComponentTransform();

		//로컬 기준으로 회전 적용
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

	//기존 카메라->Active Camera 따라가게 해 영화 같은 연출
	const FTransform T = TargetCamera->GetComponentTransform();

	//타깃 위치 = 타깃 트랜스폼 * Offset
	const FVector TargetLoc = T.TransformPosition(Offset);

	//위치 따라가기
	const FVector NewLoc = FMath::VInterpTo(GetActorLocation(), TargetLoc, DeltaTime, InterpSpeed);

	//회전 따라가기
	FRotator NewRot = GetActorRotation();
	if (bFollowRotation)
	{
		const FRotator TargetRot = T.Rotator();
		NewRot = FMath::RInterpTo(NewRot, TargetRot, DeltaTime, RotInterpSpeed);
	}

	SetActorLocationAndRotation(NewLoc, NewRot, false, nullptr, ETeleportType::None);

}

