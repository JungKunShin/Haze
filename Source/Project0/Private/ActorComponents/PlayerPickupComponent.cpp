//Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerPickupComponent.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Actors/EmergencyButton.h"

#include "Chaos/ChaosEngineInterface.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Field/FieldSystemObjects.h"

UPlayerPickupComponent::UPlayerPickupComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UPlayerPickupComponent::BeginPlay()
{
	Super::BeginPlay();

	SetPawnIgnore(true);
}

void UPlayerPickupComponent::BeginInteraction_Implementation(APlayerCharacter* Player)
{
	Super::BeginInteraction_Implementation(Player);


	Holder = Player;

	if (!Holder) return;

	Holder->StartInteractTo(this);



	//상호작용 시작 때 이동 입력 끄기
	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Holder->GetController()))
	{
		PC->RemoveGameIMC();
	}


	Holder->PendingPickup = this;



	//몽타주만 재생(다른 작업은 AttachToHand에서)
	Holder->PlayAnimMontage(Holder->PickupMontage);

	// 파괴 시 안전 정리
	GetOwner()->OnDestroyed.AddUniqueDynamic(this, &UPlayerPickupComponent::OnDestroyedPickup);

	//미끄러짐 방지
	if (UCharacterMovementComponent* MoveComp = Holder->GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}

}

void UPlayerPickupComponent::AttachToHand()
{

	if (!Holder) return;

	//물리 충돌 임시 끄기
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()))
	{
		bSavedSimulatePhysics = Prim->IsSimulatingPhysics();
		Prim->SetSimulatePhysics(false);
		Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//손 소켓에 부착 
	GetOwner()->AttachToComponent(
		Holder->GetMesh(),
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		AttachSocketName);

	Holder->HeldPickup = this;
	Holder->PendingPickup = nullptr;


}


void UPlayerPickupComponent::Throw()
{
	if (!Holder) return;
	LastThrower = Holder;

	AActor* OwnerActor = GetOwner();
	OwnerActor->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent()))
	{
		//던질 때 물리 작용
		Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Prim->SetSimulatePhysics(true);
		Prim->SetEnableGravity(true);
		Prim->SetNotifyRigidBodyCollision(true);

		//빠른 물체 뚫림 방지
		Prim->SetUseCCD(true);
		
		//Hit 델리게이트 바인딩
		Prim->OnComponentHit.AddUniqueDynamic(this, &UPlayerPickupComponent::OnThrownHit);

		SetPawnIgnore(false);

		//던지는 방향과 각도
		const float Yaw = Holder->GetActorRotation().Yaw;
		const FRotator LaunchRot(ThrowDegree, Yaw, 0.f);
		const FVector LaunchVel = LaunchRot.Vector() * ThrowImpulse;
		Prim->SetPhysicsLinearVelocity(LaunchVel, true);
	}

	if (UWorld* World = GetWorld())
	{
		World->GetTimerManager().ClearTimer(PawnIgnoreTimer);
		World->GetTimerManager().SetTimer(
			PawnIgnoreTimer, this, &UPlayerPickupComponent::RestorePawnIgnore,
			ThrowPawnIgnoreDelay, false);
	}

	Holder->HeldPickup = nullptr;   // 캐릭터 상태 초기화
	Holder = nullptr;

}





void UPlayerPickupComponent::PutDown()
{
	if (!Holder) return;

	AActor* OwnerActor = GetOwner();

	//손에서 떼고 위치 유지
	if (USceneComponent* Root = OwnerActor->GetRootComponent())
	{
		Root->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent()))
	{
		//충돌-물리 활성화
		Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Prim->SetSimulatePhysics(true);
		Prim->SetEnableGravity(true);
		Prim->SetNotifyRigidBodyCollision(true);

		

		//튀지 않도록 속도 0
		Prim->SetPhysicsLinearVelocity(FVector::ZeroVector, false);
		Prim->SetPhysicsAngularVelocityInDegrees(FVector::ZeroVector, false);

	}

	if (Holder)
	{
		AWorldPlayerController* WPC = Cast< AWorldPlayerController>(Holder->GetController());
		AWorldHUD* WH = Cast< AWorldHUD>(WPC->GetHUD());
		if (WPC && WH&&WH->PutDownWidget)
		{
			WH->SetPutDownVisible(false);
		}
		
	}
	//소유 관계 정리
	Holder->HeldPickup = nullptr;
	Holder = nullptr;
}

void UPlayerPickupComponent::ResetPickUpComp()
{
	if (Holder)
	{
		if (Holder->HeldPickup)
		{
			Holder->SetCanInteract(false);

			EndInteraction_Implementation();

			Holder->EndInteract();

			Holder->PendingPickup = nullptr;
			
			PutDown();
		}
	}
}

void UPlayerPickupComponent::OnThrownHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	

	if (HitComp)
	{
		HitComp->OnComponentHit.RemoveDynamic(this, &UPlayerPickupComponent::OnThrownHit);
	}

	if (!OtherActor) return;


	//밑의 2개 로직으로 맞은 GeometryCollection(부서지는 재질)의 성질을 설정해준다.
	if (UGeometryCollectionComponent* GCC = Cast<UGeometryCollectionComponent>(OtherComp))
	{
		GCC->SetNotifyBreaks(true);
		GCC->SetEnableGravity(true);
		GCC->WakeAllRigidBodies();


		return;
	}
	
	if (UGeometryCollectionComponent* GCC2 = OtherActor->FindComponentByClass<UGeometryCollectionComponent>())
	{
		GCC2->SetEnableGravity(true);
		GCC2->WakeAllRigidBodies();
	}

	if (AEmergencyButton* Btn = Cast<AEmergencyButton>(OtherActor))
	{
		Btn->Toggle();
		return;
	}

}




void UPlayerPickupComponent::OnDestroyedPickup(AActor* DestroyedActor)
{
	if (IsValid(Holder))
	{
		if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Holder->GetController()))
		{
			PC->AddGameIMC();
		}
		Holder->EndInteract();
		Holder->PendingPickup = nullptr;
		if (Holder->HeldPickup == this) Holder->HeldPickup = nullptr;
	}

	Holder = nullptr;
}

void UPlayerPickupComponent::SetPawnIgnore(bool bIgnore)
{
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetOwner() ? GetOwner()->GetRootComponent() : nullptr))
	{
		Prim->SetCollisionResponseToChannel(ECC_Pawn, bIgnore ? ECR_Ignore : ECR_Block);

	}
}

void UPlayerPickupComponent::RestorePawnIgnore()
{
	SetPawnIgnore(true);
}
