// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "TutorialEnd.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ATutorialEnd : public ABaseActor
{
	GENERATED_BODY()
	
public:
	ATutorialEnd();

protected:
	virtual void BeginPlay()override;

	UFUNCTION()
	virtual void OnTutorialOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Collision")
	class UBoxComponent* BoxComp;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AActor* TeleportPos;
};
