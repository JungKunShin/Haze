// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Mix/BaseMedicineElements.h"
#include "CompletedMedicine.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ACompletedMedicine : public ABaseMedicineElements
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay()override;
	
};
