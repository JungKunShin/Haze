// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MoveBeforeMaze.generated.h"

class UBoxComponent;
class AFriendlyCharacter;
class APlayerCharacter;

UCLASS()
class PROJECT0_API AMoveBeforeMaze : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMoveBeforeMaze();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, Category = "Trigger")
    UBoxComponent* Box;

    
    UPROPERTY(EditInstanceOnly, Category = "Refs")
    AFriendlyCharacter* MomRef = nullptr;

    UPROPERTY(EditInstanceOnly, Category = "Refs")
    AFriendlyCharacter* SisterRef = nullptr;

    //디버그 목적지
    UPROPERTY(EditAnywhere, Category = "Teleport")
    FVector DebugLocation;

    //겹침 방지용
    UPROPERTY(EditAnywhere, Category = "Teleport")
    FVector MomOffset = FVector(100.f, 80.f, 0.f);

    UPROPERTY(EditAnywhere, Category = "Teleport")
    FVector SisterOffset = FVector(100.f, -80.f, 0.f);

    //상태
    bool bTouchedPlayer = false;
    bool bTouchedMom = false;
    bool bTouchedSister = false;
    bool bFiredOnce = false;

    
    TWeakObjectPtr<APlayerCharacter> CachedPlayer;

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    void TryExecute();

    

};
