#include "ActorComponents/InspectableNoteComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Camera/PlayerCameraManager.h"
#include "Components/PrimitiveComponent.h"
#include "Components/SceneComponent.h"

#include "Blueprint/UserWidget.h"
#include "Components/Widget.h"
#include "Components/TextBlock.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"

#include "HUD/WorldHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Actors/Camera/ActiveCamera.h"
#include "Widget/ObjectDescriptionWidget.h"


static AWorldHUD* GetWorldHUD(APlayerCharacter* Player)
{
    if (!Player) return nullptr;
    if (APlayerController* PC = Cast<APlayerController>(Player->GetController()))
    {
        return Cast<AWorldHUD>(PC->GetHUD());
    }
    return nullptr;
}

UInspectableNoteComponent::UInspectableNoteComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
}

void UInspectableNoteComponent::BeginPlay()
{
    Super::BeginPlay();
    EnsureMeshRef();
    CurrentDistance = InspectDistance;
}

void UInspectableNoteComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTick)
{
    Super::TickComponent(DeltaTime, TickType, ThisTick);

    if (!bIsInteracting) return;

    UpdateAnchorTransform();

    if (bLerping)
    {
        LerpElapsed += DeltaTime;
        const float Alpha = FMath::Clamp(LerpElapsed / LiftDuration, 0.f, 1.f);
        ApplyLerp(Alpha);

        if (Alpha >= 1.f)
        {
            bLerping = false;

            if (!bReturning)
            {
                AttachToAnchor();
            }
            else
            {
                //복귀 처리
                DetachToOriginalParent();
                RestorePhysicsAndCollision();

                //카메라 입력 원복
                if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer ? CurrentInteractPlayer->GetController() : nullptr))
                {
                    PC->SetIgnoreLookInput(false);
                    PC->SetShowMouseCursor(false);
                }

                Super::EndInteraction_Implementation();
                if (CurrentInteractPlayer) CurrentInteractPlayer->EndInteract();
                CurrentInteractPlayer = nullptr;
            }
        }
    }
    else
    {
        if (!bReading && MovedRoot && InspectAnchor)
        {
            if (bUseMouseRotate)
            {
                if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer ? CurrentInteractPlayer->GetController() : nullptr))
                {
                    float DX = 0.f, DY = 0.f;
                    PC->GetInputMouseDelta(DX, DY);
                    AccumulateYaw += DX * MouseRotateSpeed;
                    AccumulatePitch = FMath::Clamp(AccumulatePitch - DY * MouseRotateSpeed, -PitchClamp, PitchClamp);
                }
            }
            else
            {
                AccumulateYaw += PendingInput.X * RotationSpeed * DeltaTime;
                AccumulatePitch = FMath::Clamp(AccumulatePitch + PendingInput.Y * RotationSpeed * DeltaTime, -PitchClamp, PitchClamp);
            }

            const FRotator LocalRot(
                BaseInspectLocalRot.Pitch + AccumulatePitch,
                BaseInspectLocalRot.Yaw + AccumulateYaw,
                BaseInspectLocalRot.Roll  //초기 Roll 유지(화면 내 회전 각)
            );
            MovedRoot->SetRelativeRotation(LocalRot);
        }
        
    }

    if (bFreezeCameraDuringInspect)
    {
        if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer ? CurrentInteractPlayer->GetController() : nullptr))
        {
            PC->SetControlRotation(SavedControlRot); //매 틱 원래 각도로 되돌려 화면 고정
        }
    }

    PendingInput = FVector2D::ZeroVector;
}

void UInspectableNoteComponent::EnsureMeshRef()
{
    if (MeshToInspect) { MovedRoot = MeshToInspect; return; }

    if (AActor* Owner = GetOwner())
    {
        //오너에 있는 첫 번째 Primitive을 기본 대상으로
        if (UPrimitiveComponent* Prim = Owner->FindComponentByClass<UPrimitiveComponent>())
        {
            MeshToInspect = Prim;
            MovedRoot = Prim;
        }
    }
}

UCameraComponent* UInspectableNoteComponent::GetPlayerCamera() const
{
    return CurrentInteractPlayer ? CurrentInteractPlayer->FindComponentByClass<UCameraComponent>() : nullptr;
}

bool UInspectableNoteComponent::GetLiveCamera(FVector& OutLoc, FQuat& OutRot) const
{
    if (CurrentInteractPlayer)
    {
        if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer->GetController()))
        {
            OutLoc = PC->PlayerCameraManager->GetCameraLocation();
            OutRot = PC->PlayerCameraManager->GetCameraRotation().Quaternion();
            return true;
        }
    }
    return false;
}

