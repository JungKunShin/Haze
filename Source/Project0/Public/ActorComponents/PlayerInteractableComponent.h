// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActorComponents/QuestSystemComponent.h"
#include "PlayerInteractableComponent.generated.h"

class APlayerCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), Blueprintable)
class PROJECT0_API UPlayerInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPlayerInteractableComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndFocus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void BeginInteraction(APlayerCharacter* Player);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void EndInteraction();

	UFUNCTION(BlueprintNativeEvent)
	void MoveInput(const FVector2D& Value);
	UFUNCTION(BlueprintNativeEvent)
	void InteractInput();

	FORCEINLINE bool IsInteracting() { return bIsInteracting; };


public:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	EQuestState ConnectedQuest = EQuestState::None;

protected:
	bool bIsInteracting = false;
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	APlayerCharacter* CurrentInteractingPlayer;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TArray<UMeshComponent*> TargetsToHighlight;
		
};
