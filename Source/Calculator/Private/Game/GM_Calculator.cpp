// Fill out your copyright notice in the Description page of Project Settings.


#include "Game/GM_Calculator.h"

AGM_Calculator::AGM_Calculator():Super()
{
}

void AGM_Calculator::BeginPlay()
{
	Super::BeginPlay();
	if (!GEngine || !GEngine->GameViewport) return;
	GEngine->GameViewport->bDisableWorldRendering = true;
}
