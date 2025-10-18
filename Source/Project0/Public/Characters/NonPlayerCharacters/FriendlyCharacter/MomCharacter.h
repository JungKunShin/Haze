// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/FriendlyCharacter.h"
#include "MomCharacter.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API AMomCharacter : public AFriendlyCharacter
{
	GENERATED_BODY()
	
public:
	AMomCharacter()
	{
		TriggerRadius = 200.f;
		KeepFollowRadius = 1200.f;
		AcceptRadius = 150.f;
		RepathInterval = 0.3f;
	}
	
};
