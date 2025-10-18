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

    //����� ��ȭ ����(�� ��ȣ)
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint MomRange = FIntPoint(4001, 4004);

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Conversation")
    FIntPoint SisterRange = FIntPoint(4005, 4009);

    UFUNCTION(BlueprintCallable)
    void SetActiveTalker(EChapter4Talker In);   //�� �� ��� ����

    //�� ����� ����� �ε����� �ݿ�
    virtual void RowNumUp() override;

private:
    //���� �� ���
    EChapter4Talker ActiveTalker = EChapter4Talker::None;

  
    int32 MomRowNum = 0;
    int32 SisterRowNum = 0;

    void SyncFromTalker();      // ActiveTalker�� ���� RowStart/End/RowNum ����
    void LoadRowToConversation(int32 Row); // RowNum�� ���� �ؽ�Ʈ �ε�
};