void UInspectableNoteComponent::EnsureAnchor(APlayerCharacter* Player)
{
    if (!InspectAnchor)
    {
        InspectAnchor = NewObject<USceneComponent>(this, TEXT("InspectAnchor"));
        InspectAnchor->RegisterComponent();
        InspectAnchor->SetMobility(EComponentMobility::Movable);
        InspectAnchor->AttachToComponent(Player->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    }
    UpdateAnchorTransform();
}

void UInspectableNoteComponent::UpdateAnchorTransform() const
{
    if (!InspectAnchor) return;

    FVector CamLoc; FQuat CamRot;
    if (GetLiveCamera(CamLoc, CamRot))
    {
        const FVector TargetLoc = CamLoc + CamRot.GetForwardVector() * CurrentDistance;
        InspectAnchor->SetWorldLocation(TargetLoc);
        InspectAnchor->SetWorldRotation(CamRot);
    }
}

FTransform UInspectableNoteComponent::MakeTargetWorldTransform() const
{
    check(InspectAnchor);
    const FTransform AnchorXf = InspectAnchor->GetComponentTransform();

    //카메라 로컬 기준 최종 회전(가로-세로 전환 포함)
    const FRotator LocalDesired = FRotator(
        InitialFacingOffset.Pitch,
        InitialFacingOffset.Yaw,
        InitialFacingOffset.Roll + ViewRollDeg //세로 전환 핵심
    );

    const FQuat   WorldRot = AnchorXf.GetRotation() * LocalDesired.Quaternion();
    const FVector WorldLoc = AnchorXf.TransformPosition(InspectRelativeOffset);
    const FVector WorldScl = MovedRoot ? MovedRoot->GetComponentScale() : FVector(1.f);

    return FTransform(WorldRot, WorldLoc, WorldScl);
}

void UInspectableNoteComponent::StartLerp(bool bBack)
{
    bReturning = bBack;
    bLerping = true;
    LerpElapsed = 0.f;
}

void UInspectableNoteComponent::ApplyLerp(float Alpha)
{
    if (!MovedRoot) return;

    const FTransform From = bReturning ? MakeTargetWorldTransform() : OriginalWorld;
    const FTransform To = bReturning ? OriginalWorld : MakeTargetWorldTransform();

    const FVector NewLoc = FMath::Lerp(From.GetLocation(), To.GetLocation(), Alpha);
    const FQuat   NewRot = FQuat::Slerp(From.GetRotation(), To.GetRotation(), Alpha);
    const FVector NewScl = FMath::Lerp(From.GetScale3D(), To.GetScale3D(), Alpha);

    MovedRoot->SetWorldLocationAndRotation(NewLoc, NewRot);
    MovedRoot->SetWorldScale3D(NewScl);
}

void UInspectableNoteComponent::AttachToAnchor()
{
    if (!MovedRoot || !InspectAnchor) return;

    //카메라 앵커에 부착(월드 유지)
    MovedRoot->AttachToComponent(InspectAnchor, FAttachmentTransformRules::KeepWorldTransform);

    //앵커(카메라) 기준 상대 위치/회전 세팅
    FRotator R = InitialFacingOffset;
    R.Roll += ViewRollDeg;                //가로-세로 전환(필요 시 0/90)
    MovedRoot->SetRelativeLocation(InspectRelativeOffset);
    MovedRoot->SetRelativeRotation(R);

    //정면 반대면 보정
    if (FVector::DotProduct(MovedRoot->GetForwardVector(), InspectAnchor->GetForwardVector()) < 0.f)
    {
        R = MovedRoot->GetRelativeRotation();
        R.Yaw += 180.f;
        MovedRoot->SetRelativeRotation(R);
    }

    //이 자세를 기준 회전으로 저장(마우스 회전 누적 기준)
    BaseInspectLocalRot = MovedRoot->GetRelativeRotation();
    AccumulateYaw = AccumulatePitch = 0.f;
}

void UInspectableNoteComponent::DetachToOriginalParent()
{
    if (!MovedRoot) return;

    MovedRoot->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);

    if (OriginalParent)
    {
        MovedRoot->AttachToComponent(OriginalParent, FAttachmentTransformRules::KeepWorldTransform, OriginalSocket);
    }
}

void UInspectableNoteComponent::RestorePhysicsAndCollision()
{
    if (MeshToInspect)
    {
        MeshToInspect->SetSimulatePhysics(bSavedSimulate);
        MeshToInspect->SetCollisionEnabled(SavedCollision);
    }
}


