// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "vector"
#include "UW_Calculator.generated.h"
class UButton;
class UTextBlock;
/**
 * 
 */
UCLASS()
class CALCULATOR_API UUW_Calculator : public UUserWidget
{
	GENERATED_BODY()
protected:
	UUW_Calculator(const FObjectInitializer& ObjectInitializer);
	virtual  void NativeOnInitialized() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_1;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_2;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_3;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_4;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_5;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_6;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_7;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_8;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_9;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_0;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_point;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Sum;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Sub;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Mult;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Div;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Equal;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Delete;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Clear;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Open;
	UPROPERTY(meta=(BindWidget))
	TObjectPtr<UButton>Button_Close;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock>Text_Expression;
	UPROPERTY(BlueprintReadOnly, meta=(BindWidget))
	TObjectPtr<UTextBlock>Text_Result;
	UFUNCTION()
	FText SetText_Expression();
	UFUNCTION()
	FText SetText_Result();
	// ==================== 按钮点击事件 ====================

	// 数字键 0-9
	UFUNCTION()
	void OnButton0Clicked();
	UFUNCTION()
	void OnButton1Clicked();
	UFUNCTION()
	void OnButton2Clicked();
	UFUNCTION()
	void OnButton3Clicked();
	UFUNCTION()
	void OnButton4Clicked();
	UFUNCTION()
	void OnButton5Clicked();
	UFUNCTION()
	void OnButton6Clicked();
	UFUNCTION()
	void OnButton7Clicked();
	UFUNCTION()
	void OnButton8Clicked();
	UFUNCTION()
	void OnButton9Clicked();

	// 小数点
	UFUNCTION()
	void OnButtonPointClicked();

	// 四则运算
	UFUNCTION()
	void OnButtonSumClicked();
	UFUNCTION()
	void OnButtonSubClicked();
	UFUNCTION()
	void OnButtonMultClicked();
	UFUNCTION()
	void OnButtonDivClicked();

	// 等号
	UFUNCTION()
	void OnButtonEqualClicked();

	// 退格 / 清零
	UFUNCTION()
	void OnButtonDeleteClicked();
	UFUNCTION()
	void OnButtonClearClicked();

	// 左右括号
	UFUNCTION()
	void OnButtonOpenClicked();
	UFUNCTION()
	void OnButtonCloseClicked();

private:
	//当前输入的中缀表达式
	FString M_format;
	//转换后的后缀表达式，每个元素是一个操作数或一个运算符
	std::vector<FString> B_format;
	//结果行显示的文本
	FString result;
	//把字符追加进表达式
	void AddElement(char element);
	//求值用的数字栈
	std::vector<double>NumberStack;
	//中缀转后缀时暂存运算符的栈
	std::vector<char>SignStack;
	void PushStack(double number);
	void PushStack(char sign);
	double CharToNumber(FString Temp_Number);
	//在结果行显示错误提示
	void Error();
	bool compareSign(char A,char B);
	//中缀转后缀，失败返回 false
	bool MToB();
	//对后缀表达式求值
	void Calculate();
	
};

