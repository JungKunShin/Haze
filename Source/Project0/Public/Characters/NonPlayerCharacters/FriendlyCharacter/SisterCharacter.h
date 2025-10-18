// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/FriendlyCharacter.h"
#include "SisterCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ASisterCharacter : public AFriendlyCharacter
{
	GENERATED_BODY()

public:

	ASisterCharacter()
	{
		TriggerRadius = 200.f;
		KeepFollowRadius = 1200.f;
		AcceptRadius = 150.f;
		RepathInterval = 0.3f;
	}
	
};
