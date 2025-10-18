// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/ChapterThreeQuest.h"

AChapterThreeQuest::AChapterThreeQuest()
{
    RowStart = 3001;
    RowNum = RowStart;
    RowEnd = 3011;

    if (ConversationDataTable)
    {
        const FName RowName(*FString::FromInt(RowNum));
        if (const FConversationData* Row =
            ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("FindNameRow")))
        {
            ConversationData.Who = Row->Who;
            ConversationData.Conversation = Row->Conversation;
        }
    }
}
