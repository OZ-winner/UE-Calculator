// Fill out your copyright notice in the Description page of Project Settings.


#include "Player/Calculator_PlayerController.h"

ACalculator_PlayerController::ACalculator_PlayerController(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	
}

void ACalculator_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	// 没设置控件类就直接退出，避免 CreateWidget 拿到空类。
	// 这个值要在 PlayerController 的蓝图子类里填（EditDefaultsOnly），
	// 而且必须选蓝图子类（类名带 _C、资源在 Content/Calculator/Widget 里），
	// 不能选同名的 C++ 类本身——它没有控件树。
	if (!CalculatorHUD)
	{
		UE_LOG(LogTemp, Error,
			TEXT("[Calculator] CalculatorHUD 未设置：打开 PlayerController 蓝图，在类默认值里把 Calculator HUD 选成 UW_Calculator。"));
		return;
	}

	UUW_Calculator* MyCalculatorHUD = CreateWidget<UUW_Calculator>(this,CalculatorHUD,TEXT("CalculatorHUD"));

	// 原来这里判的是 MyHUD —— PlayerController 自带的 HUD 指针，跟刚创建的控件不是一回事。
	// 控件创建失败时 MyHUD 可能非空，于是照样往下走，在 AddToViewport 里解引用空指针。
	if (!MyCalculatorHUD)
	{
		UE_LOG(LogTemp, Error, TEXT("[Calculator] CreateWidget 失败，控件没有创建出来。"));
		return;
	}

	MyCalculatorHUD->AddToViewport(0);
}
