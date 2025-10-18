// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/BaseActor.h"
#include "NoteActor.generated.h"

/**
 * 
 */

class UStaticMeshComponent;
class UInspectableNoteComponent;

UCLASS()
class PROJECT0_API ANoteActor : public ABaseActor
{
	GENERATED_BODY()
	
	public:
    ANoteActor();

protected:
    virtual void OnConstruction(const FTransform& Transform) override;

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Note")
    UStaticMeshComponent* PaperMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Note")
    UInspectableNoteComponent* NoteComponent;
    
};
