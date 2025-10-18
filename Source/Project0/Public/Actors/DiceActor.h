// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DiceActor.generated.h"

class UStaticMeshComponent;
class UArrowComponent;


UCLASS()
class PROJECT0_API ADiceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADiceActor();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* Mesh;

	//BP에서 만든 Face Arrow들을 자동으로 찾음
	UPROPERTY(EditAnywhere, Category = "Dice")
	FName FaceTagPrefix = TEXT("Face"); 

	//dir 방향을 정면으로 봤을 때 보이는 숫자(없으면 -1)
	UFUNCTION(BlueprintCallable, Category = "Dice")
	int32 GetFaceTowardDir(const FVector& Dir, float MinDot = 0.75f) const;

	UFUNCTION(BlueprintCallable, Category = "Dice")
	bool IsSleeping(float LinThresh = 2.f, float AngThresh = 5.f) const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

private:

	void BuildFaceMap() const;

	mutable TMap<int32, TWeakObjectPtr<UArrowComponent>> FaceArrows;
	mutable bool bFaceMapBuilt = false;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
