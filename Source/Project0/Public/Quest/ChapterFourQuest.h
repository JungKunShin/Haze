// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Quest/BaseQuest.h"
#include "ChapterFourQuest.generated.h"

UENUM(BlueprintType)
enum class EChapter4Talker : uint8 { None, Mom, Sister };

UCLASS()
class PROJECT0_API AChapterFourQuest : public ABaseQuest
{
    GENERATED_BODY()
    public:
    AChapterFourQuest();

    //사람별 대화 범위(행 번호)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint MomRange = FIntPoint(4001, 4004);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint SisterRange = FIntPoint(4005, 4009);

    UFUNCTION(BlueprintCallable)
    void SetActiveTalker(EChapter4Talker In);   //말 건 대상 설정

    //줄 진행시 사람별 인덱스에 반영
    virtual void RowNumUp() override;

private:
    //현재 말 상대
    EChapter4Talker ActiveTalker = EChapter4Talker::None;

  
    int32 MomRowNum = 0;
    int32 SisterRowNum = 0;

    void SyncFromTalker();      // ActiveTalker에 맞춰 RowStart/End/RowNum 스왑
    void LoadRowToConversation(int32 Row); // RowNum의 실제 텍스트 로드
};
