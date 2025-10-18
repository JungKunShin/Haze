// Fill out your copyright notice in the Description page of Project Settings.


#include "Quest/BaseQuest.h"

// Sets default values
ABaseQuest::ABaseQuest()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableObj(TEXT("/Script/Engine.DataTable'/Game/Table/TB_ConversationData.TB_ConversationData'"));
	if (DataTableObj.Succeeded())
	{
		ConversationDataTable = DataTableObj.Object;
	}
}

// Called when the game starts or when spawned
void ABaseQuest::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABaseQuest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseQuest::RowNumUp()
{
    ++RowNum;
    if (ConversationDataTable)
    {
        FName RowName = FName(*FString::FromInt(RowNum)); // ← 데이터 테이블에서 찾고 싶은 행의 이름
        FConversationData* Row = ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("FindNameRow"));

        if (Row)
        {
            ConversationData.Who = Row->Who;
            ConversationData.Conversation = Row->Conversation;
        }
    }
}

void ABaseQuest::ResetConversation()
{
    RowNum = RowStart;  //첫 줄로 되돌리기
    if (ConversationDataTable)
    {
        const FName RowName(*FString::FromInt(RowNum));
        if (auto* Row = ConversationDataTable->FindRow<FConversationData>(RowName, TEXT("Reset")))
        {
            ConversationData.Who = Row->Who;
            ConversationData.Conversation = Row->Conversation;
        }
    }
}

