// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/ChapterFiveQuest.h"

AChapterFiveQuest::AChapterFiveQuest()
{
    //�⺻: ���� ������ �ʱ�ȭ
    RowStart = MomRange.X;
    RowEnd = MomRange.Y;
    RowNum = RowStart;

    MomRowNum = MomRange.X;
    SisterRowNum = SisterRange.X;

    LoadRowToConversation(RowNum);
}

void AChapterFiveQuest::SetActiveTalker(EChapter5Talker In)
{
    ActiveTalker = In;
    SyncFromTalker();           //���� ��ȭ ����/�ε����� ��� �°� ����
}

void AChapterFiveQuest::SyncFromTalker()
{
    if (ActiveTalker == EChapter5Talker::Mom)
    {
        RowStart = MomRange.X;  RowEnd = MomRange.Y;
        //�ݺ� ����: �����ٸ� ó������
        if (MomRowNum < RowStart || MomRowNum > RowEnd) MomRowNum = RowStart;
        RowNum = MomRowNum;
    }
    else if (ActiveTalker == EChapter5Talker::Sister)
    {
        RowStart = SisterRange.X; RowEnd = SisterRange.Y;
        if (SisterRowNum < RowStart || SisterRowNum > RowEnd) SisterRowNum = RowStart;
        RowNum = SisterRowNum;
    }
    // ���� RowNum�� �ؽ�Ʈ�� ConversationData�� �ݿ�
    LoadRowToConversation(RowNum);
}

void AChapterFiveQuest::RowNumUp()
{
    //�⺻ ����
    Super::RowNumUp();

    //���� �ε����� ������� ����
    if (ActiveTalker == EChapter5Talker::Mom)      MomRowNum = RowNum;
    else if (ActiveTalker == EChapter5Talker::Sister) SisterRowNum = RowNum;
}

void AChapterFiveQuest::LoadRowToConversation(int32 Row)
{
    if (!ConversationDataTable) return;

    const FName RowName(*FString::FromInt(Row));
    if (const FConversationData* R =
        ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("C4 Load")))
    {
        ConversationData.Who = R->Who;
        ConversationData.Conversation = R->Conversation;
    }
}