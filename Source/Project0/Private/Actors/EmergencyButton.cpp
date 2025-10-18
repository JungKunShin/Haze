// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/EmergencyButton.h"
#include "ActorComponents/PlayerPickupComponent.h"

// Sets default values
AEmergencyButton::AEmergencyButton()
{
 	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);
	Mesh->SetNotifyRigidBodyCollision(true);
	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName(TEXT("BlockAll"));

}

// Called when the game starts or when spawned
void AEmergencyButton::BeginPlay()
{
	Super::BeginPlay();
	if (MatOff) Mesh->SetMaterial(MaterialIndexToToggle, MatOff);

	const int32 Num = Mesh->GetNumMaterials();

}

void AEmergencyButton::Toggle() {


	bIsOn = !bIsOn;

	Mesh->SetMaterial(MaterialIndexToToggle, bIsOn ? MatOn : MatOff);
	

	OnButtonToggled.Broadcast(this);

}



// Called every frame
void AEmergencyButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEmergencyButton::SetOn(bool bNewOn, bool bBroadcast /*=false*/)
{
	bIsOn = bNewOn;
	if (Mesh)
	{
		Mesh->SetMaterial(MaterialIndexToToggle, bIsOn ? MatOn : MatOff);
	}
	if (bBroadcast)
	{
		OnButtonToggled.Broadcast(this);
	}
}

void AEmergencyButton::ResetState()
{
	SetOn(false, false); 
}