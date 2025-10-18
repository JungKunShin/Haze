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

	//DataTable �� ����(�������Ʈ���� A, B ���ø��� �ٸ��� ����)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
	FIntPoint RowRange = FIntPoint(6001, 6010);

	virtual void RowNumUp() override;

protected:
	virtual void PostInitProperties() override;

private:
	void ApplyRange();                          
	void LoadRowToConversation(int32 Row);
	
};
