// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/ChapterOneQuest.h"

AChapterOneQuest::AChapterOneQuest()
{
  
    RowStart = 1001;
    RowNum = RowStart;
    RowEnd = 1016;
    if (ConversationDataTable)
    {
        FName RowName = FName(*FString::FromInt(RowNum)); // �� ������ ���̺��� ã�� ���� ���� �̸�
        FConversationData* Row = ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("FindNameRow"));

        if (Row)
        {
            ConversationData.Who = Row->Who;
            ConversationData.Conversation = Row->Conversation;
        }
    }
}