void UInspectableNoteComponent::BeginInteraction_Implementation(APlayerCharacter* Player)
{
    if (this->bIsInteracting) return;
    this->bIsInteracting = true;
    CurrentInteractPlayer = Player;
    if (Player) Player->StartInteractTo(this);

    OpenObjectWidget();

   

    if (bAutoRead) {
        SetInspectHUDVisible(false);
        SetObjectPanelVisible(true);
        LockPlayer();
        return;
    }

    EnsureMeshRef();
    EnsureAnchor(Player);

    if (bFreezeCameraDuringInspect)
    {
        if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer->GetController()))
        {
            SavedControlRot = PC->GetControlRotation();
            PC->SetIgnoreLookInput(true);   //입력 무시
            PC->SetShowMouseCursor(true);   //마우스 회전용 커서
        }
    }

    //관찰 중 카메라 시선 고정(선택)
    if (bLockLookInInspect) {
        if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer->GetController())) {
            PC->SetIgnoreLookInput(true);
            PC->SetShowMouseCursor(true);
        }
    }

    if (MeshToInspect) {
        OriginalWorld = MeshToInspect->GetComponentTransform();
        MovedRoot = MeshToInspect;
        OriginalParent = MeshToInspect->GetAttachParent();
        OriginalSocket = MeshToInspect->GetAttachSocketName();
        bSavedSimulate = MeshToInspect->IsSimulatingPhysics();
        SavedCollision = MeshToInspect->GetCollisionEnabled();

        MeshToInspect->SetMobility(EComponentMobility::Movable);
        MeshToInspect->SetSimulatePhysics(false);
        MeshToInspect->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    }

    if (bPauseActiveCameraDuringInspect)
    {
        CachedActiveCam = FindActiveCamera();
        if (CachedActiveCam) CachedActiveCam->SetFollowPaused(true);
    }

    bReading = false;
    StartLerp(false);

 
}

void UInspectableNoteComponent::EndInteraction_Implementation()
{
    if (bAutoRead)
    {
        UnlockPlayer();
        this->bIsInteracting = false;
        if (CurrentInteractPlayer) CurrentInteractPlayer->EndInteract();
        CurrentInteractPlayer = nullptr;
        return;
    }

   

    if (bReading)
    {
        SetObjectPanelVisible(false);
        bReading = false;
    }

    if (bPauseActiveCameraDuringInspect && CachedActiveCam)
    {
        CachedActiveCam->SetFollowPaused(false);
        CachedActiveCam = nullptr;
    }


    StartLerp(true);
   
    if (APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0))
    {
        if (AWorldHUD* WHUD = Cast<AWorldHUD>(PC->GetHUD()))
        {
            if (WHUD->IsObjectWidgetOpen())
            {
                WHUD->CloseObjectWidget();
                return;
            }
        }
    }
}

void UInspectableNoteComponent::MoveInput_Implementation(const FVector2D& Value)
{
    if (bIsInteracting && !bReading)
    {
        PendingInput += Value;
    }
}

void UInspectableNoteComponent::InteractInput_Implementation()
{
    if (!bIsInteracting || bLerping)     return;

    if (bAutoRead)
    {
        EndInteraction_Implementation();
        return;
    }

    if (!bReading)
    {
        SetInspectHUDVisible(false);
        SetObjectPanelVisible(true);
        bReading = true;
    }
    else
    {
        SetObjectPanelVisible(false);
        bReading = false;
        EndInteraction_Implementation();
    }
}



void UInspectableNoteComponent::OpenObjectWidget()
{
    if (AWorldHUD* WHUD = GetWorldHUD(CurrentInteractPlayer))
    {
        WHUD->OpenObjectWidget(ObjectTitle, ObjectText, ObjectDescription, InspectHint, false);
    }

}


void UInspectableNoteComponent::SetInspectHUDVisible(bool bVisible)
{
    //힌트패널 On Off
    if (AWorldHUD* WHUD = GetWorldHUD(CurrentInteractPlayer))
    {
        WHUD->ShowObjectReading(!bVisible);
    }
}

void UInspectableNoteComponent::SetObjectPanelVisible(bool bVisible)
{
    if (AWorldHUD* WHUD = GetWorldHUD(CurrentInteractPlayer))
    {
        WHUD->ShowObjectReading(bVisible);
    }
}


//플레이어 락
void UInspectableNoteComponent::LockPlayer()
{
    if (!CurrentInteractPlayer) return;

    if (UCharacterMovementComponent* Move = CurrentInteractPlayer->GetCharacterMovement())
    {
        Move->DisableMovement();
    }
    if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer->GetController()))
    {
        PC->SetIgnoreLookInput(true);
        PC->SetShowMouseCursor(true);
    }
}

void UInspectableNoteComponent::UnlockPlayer()
{
    if (!CurrentInteractPlayer) return;

    if (UCharacterMovementComponent* Move = CurrentInteractPlayer->GetCharacterMovement())
    {
        Move->SetMovementMode(MOVE_Walking);
    }
    if (APlayerController* PC = Cast<APlayerController>(CurrentInteractPlayer->GetController()))
    {
        PC->SetIgnoreLookInput(false);
        PC->SetShowMouseCursor(false);
    }
}

AActiveCamera* UInspectableNoteComponent::FindActiveCamera() const
{
    return Cast<AActiveCamera>(UGameplayStatics::GetActorOfClass(GetWorld(), AActiveCamera::StaticClass()));
}


