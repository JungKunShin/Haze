// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "CoreMinimal.h"
#include "Quest/BaseQuest.h"
#include "ChapterFiveQuest.generated.h"

UENUM(BlueprintType)
enum class EChapter5Talker : uint8 { None, Mom, Sister };

UCLASS()
class PROJECT0_API AChapterFiveQuest : public ABaseQuest
{
    GENERATED_BODY()
    public:
    AChapterFiveQuest();

    //����� ��ȭ ����(�� ��ȣ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint MomRange = FIntPoint(5001, 5014);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint SisterRange = FIntPoint(5015, 5018);

    UFUNCTION(BlueprintCallable)
    void SetActiveTalker(EChapter5Talker In);   //�� �� ��� ����

    //�� ����� ����� �ε����� �ݿ�
    virtual void RowNumUp() override;

private:
    //���� �� ���
    EChapter5Talker ActiveTalker = EChapter5Talker::None;

    int32 MomRowNum = 0;
    int32 SisterRowNum = 0;

    void SyncFromTalker();      // ActiveTalker�� ���� RowStart/End/RowNum ����
    void LoadRowToConversation(int32 Row); // RowNum�� ���� �ؽ�Ʈ �ε�
};
