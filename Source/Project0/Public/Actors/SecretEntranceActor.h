// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Chaos/ChaosEngineInterface.h"
#include "SecretEntranceActor.generated.h"

class UGeometryCollectionComponent;
class UPrimitiveComponent;
class UArrowComponent;

UCLASS()
class PROJECT0_API ASecretEntranceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASecretEntranceActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//거울 깨질 때 호출
	UFUNCTION()
	void OnMirrorBroken(const FChaosBreakEvent& BreakEvent);

               
    
    UFUNCTION(BlueprintCallable, Category = "SecretEntrance")
    void SetMirrorActorAndRebind(AActor* NewMirror);

    UPROPERTY() 
    class UGeometryCollectionComponent* BoundGC = nullptr;


	//스윙 제어
	void StartSwingOpen();
	void TickSwing();
	void SetAroundPivot(UPrimitiveComponent* Comp, const FVector& PivotWS, const FQuat& NewRot, bool bSweep, FHitResult* OutHit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

    UPROPERTY(EditInstanceOnly, Category = "SecretEntrance")
    AActor* MirrorActor = nullptr;                      //깨질 거울 액터

    UPROPERTY(EditInstanceOnly, Category = "SecretEntrance")
    UStaticMeshComponent* CabinetMesh = nullptr;        //회전시킬 메시

    UPROPERTY(VisibleAnywhere, Category = "SecretEntrance|Swing")
    UArrowComponent* HingeArrow = nullptr;              

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float SwingAngleDeg = 90.f;                         //여는 각도

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float SwingDuration = 1.0f;                         //여는 시간

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    bool bOpenOutwards = true;                          //Arrow Forward 방향으로 열기

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    bool bStopOnHit = true;                             //충돌 조기 정지

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float LinDamping = 0.3f, AngDamping = 1.2f;         //회전 중 감쇠

    FTimerHandle SwingTimer;
    bool  bTriggered = false;
    float SwingElapsed = 0.f;

    FVector HingePivotWS;   //경첩 좌표
    FVector HingeAxisWS;    //회전축
    FQuat   StartRotQ;      //시작 회전
    FVector StartOffset;    //피벗-메쉬 위치 벡터

};
