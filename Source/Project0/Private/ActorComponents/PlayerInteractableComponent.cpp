// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/PlayerInteractableComponent.h"

#include "Characters/PlayerCharacters/PlayerCharacter.h"

UPlayerInteractableComponent::UPlayerInteractableComponent()
{
	PrimaryComponentTick.bCanEverTick = true;


}

void UPlayerInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UPlayerInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UPlayerInteractableComponent::MoveInput_Implementation(const FVector2D& Value)
{

}

void UPlayerInteractableComponent::InteractInput_Implementation()
{

}

void UPlayerInteractableComponent::BeginFocus_Implementation()
{
	for (UMeshComponent* Target : TargetsToHighlight) {
		if (Target)
		{
			Target->SetOverlayMaterialMaxDrawDistance(5000.0f);
		}
	}
}

void UPlayerInteractableComponent::EndFocus_Implementation()
{
	for (UMeshComponent* Target : TargetsToHighlight) {
		if (Target)
		{
			Target->SetOverlayMaterialMaxDrawDistance(0.01f);
		}
	}
}

void UPlayerInteractableComponent::BeginInteraction_Implementation(APlayerCharacter* Player)
{
	bIsInteracting = true;
	CurrentInteractingPlayer = Player;
	EndFocus();

	Player->StartInteractTo(this);
}

void UPlayerInteractableComponent::EndInteraction_Implementation()
{
	bIsInteracting = false;
	if (CurrentInteractingPlayer) {
		CurrentInteractingPlayer->EndInteract();
	}
}

