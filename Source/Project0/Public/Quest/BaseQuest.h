// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseQuest.generated.h"

USTRUCT(BlueprintType)
struct FConversationData : public FTableRowBase
{
	GENERATED_BODY()

	//말하는 이
	UPROPERTY()
	FText Who;

	//대화 내용
	UPROPERTY()
	FText Conversation;
};

UCLASS()
class PROJECT0_API ABaseQuest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseQuest();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void RowNumUp();

	UFUNCTION(BlueprintCallable)
	void ResetConversation();

public:
	UPROPERTY()
	FConversationData ConversationData;

	UPROPERTY()
	UDataTable* ConversationDataTable;

public:
	int32 RowNum;
	int32 RowStart;
	int32 RowEnd;
};
