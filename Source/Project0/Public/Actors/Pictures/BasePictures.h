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
	const int32 OPEN_NUM = 4; // ���� ���� �Ϸ᰹��

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
	float PictureRotation; // ���� ����

	int32 CurPitch; // ���� ����

	int32 ResetCurPitch; // ���� �� ���� ���� �ʱ�ȭ

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerRotationComponent* RotateComp = nullptr;

public:
	//�⺻ ī�޶� ��������
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	//�⺻ ī�޶�
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
	static bool IsComplete;	// ���� ���� �ϷῩ��

private:
	static int32 SolutionNum; // ���� ���� ���� ���䰹��

	// ��ü ���� ���� �Ϸ� ��, �ش� ������ ��������
	bool IsPreCorrect = false; 
};
