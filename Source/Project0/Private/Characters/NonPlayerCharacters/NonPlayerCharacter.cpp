// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/NonPlayerCharacters/NonPlayerCharacter.h"

void ANonPlayerCharacter::ResetPatrolPathIndex()
{
	PatrolPathIndex = 0;
}

FVector ANonPlayerCharacter::GetNextPatrolPath()
{
	if (PatrolPaths.IsEmpty()) return GetActorLocation();

	PatrolPathIndex = ++PatrolPathIndex % PatrolPaths.Num();
	return PatrolPaths[PatrolPathIndex]->GetActorLocation();
}
