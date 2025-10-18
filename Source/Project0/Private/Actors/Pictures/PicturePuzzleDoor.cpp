// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pictures/PicturePuzzleDoor.h"

void APicturePuzzleDoor::OpenTheDoor()
{
	FRotator ChangeRot = GetActorRotation();
	ChangeRot.Yaw -= 90.f;
	SetActorRotation(ChangeRot);

	InteractSoundInit();
}
