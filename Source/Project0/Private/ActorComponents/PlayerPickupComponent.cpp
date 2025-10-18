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



	//��ȣ�ۿ� ���� �� �̵� �Է� ����
	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(Holder->GetController()))
	{
		PC->RemoveGameIMC();
	}


	Holder->PendingPickup = this;



	//��Ÿ�ָ� ���(�ٸ� �۾��� AttachToHand����)
	Holder->PlayAnimMontage(Holder->PickupMontage);

	// �ı� �� ���� ����
	GetOwner()->OnDestroyed.AddUniqueDynamic(this, &UPlayerPickupComponent::OnDestroyedPickup);

	//�̲����� ����
	if (UCharacterMovementComponent* MoveComp = Holder->GetCharacterMovement())
	{
		MoveComp->StopMovementImmediately();
	}

}

void UPlayerPickupComponent::AttachToHand()
{

	if (!Holder) return;

	//���� �浹 �ӽ� ����
	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent()))
	{
		bSavedSimulatePhysics = Prim->IsSimulatingPhysics();
		Prim->SetSimulatePhysics(false);
		Prim->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	//�� ���Ͽ� ���� 
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
		//���� �� ���� �ۿ�
		Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Prim->SetSimulatePhysics(true);
		Prim->SetEnableGravity(true);
		Prim->SetNotifyRigidBodyCollision(true);

		//���� ��ü �ո� ����
		Prim->SetUseCCD(true);
		
		//Hit ��������Ʈ ���ε�
		Prim->OnComponentHit.AddUniqueDynamic(this, &UPlayerPickupComponent::OnThrownHit);

		SetPawnIgnore(false);

		//������ ����� ����
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

	Holder->HeldPickup = nullptr;   // ĳ���� ���� �ʱ�ȭ
	Holder = nullptr;

}





void UPlayerPickupComponent::PutDown()
{
	if (!Holder) return;

	AActor* OwnerActor = GetOwner();

	//�տ��� ���� ��ġ ����
	if (USceneComponent* Root = OwnerActor->GetRootComponent())
	{
		Root->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	}

	if (UPrimitiveComponent* Prim = Cast<UPrimitiveComponent>(OwnerActor->GetRootComponent()))
	{
		//�浹-���� Ȱ��ȭ
		Prim->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Prim->SetSimulatePhysics(true);
		Prim->SetEnableGravity(true);
		Prim->SetNotifyRigidBodyCollision(true);

		

		//Ƣ�� �ʵ��� �ӵ� 0
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
	//���� ���� ����
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


	//���� 2�� �������� ���� GeometryCollection(�μ����� ����)�� ������ �������ش�.
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
