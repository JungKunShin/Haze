// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/BaseCharacter.h"
#include "NonPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ANonPlayerCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void ResetPatrolPathIndex();
	UFUNCTION(BlueprintPure)
	FVector GetNextPatrolPath();
	
protected:
	UPROPERTY(EditInstanceOnly, meta = (MakeEditWidget = "true"))
	TArray<AActor*> PatrolPaths;

	int PatrolPathIndex = 0;

};
