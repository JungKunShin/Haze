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

	//액티브 카메라 스프링암
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, Meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraSpring;

	//새로운 카메라가 따라갈 타겟 카메라
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	class UCameraComponent* TargetCamera = nullptr;

	//새로운 카메라로 교체되는 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	float InterpSpeed = 5.0f;

	//회전 교체 속도 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow", meta = (ClampMin = "0.0"))
	float RotInterpSpeed = 10.0f;

	//시작 프레임 스냅(위치-회전 즉시 일치)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Follow")
	bool bSnapOnStart = true;

	// 타깃 회전을 따라갈지 여부
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

	//새로운 뷰를 담당할 카메라 컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* ActiveCamera;
};
