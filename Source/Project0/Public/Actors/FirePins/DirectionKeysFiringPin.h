// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/FirePins/BaseFiringPin.h"
#include "DirectionKeysFiringPin.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ADirectionKeysFiringPin : public ABaseFiringPin
{
	GENERATED_BODY()
	
protected:

	virtual void OnOverlapStart(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)override;
};
