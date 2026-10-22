// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GM_Calculator.generated.h"

/**
 * 
 */
UCLASS()
class CALCULATOR_API AGM_Calculator : public AGameModeBase
{
	GENERATED_BODY()
	AGM_Calculator();
	virtual  void BeginPlay() override;
};
