// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveCamera.generated.h"

UCLASS()
class PROJECT0_API AActiveCamera : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActiveCamera();

	UFUNCTION(BlueprintCallable, Category = "Follow")
	void SetFollowPaused(bool bPaused);

	UFUNCTION(BlueprintPure, Category = "Follow")
	bool IsFollowPaused() const { return bFollowPaused; }

	//��Ƽ�� ī�޶� ��������
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpring;

	//���ο� ī�޶� ���� Ÿ�� ī�޶�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	class UCameraComponent* TargetCamera = nullptr;

	//���ο� ī�޶�� ��ü�Ǵ� �ӵ�
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float InterpSpeed = 5.0f;

	//ȸ�� ��ü �ӵ� 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow", meta = (ClampMin = "0.0"))
	float RotInterpSpeed = 10.0f;

	//���� ������ ����(��ġ-ȸ�� ��� ��ġ)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	bool bSnapOnStart = true;

	// Ÿ�� ȸ���� ������ ����
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	bool bFollowRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	FVector Offset = FVector::ZeroVector;

	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	bool bFollowPaused = false;
	FVector PausedLoc;
	FRotator PausedRot;

	//���ο� �並 ����� ī�޶� ������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ActiveCamera;
};
