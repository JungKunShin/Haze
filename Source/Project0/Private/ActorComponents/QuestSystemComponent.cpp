// Fill out your copyright notice in the Description page of Project Settings.


#include "ActorComponents/QuestSystemComponent.h"
#include "PlayerControllers/WorldPlayerController.h"
#include "HUD/WorldHUD.h"
#include "Quest/ChapterOneQuest.h"
#include "Quest/ChapterTwoQuest.h"
#include "Quest/ChapterThreeQuest.h"
#include "Quest/ChapterFourQuest.h"
#include "Quest/ChapterFiveQuest.h"
#include "Quest/SequenceBranchQuest.h"
#include "Quest/GrandFatherDeathQuest.h"
#include "Characters/NonPlayerCharacters/EnemyCharacter.h"
#include "Characters/NonPlayerCharacters/GrandFatherCharacter.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/FriendlyCharacter.h"
#include "Characters/PlayerCharacters/PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Widget/ConversationWidget.h"
#include "CineCameraActor.h"
#include "Characters/NonPlayerCharacters/FriendlyCharacter/SisterCharacter.h"
#include "Blueprint/WidgetBlueprintLibrary.h" 

EQuestState UQuestSystemComponent::CurQuestState = EQuestState::None;
// Sets default values for this component's properties
UQuestSystemComponent::UQuestSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UQuestSystemComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UQuestSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UQuestSystemComponent::QuestProgress(EQuestState _CurQuest)
{


	switch (_CurQuest)
	{
	case EQuestState::None:
		CurQuestState= EQuestState::FirstConversation;
		Conversation(CurQuestState); //첫번째 대화
		break;
	case EQuestState::FirstConversation:
		CurQuestState = EQuestState::DaughterBook;
		Conversation(CurQuestState); //책 갖다 주기
		break;
	case EQuestState::DaughterBook:
		CurQuestState = EQuestState::MomTalking;
		break; //엄마한테 말걸기
	case EQuestState::MomTalking: 
		CurQuestState = EQuestState::MomBarn;
		Conversation(CurQuestState); //엄마한테 돈 갖다 주기
		break;
		
	case EQuestState::MomBarn:
		CurQuestState = EQuestState::MomMix; // 액자 퍼즐 풀기
		break; 
	case EQuestState::MomMix:
		CurQuestState = EQuestState::MazeBefore; // 엄마 약 갖다주기
		Conversation(CurQuestState);
		break; 
	case EQuestState::MazeBefore:
		CurQuestState = EQuestState::MazeAfter; // 미로로 이동
		break;
	case EQuestState::MazeAfter:
		CurQuestState = EQuestState::End;
		break;
	case EQuestState::End:
		break;
	case EQuestState::GrandFatherDeath:
		break;
	case EQuestState::ZombieDeath:
		break;
	}
}

void UQuestSystemComponent::AdvanceCurrentState()
{
	QuestProgress(CurQuestState);
}

void UQuestSystemComponent::Conversation(EQuestState _CurQuest)
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* WHUD = Controller ? Cast<AWorldHUD>(Controller->GetHUD()) : nullptr;

	const bool bWidgetOpen = (WHUD && WHUD->ConversationWidget && WHUD->ConversationWidget->IsInViewport());
	if (WHUD && !bWidgetOpen)
	{
		//끝까지 읽었지만 아직 같은 퀘스트 상태라면 처음부터 반복 가능하게 리셋
		if (CurQuest && CurQuest->RowNum > CurQuest->RowEnd)
		{
			CurQuest->ResetConversation();
		}
		//위젯 켜기
		WHUD->SetConversationWidgetVisible(true);
	}
	
	switch (CurQuestState)
	{
	case EQuestState::None:
		break;
	case EQuestState::FirstConversation:
		ChapterOneInit();
		break;
	case EQuestState::DaughterBook:
		ChapterTwoInit();
		break;
	case EQuestState::MomTalking:
		ChapterThreeInit();
		break;
	case EQuestState::MomBarn:
		ChapterFourInit();
		break;
	case EQuestState::MazeBefore:
		ChapterFiveInit();
		break;
	case EQuestState::MazeAfter:
		break;
	case EQuestState::SequenceOne: 
		SequenceOneInit(); 
		break;
	case EQuestState::SequenceTwo: 
		SequenceTwoInit(); 
		break;
	case EQuestState::End:
		break;
	case EQuestState::GrandFatherDeath:
		GrandFatherDeath();
		break;
	case EQuestState::ZombieDeath:
		break;
	}
}

