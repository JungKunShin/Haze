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

	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));	// ī�޶� �� â��
	CameraBoom->SetupAttachment(RootComponent);										// RootComponent ������ �ֱ�
	CameraBoom->TargetArmLength = 150.0f;											// ī�޶� �� ���� �ʱ� ����
	CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));
	CameraBoom->bDoCollisionTest = false;

	PictureCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("PicCamera"));						// ī�޶� â��
	PictureCamera->SetupAttachment(CameraBoom);																	// ī�޶� �� ������ �ֱ�
}

void ABasePictures::BeginPlay()
{
	if (CameraBoom)
	{
		CameraBoom->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
		// �� �ڵ� �ۼ� ���� ��, ���ڰ� ȸ���� �� ��, ī�޶� ���� ����.
		// �׷��� ��Ʈ ���۳�Ʈ���� ����߷�, ���� ȸ�����ϵ��� ���� �ڵ�
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
	Gimbal Lock ������ ����, �Ϲ������� ����ϴ� FRotator�� ����ϸ�,
	���� ���ļ� �����Ӱ� ȸ���� �� ���� ���°� �ȴ�.
	�̷� ����, ����� �����ϱ� ���� ������ FRotator�� �ƴ�,
	��ǻ�Ͱ� ����ϱ� ���� ȸ�� ����� Quaternion ���
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
