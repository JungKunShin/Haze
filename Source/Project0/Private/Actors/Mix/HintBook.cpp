// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Mix/HintBook.h"
#include "ActorComponents/PlayerMixComponent.h"

AHintBook::AHintBook()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	MixComp = CreateDefaultSubobject<UPlayerMixComponent>(TEXT("MixComp"));
}
