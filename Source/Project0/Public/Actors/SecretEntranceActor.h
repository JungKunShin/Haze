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

	//�ſ� ���� �� ȣ��
	UFUNCTION()
	void OnMirrorBroken(const FChaosBreakEvent& BreakEvent);

               
    
    UFUNCTION(BlueprintCallable, Category = "SecretEntrance")
    void SetMirrorActorAndRebind(AActor* NewMirror);

    UPROPERTY() 
    class UGeometryCollectionComponent* BoundGC = nullptr;


	//���� ����
	void StartSwingOpen();
	void TickSwing();
	void SetAroundPivot(UPrimitiveComponent* Comp, const FVector& PivotWS, const FQuat& NewRot, bool bSweep, FHitResult* OutHit);

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

    UPROPERTY(EditInstanceOnly, Category = "SecretEntrance")
    AActor* MirrorActor = nullptr;                      //���� �ſ� ����

    UPROPERTY(EditInstanceOnly, Category = "SecretEntrance")
    UStaticMeshComponent* CabinetMesh = nullptr;        //ȸ����ų �޽�

    UPROPERTY(VisibleAnywhere, Category = "SecretEntrance|Swing")
    UArrowComponent* HingeArrow = nullptr;              

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float SwingAngleDeg = 90.f;                         //���� ����

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float SwingDuration = 1.0f;                         //���� �ð�

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    bool bOpenOutwards = true;                          //Arrow Forward �������� ����

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    bool bStopOnHit = true;                             //�浹 ���� ����

    UPROPERTY(EditAnywhere, Category = "SecretEntrance|Swing")
    float LinDamping = 0.3f, AngDamping = 1.2f;         //ȸ�� �� ����

    FTimerHandle SwingTimer;
    bool  bTriggered = false;
    float SwingElapsed = 0.f;

    FVector HingePivotWS;   //��ø ��ǥ
    FVector HingeAxisWS;    //ȸ����
    FQuat   StartRotQ;      //���� ȸ��
    FVector StartOffset;    //�ǹ�-�޽� ��ġ ����

};