void UQuestSystemComponent::SequenceStartDialogueByPick(EFinalPick Pick)
{
	const EQuestState Next = (Pick == EFinalPick::PickA)
		? EQuestState::SequenceOne : EQuestState::SequenceTwo;

	CurQuestState = Next;
	Conversation(CurQuestState);
}

void UQuestSystemComponent::SequenceNextLine()
{
	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr;

	if (!HUD || !CurQuest) return;

	if (CurQuest->RowNum > CurQuest->RowEnd) return; 
	HUD->SetConversationText(CurQuest);
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::SequenceJumpToRow(int32 Row)
{
	if (!CurQuest) return;

	CurQuest->RowNum = Row;

	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr;
	if (!HUD) return;

	HUD->SetConversationWidgetVisible(true);   
	if (!HUD->ConversationWidget) return;

	HUD->SetConversationText(CurQuest);
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::SequenceFinishDialogue(EQuestState NextStateAfter)
{
	SetSequenceInputBlocked(false);

	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController()))
		if (AWorldHUD* HUD = Cast<AWorldHUD>(PC->GetHUD()))
			HUD->SetConversationWidgetVisible(false);

	CurQuestState = NextStateAfter;
}

void UQuestSystemComponent::ChapterOneInit()
{

	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Cast<AWorldHUD>(Controller->GetHUD());

	if (!HUD)return;

	if (ChapterOneINS&& CurQuest!= ChapterOneINS)
	{
		CurQuest = ChapterOneINS;
		CurQuest->ResetConversation();
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}

	if (!ChapterOneINS)
	{
		ChapterOneINS = NewObject<AChapterOneQuest>(this, ChapterOne);
		CurQuest = ChapterOneINS;
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}
	else if(CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		return;
	}
	else
	{
		HUD->SetConversationText(CurQuest);
	}
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::ChapterTwoInit()
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Cast<AWorldHUD>(Controller->GetHUD());

	if (!HUD)return;

	if (ChapterTwoINS && CurQuest != ChapterTwoINS)
	{
		CurQuest = ChapterTwoINS;
		CurQuest->ResetConversation();
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}

	if (!ChapterTwoINS)
	{
		ChapterTwoINS = NewObject<AChapterTwoQuest>(this, ChapterTwo);
		CurQuest = ChapterTwoINS;
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}
	else if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		QuestProgress(CurQuestState);
		return;
	}
	else
	{
		HUD->SetConversationText(CurQuest);
	}
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::ChapterThreeInit()
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Controller ? Cast<AWorldHUD>(Controller->GetHUD()) : nullptr;
	if (!HUD) return;

	if (ChapterThreeINS && CurQuest != ChapterThreeINS)
	{
		CurQuest = ChapterThreeINS;
		CurQuest->ResetConversation();
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}

	if (!ChapterThreeINS)
	{
		
		ChapterThreeINS = NewObject<ABaseQuest>(this, ChapterThree ? *ChapterThree : AChapterThreeQuest::StaticClass());
		CurQuest = ChapterThreeINS;
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}

	else if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		return;
	}
	else
	{
		HUD->SetConversationText(CurQuest);
	}
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::ChapterFourInit()
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Controller ? Cast<AWorldHUD>(Controller->GetHUD()) : nullptr;
	if (!HUD) return;

	//챕터4 인스턴스 보장
	if (!ChapterFourINS)
	{
		ChapterFourINS = NewObject<ABaseQuest>(this, ChapterFour ? *ChapterFour : AChapterFourQuest::StaticClass());
	}
	CurQuest = ChapterFourINS;

	//여기서 말 건 대상 적용
	if (PendingTalker != ETalker::None)
	{
		if (auto* C4 = Cast<AChapterFourQuest>(CurQuest))
		{
			const EChapter4Talker Who =
				(PendingTalker == ETalker::Mom) ? EChapter4Talker::Mom : EChapter4Talker::Sister;
			C4->SetActiveTalker(Who);
		}
		PendingTalker = ETalker::None; //소진했으면 리셋
	}

	HUD->SetConversationWidgetVisible(true);

	if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		return;
	}

	HUD->SetConversationText(CurQuest);

	CurQuest->RowNumUp();
}

