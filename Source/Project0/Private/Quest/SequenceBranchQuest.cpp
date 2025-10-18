// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/SequenceBranchQuest.h"

ASequenceBranchQuest::ASequenceBranchQuest()
{
	ApplyRange();
}

void ASequenceBranchQuest::PostInitProperties()
{
	Super::PostInitProperties();
	ApplyRange();
	LoadRowToConversation(RowNum);
}

void ASequenceBranchQuest::ApplyRange()
{
	RowStart = RowRange.X;
	RowEnd = RowRange.Y;
	if (RowNum < RowStart || RowNum > RowEnd) RowNum = RowStart;
}

void ASequenceBranchQuest::RowNumUp()
{
	Super::RowNumUp();
	
}

void ASequenceBranchQuest::LoadRowToConversation(int32 Row)
{
	if (!ConversationDataTable) return;
	const FName RowName(*FString::FromInt(Row));
	if (const FConversationData* R =
		ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("SeqBranch Load")))
	{
		ConversationData.Who = R->Who;
		ConversationData.Conversation = R->Conversation;
	}
}