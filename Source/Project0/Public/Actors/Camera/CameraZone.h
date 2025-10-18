// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "CameraZone.generated.h"

UCLASS()
class PROJECT0_API ACameraZone : public ACameraActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACameraZone();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;


	class APlayerCharacter* TargetChar = nullptr;
	class APlayerController* PC = nullptr;

	bool bActive = false;

	UPROPERTY(EditAnywhere)
	float InterpSpeed = 5.0f;

	UPROPERTY(EditAnywhere)
	float BlendInTime = 0.6f;

	UPROPERTY(EditAnywhere)
	float BlendOutTime = 0.6f;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* CamRig = nullptr;

	//캐릭터가 진입하는 지점
	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Trigger;

	//카메라 경로
	UPROPERTY(VisibleAnywhere)
	USceneComponent* PathStart = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* PathEnd = nullptr;

	UFUNCTION()
	void EnterZone(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void ExitZone(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

		
public:	
	

};
