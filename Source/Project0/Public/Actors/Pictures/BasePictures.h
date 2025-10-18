// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "BasePictures.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ABasePictures : public ABaseActor
{
	GENERATED_BODY()
	
private:
	const int32 OPEN_NUM = 4; // 액자 퍼즐 완료갯수

public:
	ABasePictures();

protected:
	virtual void BeginPlay()override;

	void Resolution();

	void Unsolution();

public:
	void LeftRotatePicture();

	void RightRotatePicture();

	UFUNCTION(BlueprintCallable)
	void ResetPictures();

protected:
	float PictureRotation; // 액자 각도

	int32 CurPitch; // 현재 각도

	int32 ResetCurPitch; // 리셋 시 현재 각도 초기화

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerRotationComponent* RotateComp = nullptr;

public:
	//기본 카메라 스프링암
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//기본 카메라
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* PictureCamera;

protected:
	UPROPERTY(VisibleAnywhere, BluePrintReadOnly)
	UStaticMeshComponent* FrameMesh;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* CubeMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class APicturePuzzleDoor* Door;

public:
	static bool IsComplete;	// 액자 퍼즐 완료여부

private:
	static int32 SolutionNum; // 현재 액자 퍼즐 정답갯수

	// 전체 액자 퍼즐 완료 전, 해당 퍼즐의 성공여부
	bool IsPreCorrect = false; 
};
