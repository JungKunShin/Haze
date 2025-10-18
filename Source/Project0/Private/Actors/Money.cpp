// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Money.h"
#include "ActorComponents/PlayerPickupComponent.h"


// Sets default values
AMoney::AMoney()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	Mesh->SetSimulatePhysics(true);
	Mesh->SetCollisionProfileName(TEXT("PhysicsActor"));

	Pickup = CreateDefaultSubobject<UPlayerPickupComponent>(TEXT("Pickup"));

}

// Called when the game starts or when spawned
void AMoney::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMoney::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

