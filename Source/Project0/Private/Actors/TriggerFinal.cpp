// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/TriggerFinal.h"
#include "Components/BoxComponent.h"
#include "LevelSequence.h"
#include "LevelSequencePlayer.h"
#include "LevelSequenceActor.h"
#include "MovieSceneSequencePlayer.h"


ATriggerFinal::ATriggerFinal()
{
	PrimaryActorTick.bCanEverTick = false;

	Box = CreateDefaultSubobject<UBoxComponent>(TEXT("Box"));
	SetRootComponent(Box);
	Box->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Box->SetCollisionResponseToAllChannels(ECR_Ignore);
	Box->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Box->OnComponentBeginOverlap.AddDynamic(this, &ATriggerFinal::OnBeginOverlap);
}

void ATriggerFinal::SetEnabled(bool bInEnabled)
{
	bEnabled = bInEnabled;
}

void ATriggerFinal::OnBeginOverlap(UPrimitiveComponent*, AActor* Other,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (!bEnabled || !Other) return;
	OnPromptRequested.Broadcast(this); //퀘스트 시스템이 받아서 선택지 UI 띄움

	
}

void ATriggerFinal::PlayChosen(EFinalPick Pick)
{
	switch (Pick)
	{
	case EFinalPick::PickA: PlayInternal(SequenceA, Pick); break;
	case EFinalPick::PickB: PlayInternal(SequenceB, Pick); break;
	default: break;
	}
}

void ATriggerFinal::PlayInternal(TSoftObjectPtr<ULevelSequence> Asset, EFinalPick Pick)
{
	if (Asset.IsNull()) return;
	if (!Asset.IsValid()) Asset.LoadSynchronous();

	if (ULevelSequence* Seq = Asset.Get())
	{
		FMovieSceneSequencePlaybackSettings Settings;
		Player = ULevelSequencePlayer::CreateLevelSequencePlayer(GetWorld(), Seq, Settings, SpawnedActor);

		if (Player)
		{
			LastPick = Pick;
			OnSequencePlayed.Broadcast(this, Pick);
			Player->Play();
			bEnabled = false;
		}
	}
}
