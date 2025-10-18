// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Mix/BaseMedicineElements.h"
#include "ActorComponents/PlayerPickupComponent.h"

ABaseMedicineElements::ABaseMedicineElements()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(false);
	Mesh->SetCollisionProfileName(TEXT("Medicine"));

	PickupComp = CreateDefaultSubobject<UPlayerPickupComponent>(TEXT("PickupComp"));
}

void ABaseMedicineElements::ResetMedicine()
{
	Mesh->SetSimulatePhysics(false);
	SetActorHiddenInGame(false);
	SetActorEnableCollision(true);
	Mesh->SetCollisionProfileName(TEXT("Medicine"));
}

void ABaseMedicineElements::HiddenMedicine()
{
	Mesh->SetSimulatePhysics(false);
	SetActorHiddenInGame(true);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}
