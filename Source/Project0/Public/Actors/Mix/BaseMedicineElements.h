// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "BaseMedicineElements.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ABaseMedicineElements : public ABaseActor
{
	GENERATED_BODY()
	
public:
	ABaseMedicineElements();

	UFUNCTION(BlueprintCallable)
	void ResetMedicine();

	UFUNCTION(BlueprintCallable)
	void HiddenMedicine();
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UStaticMeshComponent* Mesh = nullptr;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UPlayerPickupComponent* PickupComp = nullptr;
};