void UQuestSystemComponent::ChapterFiveInit()
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Controller ? Cast<AWorldHUD>(Controller->GetHUD()) : nullptr;
	if (!HUD) return;

	if (!ChapterFiveINS)
		ChapterFiveINS = NewObject<ABaseQuest>(this, ChapterFive ? *ChapterFive : AChapterFiveQuest::StaticClass());
	CurQuest = ChapterFiveINS;

	ETalker AppliedTalker = PendingTalker;


	if (PendingTalker != ETalker::None)
	{
		if (auto* C5 = Cast<AChapterFiveQuest>(CurQuest))
		{
			const EChapter5Talker Who =
				(PendingTalker == ETalker::Mom) ? EChapter5Talker::Mom : EChapter5Talker::Sister;
			C5->SetActiveTalker(Who); 
		}
		PendingTalker = ETalker::None;
	}

	HUD->SetConversationWidgetVisible(true);

	if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		if (AppliedTalker == ETalker::Sister)
		{
			if (auto* C5 = Cast<AChapterFiveQuest>(CurQuest))
			{
				C5->SetActiveTalker(EChapter5Talker::Sister); 
			}
			HUD->SetConversationText(CurQuest);
			CurQuest->RowNumUp();
			return; 
		}

		TArray<AActor*> FriendlyList;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AFriendlyCharacter::StaticClass(), FriendlyList);
		for (AActor* A : FriendlyList)
			if (AFriendlyCharacter* FC = Cast<AFriendlyCharacter>(A))
				FC->SetRequireQuestUnlock(true);

		HUD->SetConversationWidgetVisible(false);
		return; 
	}

	HUD->SetConversationText(CurQuest);
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::SequenceOneInit()
{

	SetSequenceInputBlocked(true);

	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr;
	if (!HUD) return;

	if (!SequenceOneINS)
		SequenceOneINS = NewObject<ABaseQuest>(this, SequenceOneClass ? *SequenceOneClass : ASequenceBranchQuest::StaticClass());

	CurQuest = SequenceOneINS;
	HUD->SetConversationWidgetVisible(true);

	if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		CurQuestState = EQuestState::End; 
		return;
	}
	HUD->SetConversationText(CurQuest);
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::SequenceTwoInit()
{
	SetSequenceInputBlocked(true);


	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr;
	if (!HUD) return;

	if (!SequenceTwoINS)
		SequenceTwoINS = NewObject<ABaseQuest>(this, SequenceTwoClass ? *SequenceTwoClass : ASequenceBranchQuest::StaticClass());

	CurQuest = SequenceTwoINS;
	HUD->SetConversationWidgetVisible(true);

	if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);
		CurQuestState = EQuestState::End;
		return;
	}
	HUD->SetConversationText(CurQuest);
	CurQuest->RowNumUp();
}

void UQuestSystemComponent::SetSequenceInputBlocked(bool bBlocked)
{
	bSeqInputBlocked = bBlocked;

	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	if (!PC) return;

	if (bBlocked)
	{
		PC->DeleteAllIMC();
	}
	else
	{
		PC->InitAllIMC();
	}
}


void UQuestSystemComponent::GrandFatherDeath()
{
	AWorldPlayerController* Controller = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = Cast<AWorldHUD>(Controller->GetHUD());

	if (!HUD)return;

	if (GrandFatherDeathINS && CurQuest != GrandFatherDeathINS)
	{
		CurQuest = GrandFatherDeathINS;
		CurQuest->ResetConversation();
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}

	if (!GrandFatherDeathINS)
	{
		GrandFatherDeathINS = NewObject<AGrandFatherDeathQuest>(this, GrandFatherDeathState);
		CurQuest = GrandFatherDeathINS;
		HUD->SetConversationWidgetVisible(true);
		HUD->SetConversationText(CurQuest);
	}
	else if (CurQuest->RowNum > CurQuest->RowEnd)
	{
		HUD->SetConversationWidgetVisible(false);

		TArray<AActor*> EnemyCh;
		UGameplayStatics::GetAllActorsWithTag(GetWorld(), FName("Chasing"), EnemyCh);

		for (AActor* MyActor : EnemyCh)
		{
			AGrandFatherCharacter* GrandFather = Cast<AGrandFatherCharacter>(MyActor);
			if (GrandFather)
			{
				GrandFather->KillPlayer();
				break;
			}
		}

		CurQuestState = HitTempQuestState;

		APlayerCharacter* PlayerCharacter = Cast< APlayerCharacter>(GetOwner());
		PlayerCharacter->CharacterPlayMontage(PlayerCharacter->DeathMontage);
		HUD->SetDeathWidgetVisible(true);
		return;
	}
	else
	{
		HUD->SetConversationText(CurQuest);
	}
	CurQuest->RowNumUp();
}

