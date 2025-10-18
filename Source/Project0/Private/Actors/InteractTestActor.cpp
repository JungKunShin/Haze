// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/InteractTestActor.h"
#include "Components/StaticMeshComponent.h"
#include "ActorComponents/PlayerPickupComponent.h"

AInteractTestActor::AInteractTestActor()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);                      
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	PickupComp = CreateDefaultSubobject<UPlayerPickupComponent>(TEXT("PickupComp"));

}
