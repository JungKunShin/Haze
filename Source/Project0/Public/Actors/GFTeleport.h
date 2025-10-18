// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "GFTeleport.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AGFTeleport : public ABaseActor
{
	GENERATED_BODY()
	
private:
	const float CLOSE_LENGTH = 400.f;
	const float ZOOM_SPEED = 300.0f; // 초당 증가량
	const float DOWN_LENGTH = 170.f;

public:
	AGFTeleport();

protected:

	virtual void BeginPlay()override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,	UPrimitiveComponent* OtherComp,	int32 OtherBodyIndex,bool bFromSweep,const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp,AActor* OtherActor,UPrimitiveComponent* OtherComp,int32 OtherBodyIndex);

	void DelayOverlapEnd();
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* InnerBoxComp;

private:
	bool IsIn = false;

	bool IsOut=false;

	float OriginalLength;

	FVector OriginalLoc;

	FTimerHandle OutTH;

	class APlayerCharacter* PlayerCh;
};
