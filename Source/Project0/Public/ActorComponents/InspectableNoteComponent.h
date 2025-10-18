// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorComponents/PlayerInteractableComponent.h"
#include "InspectableNoteComponent.generated.h"

/**
 * 
 */
class UUserWidget;
class UPrimitiveComponent;
class UCameraComponent;
class USceneComponent;
class APlayerCharacter;
class AActiveCamera;


UCLASS()
class 
    UInspectableNoteComponent : public UPlayerInteractableComponent
{
	GENERATED_BODY()
	
	
	public:

   

    UInspectableNoteComponent();

    virtual void BeginPlay() override;

    //틱이랑 똑같음. 캡슐화
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTick) override;

    //Interact 관련 함수
    virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;
    virtual void EndInteraction_Implementation() override;
    virtual void MoveInput_Implementation(const FVector2D& Value) override;
    virtual void InteractInput_Implementation() override;


   //내 앞에 불러들일 메시
    UPROPERTY(VisibleAnywhere, Category = "Inspect", meta = (AdvancedDisplay))
    UPrimitiveComponent* MeshToInspect = nullptr;

    //NoteWidget 켜진 동안 카메라 움직임 제한
    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bPauseActiveCameraDuringInspect = true;

    //관찰 거리
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float InspectDistance = 25.f;

    //메시 다가오는 속도
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float LiftDuration = 0.25f;

    //다가온 메시 회전하는 속도
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float RotationSpeed = 90.f;

    //상하 기울기 제한
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float PitchClamp = 80.f;


    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ObjectTitle;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (MultiLine = "true"), Category = "UI")
    FText ObjectText;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText ObjectDescription;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    FText InspectHint;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
    bool bAutoRead = false;

private:

    //내부 유틸
    void EnsureMeshRef();
    UCameraComponent* GetPlayerCamera() const;
    bool GetLiveCamera(FVector& OutLoc, FQuat& OutRot) const;

    void EnsureAnchor(APlayerCharacter* Player);
    void UpdateAnchorTransform() const;
    FTransform MakeTargetWorldTransform() const;
    void StartLerp(bool bBack);
    void ApplyLerp(float Alpha);
    void AttachToAnchor();
    void DetachToOriginalParent();
    void RestorePhysicsAndCollision();

    //UI
    void OpenObjectWidget();
    void SetInspectHUDVisible(bool bVisible);
    void SetObjectPanelVisible(bool bVisible);


    //플레이어 제어
    void LockPlayer();
    void UnlockPlayer();

    UPROPERTY() FRotator SavedControlRot;
    
    UPROPERTY(EditAnywhere, Category = "Inspect")
    
    bool bFreezeCameraDuringInspect = true;

private:
    UPROPERTY()
    UUserWidget* ObjectWidget = nullptr;

    UPROPERTY()
    APlayerCharacter* CurrentInteractPlayer = nullptr;

    UPROPERTY()
    USceneComponent* InspectAnchor = nullptr;

    UPROPERTY()
    UPrimitiveComponent* MovedRoot = nullptr;

    UPROPERTY()
    USceneComponent* OriginalParent = nullptr;

    UPROPERTY()
    FName OriginalSocket;

    FTransform OriginalWorld;
    bool bSavedSimulate = false;
    TEnumAsByte<ECollisionEnabled::Type> SavedCollision = ECollisionEnabled::NoCollision;

    UPROPERTY(EditAnywhere, Category = "Inspect")
    FRotator InitialFacingOffset = FRotator(-90.f, 180.f, 0.f); 

    //카메라 기준 상대 위치 보정 (살짝 위-아래-앞 조정)
    UPROPERTY(EditAnywhere, Category = "Inspect")
    FVector  InspectRelativeOffset = FVector(0.f, 0.f, 0.f);

    bool bReading = false;
    bool bLerping = false;
    bool bReturning = false;
    float LerpElapsed = 0.f;

    float CurrentDistance = 25.f;
    float AccumulateYaw = 0.f;
    float AccumulatePitch = 0.f;
    FVector2D PendingInput = FVector2D::ZeroVector;

    FRotator BaseInspectLocalRot = FRotator::ZeroRotator;

    UPROPERTY(EditAnywhere, Category = "Inspect")
    float ViewRollDeg = 0.f;    

    UPROPERTY(EditAnywhere, Category = "Inspect")
    float MouseRotateSpeed = 0.15f; //마우스 감도

    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bLockLookInInspect = true; //관찰 중 카메라 시선 고정

    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bUseMouseRotate = true;

    UPROPERTY()
    AActiveCamera* CachedActiveCam = nullptr;

    AActiveCamera* FindActiveCamera() const;

  
};
