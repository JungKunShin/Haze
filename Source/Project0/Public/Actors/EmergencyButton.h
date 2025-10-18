// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EmergencyButton.generated.h"

UCLASS()
class PROJECT0_API AEmergencyButton : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEmergencyButton();

	UPROPERTY(BlueprintReadWrite) 
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly) 
	UMaterialInterface* MatOff;

	UPROPERTY(EditDefaultsOnly) 
	UMaterialInterface* MatOn;

	UPROPERTY(EditAnywhere) 
	int32 MaterialIndexToToggle = 1;

	UPROPERTY(BlueprintReadOnly) bool bIsOn = false;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FButtonToggled, AEmergencyButton*, Button);

	UPROPERTY(BlueprintAssignable) 
	FButtonToggled OnButtonToggled;

	UFUNCTION(BlueprintCallable) 
	void Toggle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable) 
	void SetOn(bool bNewOn, bool bBroadcast = false);

	UFUNCTION(BlueprintCallable) 
	void ResetState();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
