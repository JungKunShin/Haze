// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Pictures/WinterPicture.h"

AWinterPicture::AWinterPicture()
{
	PictureRotation = 270.f; // 정답 각도
	ResetCurPitch = 90;		 // 리셋 시 초기화 각도
	CurPitch = 90;			 // 현재 각도
}
