// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TreasureChest.generated.h"

class USkeletalMeshComponent;
class UAnimMontage;
class ADiceCarpet;

UCLASS()
class PROJECT0_API ATreasureChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATreasureChest();

	UPROPERTY(VisibleAnywhere) 
	USkeletalMeshComponent* Mesh;

	UPROPERTY(EditInstanceOnly) 
	ADiceCarpet* CarpetA;

	UPROPERTY(EditInstanceOnly) 
	ADiceCarpet* CarpetB;

	UPROPERTY(EditInstanceOnly) 
	ADiceCarpet* CarpetC;

	UPROPERTY(EditAnywhere) 
	UAnimMontage* OpenMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	TArray<AActor*> MoneyActors;                

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Loot")
	USceneComponent* EjectDir = nullptr;         

	UPROPERTY(EditAnywhere, Category = "Loot")
	float EjectImpulse = 100000.f;               

	UPROPERTY(EditAnywhere, Category = "Loot")
	float EjectConeDeg = 12.f;                   


	UFUNCTION() 
	void OnMontageNotifyBegin(FName NotifyName, const FBranchingPointNotifyPayload& Payload);
	void EjectMoney();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION() 
	void OnCarpetChanged(bool None);


	void TryOpen();

private:

	bool bOpened = false;

	FTimerHandle FreezeTimer;

	void FreezeAtEnd();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
