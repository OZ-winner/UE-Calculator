// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "UserWidget/UW_Calculator.h"
#include "Calculator_PlayerController.generated.h"

/**
 * 
 */
UCLASS()
class CALCULATOR_API ACalculator_PlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	ACalculator_PlayerController(const FObjectInitializer& ObjectInitializer);
protected:
	virtual void BeginPlay() override;
	UPROPERTY(EditDefaultsOnly,Category="Calculator|UI")
	TSubclassOf<UUW_Calculator>CalculatorHUD;
};
