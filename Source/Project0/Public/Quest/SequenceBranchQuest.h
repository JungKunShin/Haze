// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Quest/BaseQuest.h"
#include "SequenceBranchQuest.generated.h"

/**
 * 
 */
UCLASS()
class PROJECT0_API ASequenceBranchQuest : public ABaseQuest
{
	GENERATED_BODY()
	
public:
	ASequenceBranchQuest();

	//DataTable 행 범위(블루프린트에서 A, B 선택마다 다르게 적용)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FIntPoint RowRange = FIntPoint(6001, 6010);

	virtual void RowNumUp() override;

protected:
	virtual void PostInitProperties() override;

private:
	void ApplyRange();                          
	void LoadRowToConversation(int32 Row);
	
};
