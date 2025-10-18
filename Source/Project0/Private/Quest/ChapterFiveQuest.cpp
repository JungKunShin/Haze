// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/ChapterFiveQuest.h"

AChapterFiveQuest::AChapterFiveQuest()
{
    //기본: 엄마 범위로 초기화
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
    SyncFromTalker();           //현재 대화 범위/인덱스를 대상에 맞게 스왑
}

void AChapterFiveQuest::SyncFromTalker()
{
    if (ActiveTalker == EChapter5Talker::Mom)
    {
        RowStart = MomRange.X;  RowEnd = MomRange.Y;
        //반복 보장: 끝났다면 처음으로
        if (MomRowNum < RowStart || MomRowNum > RowEnd) MomRowNum = RowStart;
        RowNum = MomRowNum;
    }
    else if (ActiveTalker == EChapter5Talker::Sister)
    {
        RowStart = SisterRange.X; RowEnd = SisterRange.Y;
        if (SisterRowNum < RowStart || SisterRowNum > RowEnd) SisterRowNum = RowStart;
        RowNum = SisterRowNum;
    }
    // 현재 RowNum의 텍스트를 ConversationData에 반영
    LoadRowToConversation(RowNum);
}

void AChapterFiveQuest::RowNumUp()
{
    //기본 진행
    Super::RowNumUp();

    //진행 인덱스를 사람별로 저장
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