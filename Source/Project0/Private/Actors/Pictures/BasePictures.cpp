// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pictures/BasePictures.h"
#include "ActorComponents/PlayerRotationComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "HUD/WorldHUD.h"
#include "Widget/PicturePuzzleWidget.h"
#include "Actors/Pictures/PicturePuzzleDoor.h"

int32 ABasePictures::SolutionNum = 0;
bool ABasePictures::IsComplete = false;

ABasePictures::ABasePictures()
{
	RotateComp = CreateDefaultSubobject<UPlayerRotationComponent>(TEXT("RotateComp"));

	FrameMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));
	RootComponent = FrameMesh;

	CubeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CubeMesh"));
	CubeMesh->SetupAttachment(FrameMesh);
	CubeMesh->SetRelativeScale3D(FVector(0.3f, 0.01f, 0.4f));

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// 카메라 붐 창조
	CameraBoom->SetupAttachment(RootComponent);										// RootComponent 밑으로 넣기
	CameraBoom->TargetArmLength = 150.0f;											// 카메라 붐 길이 초기 설정
	CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	PictureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PicCamera"));						// 카메라 창조
	PictureCamera->SetupAttachment(CameraBoom);																	// 카메라 붐 밑으로 넣기
}

void ABasePictures::BeginPlay()
{
	if (CameraBoom)
	{
		CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		// 이 코드 작성 안할 시, 액자가 회전을 할 때, 카메라도 같이 돌음.
		// 그래서 루트 컴퍼넌트에서 떨어뜨려, 같이 회전안하도록 막는 코드
	}
}

void ABasePictures::Resolution()
{
	++SolutionNum;

	IsPreCorrect = true;

	if (SolutionNum == OPEN_NUM)
	{
		IsComplete = true;

		APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		AWorldHUD*WH=Cast< AWorldHUD>(PC->GetHUD());
		WH->PicturePuzzleWidget->SetVisibleLeftAndRight();

		Door->OpenTheDoor();
	}
}

void ABasePictures::Unsolution()
{
	--SolutionNum;
}

void ABasePictures::LeftRotatePicture()
{
	CurPitch -= 90;

	if (CurPitch < 0)
	{
		CurPitch = 270;
	}

	/*
	Gimbal Lock 문제로 인해, 일반적으로 사용하는 FRotator를 사용하면,
	축이 겹쳐서 자유롭게 회전할 수 없는 상태가 된다.
	이로 인해, 사람이 이해하기 쉬운 각도인 FRotator가 아닌,
	컴퓨터가 계산하기 편한 회전 방식인 Quaternion 사용
	*/
	FQuat QuatRotation = GetActorQuat();
	FQuat QuatAdd = FQuat(GetActorRightVector(), FMath::DegreesToRadians(-90.f));
	QuatRotation = QuatAdd * QuatRotation;
	SetActorRotation(QuatRotation);


	if (CurPitch == PictureRotation)
	{
		Resolution();
	}
	else
	{
		if (IsPreCorrect)
		{
			IsPreCorrect = false;
			Unsolution();
		}
	}
	InteractSoundInit();
}

void ABasePictures::RightRotatePicture()
{
	CurPitch += 90;
	CurPitch %= 360;

	FQuat QuatRotation = GetActorQuat();
	FQuat QuatAdd = FQuat(GetActorRightVector(), FMath::DegreesToRadians(90.f));
	QuatRotation = QuatAdd * QuatRotation;
	SetActorRotation(QuatRotation);


	if (CurPitch == PictureRotation)
	{
		Resolution();
	}
	else
	{
		if (IsPreCorrect)
		{
			IsPreCorrect = false;
			Unsolution();
		}
	}
	InteractSoundInit();
}

void ABasePictures::ResetPictures()
{
	CurPitch = ResetCurPitch;
	IsComplete = false;
	SolutionNum = 0;
	IsPreCorrect = false;
}