EQuestState UQuestSystemComponent::CurrentQuestStateJudgement()
{
	return CurQuestState;
}
void UQuestSystemComponent::EnterMazeAfter()
{
	CurQuestState = EQuestState::MazeAfter;
	BindFinalTriggersOnce();
	SetFinalTriggersEnabled(true);
	ActiveTrigger = nullptr;
	bChoiceOpen = false;
	PendingSequenceState = EQuestState::None;
}

void UQuestSystemComponent::BindFinalTriggersOnce()
{
	if(bFinalBound) 
	return;

	TArray<AActor*> Found;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATriggerFinal::StaticClass(), Found);
	for (AActor* A : Found)
	{
		if (ATriggerFinal* T = Cast<ATriggerFinal>(A))
		{
			FinalTriggers.Add(T);
			T->OnPromptRequested.RemoveAll(this);
			T->OnPromptRequested.AddDynamic(this, &UQuestSystemComponent::OnFinalPrompt);
			T->SetEnabled(false);
		}
	}
	bFinalBound = true;
}

void UQuestSystemComponent::SetFinalTriggersEnabled(bool bEnable)
{
	for (ATriggerFinal* T : FinalTriggers)
		if (IsValid(T)) T->SetEnabled(bEnable);
}

//겹침-> 선택지 띄우기
void UQuestSystemComponent::OnFinalPrompt(ATriggerFinal* Trigger)
{
	if (CurQuestState != EQuestState::MazeAfter || bChoiceOpen || !Trigger) return;
	ActiveTrigger = Trigger; bChoiceOpen = true;

	AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController());
	AWorldHUD* HUD = PC ? Cast<AWorldHUD>(PC->GetHUD()) : nullptr;
	if (!HUD) { bChoiceOpen = false; ActiveTrigger = nullptr; return; }

	HUD->SetConversationWidgetVisible(true);
	
	if (!HUD->ConversationWidget)
	{
		bChoiceOpen = false;
		ActiveTrigger = nullptr;
		return;
	}

	TArray<FConvChoice> Opts;
	Opts.Add({ Trigger->ChoiceALabel, FName("PickA") });
	Opts.Add({ Trigger->ChoiceBLabel, FName("PickB") });

	HUD->ConversationWidget->OnChoicePicked.RemoveAll(this);
	HUD->ConversationWidget->OnChoicePicked.AddDynamic(this, &UQuestSystemComponent::OnFinalChoicePicked);
	HUD->ConversationWidget->EnterChoiceMode(Opts);
}

//선택확정 및 시퀀스 재생 분기 상태로 이동
void UQuestSystemComponent::OnFinalChoicePicked(FName Tag)
{
	if (!ActiveTrigger) return;

	if (AWorldPlayerController* PC = Cast<AWorldPlayerController>(GetWorld()->GetFirstPlayerController()))
		if (AWorldHUD* HUD = Cast<AWorldHUD>(PC->GetHUD()))
			HUD->SetConversationWidgetVisible(false);


	PendingSequenceState = (Tag == FName("PickA")) ? EQuestState::SequenceOne : EQuestState::SequenceTwo;

	const EFinalPick Pick = (PendingSequenceState == EQuestState::SequenceOne) ? EFinalPick::PickA : EFinalPick::PickB;
	ActiveTrigger->PlayChosen(Pick);

	bChoiceOpen = false;
	SetFinalTriggersEnabled(false);
}

