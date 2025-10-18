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

    //ƽ�̶� �Ȱ���. ĸ��ȭ
    virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTick) override;

    //Interact ���� �Լ�
    virtual void BeginInteraction_Implementation(APlayerCharacter* Player) override;
    virtual void EndInteraction_Implementation() override;
    virtual void MoveInput_Implementation(const FVector2D& Value) override;
    virtual void InteractInput_Implementation() override;


   //�� �տ� �ҷ����� �޽�
    UPROPERTY(VisibleAnywhere, Category = "Inspect", meta = (AdvancedDisplay))
    UPrimitiveComponent* MeshToInspect = nullptr;

    //NoteWidget ���� ���� ī�޶� ������ ����
    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bPauseActiveCameraDuringInspect = true;

    //���� �Ÿ�
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float InspectDistance = 25.f;

    //�޽� �ٰ����� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float LiftDuration = 0.25f;

    //�ٰ��� �޽� ȸ���ϴ� �ӵ�
    UPROPERTY(EditAnywhere, Category = "Inspect")
    float RotationSpeed = 90.f;

    //���� ���� ����
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

    //���� ��ƿ
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


    //�÷��̾� ����
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

    //ī�޶� ���� ��� ��ġ ���� (��¦ ��-�Ʒ�-�� ����)
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
    float MouseRotateSpeed = 0.15f; //���콺 ����

    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bLockLookInInspect = true; //���� �� ī�޶� �ü� ����

    UPROPERTY(EditAnywhere, Category = "Inspect")
    bool bUseMouseRotate = true;

    UPROPERTY()
    AActiveCamera* CachedActiveCam = nullptr;

    AActiveCamera* FindActiveCamera() const;

  
};
