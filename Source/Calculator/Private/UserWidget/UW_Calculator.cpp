// Fill out your copyright notice in the Description page of Project Settings.


#include "UserWidget/UW_Calculator.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"


UUW_Calculator::UUW_Calculator(const FObjectInitializer& ObjectInitializer):Super(ObjectInitializer)
{
	bHasScriptImplementedTick = false;
}

void UUW_Calculator::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	//把两个显示框的文本绑定到 C++ 回调，之后由引擎按需取值刷新
	//表达式行
	if (!Text_Expression)return;
	Text_Expression->TextDelegate.BindDynamic(this,&UUW_Calculator::SetText_Expression);
	//结果行
	if (!Text_Result)return;
	Text_Result->TextDelegate.BindDynamic(this,&UUW_Calculator::SetText_Result);
}

void UUW_Calculator::NativeConstruct()
{
	Super::NativeConstruct();
	//绑定前先确认控件树已就绪：BindWidget 指针来自蓝图控件树，
	//控件树为空时这些指针全是 nullptr，直接解引用会崩溃
	if (!Button_0 || !Button_9 || !Button_point || !Button_Clear || !Text_Expression || !Text_Result)
	{
		UE_LOG(LogTemp, Error,
			TEXT("[Calculator] 控件树为空，按钮绑定失败。当前控件类：%s。创建控件时要用蓝图子类（类名带 _C 的那个），不能用 C++ 类本身——它没有控件树。"),
			*GetClass()->GetName());
		return;
	}

	// ---------------- 数字键 0-9 ----------------
	Button_0->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton0Clicked);
	Button_1->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton1Clicked);
	Button_2->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton2Clicked);
	Button_3->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton3Clicked);
	Button_4->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton4Clicked);
	Button_5->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton5Clicked);
	Button_6->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton6Clicked);
	Button_7->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton7Clicked);
	Button_8->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton8Clicked);
	Button_9->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButton9Clicked);

	// ---------------- 小数点 ----------------
	Button_point->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonPointClicked);

	// ---------------- 四则运算 ----------------
	Button_Sum->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonSumClicked);
	Button_Sub->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonSubClicked);
	Button_Mult->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonMultClicked);
	Button_Div->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonDivClicked);

	// ---------------- 等号 ----------------
	Button_Equal->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonEqualClicked);

	// ---------------- 退格 / 清零 ----------------
	Button_Delete->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonDeleteClicked);
	Button_Clear->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonClearClicked);

	// ---------------- 左右括号 ----------------
	//这两个按钮单独判空，缺失时只跳过它们自己，不影响其他按键的绑定
	if (Button_Open && Button_Close)
	{
		Button_Open->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonOpenClicked);
		Button_Close->OnClicked.AddUniqueDynamic(this, &UUW_Calculator::OnButtonCloseClicked);
	}
	else
	{
		UE_LOG(LogTemp, Warning,
			TEXT("[Calculator] 缺少 Button_Open / Button_Close，括号键不可用。请在编辑器里重新编译 BP_UW_Calculator。"));
	}
}

void UUW_Calculator::NativeDestruct()
{
	Super::NativeDestruct();
}

// ==================== 按钮点击事件 ====================
// 输入类按键统一走 AddElement，把字符攒进 format。

//表达式行的取值回调
FText UUW_Calculator::SetText_Expression()
{
	if (!Text_Expression)return FText::FromString(TEXT("显示框异常"));
	return FText::FromString(M_format);
}

//结果行的取值回调
FText UUW_Calculator::SetText_Result()
{
	if (!Text_Expression)return FText::FromString(TEXT("显示框异常"));
	return FText::FromString(result);
}

void UUW_Calculator::OnButton0Clicked()
{
	AddElement('0');
}

void UUW_Calculator::OnButton1Clicked()
{
	AddElement('1');
}

void UUW_Calculator::OnButton2Clicked()
{
	AddElement('2');
}

void UUW_Calculator::OnButton3Clicked()
{
	AddElement('3');
}

void UUW_Calculator::OnButton4Clicked()
{
	AddElement('4');
}

void UUW_Calculator::OnButton5Clicked()
{
	AddElement('5');
}

void UUW_Calculator::OnButton6Clicked()
{
	AddElement('6');
}

void UUW_Calculator::OnButton7Clicked()
{
	AddElement('7');
}

void UUW_Calculator::OnButton8Clicked()
{
	AddElement('8');
}

void UUW_Calculator::OnButton9Clicked()
{
	AddElement('9');
}

void UUW_Calculator::OnButtonPointClicked()
{
	AddElement('.');
}

void UUW_Calculator::OnButtonSumClicked()
{
	AddElement('+');
}

void UUW_Calculator::OnButtonSubClicked()
{
	AddElement('-');
}

void UUW_Calculator::OnButtonMultClicked()
{
	AddElement('*');
}

void UUW_Calculator::OnButtonDivClicked()
{
	AddElement('/');
}

void UUW_Calculator::OnButtonEqualClicked()
{
	//中缀转后缀，失败时 Error 已经写好提示，直接结束
	if (!MToB())return;
	//对后缀表达式求值
	Calculate();
}

void UUW_Calculator::OnButtonDeleteClicked()
{
	//退格：删掉表达式最后一个字符，旧结果一并清空
	if (M_format.IsEmpty())return;
	M_format.RemoveAt(M_format.Len()-1);
	result.Empty();
}

void UUW_Calculator::OnButtonClearClicked()
{
	//清零：表达式和结果同时清空
	if (M_format.IsEmpty())return;
	M_format.Empty();
	result.Empty();
}

void UUW_Calculator::OnButtonOpenClicked()
{
	AddElement('(');
}

void UUW_Calculator::OnButtonCloseClicked()
{
	AddElement(')');
}

void UUW_Calculator::AddElement(char element)
{
	if (!element)return;
	//继续输入说明上一轮计算结束，旧结果清空，只保留表达式
	result.Empty();
	M_format.AppendChar(element);
}

//数字进栈
void UUW_Calculator::PushStack(double number)
{
	NumberStack.push_back(number);
}

//符号进栈
void UUW_Calculator::PushStack(char sign)
{
	SignStack.push_back(sign);
}

//数字串转 double，Atod 支持前导正负号
double UUW_Calculator::CharToNumber(FString Temp_Number)
{
	 return FCString::Atod(*Temp_Number);
}

//在结果行显示统一的错误提示
void UUW_Calculator::Error()
{
	result =TEXT("表达式非法");
}

//比较两个运算符的优先级，A 高于 B 返回 true
bool UUW_Calculator::compareSign(char A, char B)
{
	//返回优先级：加减为 1，乘除为 2
	//括号不参与比较，给 0 保证任何运算符都能压到它上面
	auto Func=[](char ch)
	{
		if (ch=='(')return 0;
		if (ch=='-'||ch=='+')return 1;
		if (ch=='*'||ch=='/')return 2;
		else
		{
			UE_LOG(LogTemp,Error,TEXT("[Calculator] compareSign 收到非运算符字符"))
			return 0;
		}
	};
	int a =Func(A);
	int b =Func(B);
	return a>b?true:false;
}

//中缀表达式转后缀表达式（调度场算法），成功返回 true，失败时已调用 Error
bool UUW_Calculator::MToB()
{
	//Temp_Number 暂存正在读取的操作数，B_format 存后缀结果，SignStack 暂存运算符
	FString Temp_Number;
	B_format.clear();
	SignStack.clear();
	if (M_format.IsEmpty())return false;

	//把 Temp_Number 里攒好的操作数作为一个完整 token 推送进后缀数组
	auto FlushNumber = [&]()
	{
		if (!Temp_Number.IsEmpty())
		{
			B_format.push_back(Temp_Number);
			Temp_Number.Empty();
		}
	};

	//pre 记录上一个字符，'a' 表示当前是第一个字符
	char pre = 'a';
	//末尾不能是运算符、小数点或左括号，否则表达式不完整
	if (M_format[M_format.Len()-1]=='+'||
		M_format[M_format.Len()-1]=='-'||
		M_format[M_format.Len()-1]=='*'||
		M_format[M_format.Len()-1]=='/'||
		M_format[M_format.Len()-1]=='.'||
		M_format[M_format.Len()-1]=='(')
	{
		Error();
		return false;
	}
	for (char ch:M_format)
	{
		//上一轮刚吃进一个符号位，这一位必须是数字，否则这个负号没有操作数
		//不放开小数点，所以 "-.5" 这种写法不合法，必须写成 "-0.5"
		if (pre=='u'&&!FChar::IsDigit(ch))
		{
			Error();
			return false;
		}
		//合法性检查：开头不能是 + * / .
		if (pre=='a'&&(ch == '+'||ch == '*'||ch == '/'||ch=='.'))
		{
			Error();
			return false;
		}
		//一元负号只允许出现在两个位置：表达式开头、左括号之后
		//这里的负号是符号位而不是减号，并入操作数，形成 "-5" 这种带符号的数
		//运算符后面的负号不放行，"1*-2" 这类仍然非法
		const bool bOperandExpected = (pre=='a'||pre=='(');
		if (ch=='-'&&bOperandExpected)
		{
			Temp_Number.AppendChar(ch);
			pre = 'u';//标记已消费一个符号位，防止连续负号拼成 "--5"
			continue;
		}
		//符号后面不能紧跟符号
		if ((pre == '+'||pre == '-'||pre == '*'||pre == '/'||pre=='.')&&(ch == '+'||ch == '-'||ch == '*'||ch == '/'||ch=='.'))
		{
				Error();
				return false;
		}
		//左括号内不能为空，且 ( 后面不能紧跟 ) + * / .
		if (pre=='('&&(ch==')'||ch=='+'||ch == '*'||ch == '/'||ch=='.'))
		{
				Error();
				return false;
		}
		pre = ch;//记录上一个字符
		//遇到操作数先攒起来，等遇到运算符或括号时再整体提交
		if (ch!='+'&&(ch!='-'&&ch!='*'&&ch!='/'&&ch!='('&&ch!=')'))
		{
			Temp_Number.AppendChar(ch);
			continue;
		}
		//到这里 ch 是运算符或括号，左边的操作数已经完整，先提交
		FlushNumber();
		//不是右括号，按优先级规则入栈或弹出栈顶
		if (ch!=')')
		{
				while (1)
				{
					//栈为空时直接入栈，同时避免对空栈取 back
					if (SignStack.empty())
					{
						PushStack(ch);
						break;
					}
					//左括号无条件压栈
					if (ch=='(')
					{
						PushStack(ch);
						break;
					}
					char temp=SignStack.back();
					//新符号优先级高于栈顶，入栈
					if (compareSign(ch,temp))
					{
						PushStack(ch);
						break;
					}
					//否则把栈顶弹到后缀数组，继续和下一个栈顶比较
					else
					{
						B_format.push_back(FString::Chr(temp));
						SignStack.pop_back();
					}
				}
		}
		//遇到右括号，弹出栈顶直到遇到左括号
		if (ch ==')')
		{
			while (1)
			{
				//栈空了还没遇到左括号，说明括号不配对
				if (SignStack.empty())
				{
					Error();
					return false;
				}
				//找到左括号，配对完成，丢弃它并结束
				if (SignStack.back()=='(')
				{
					SignStack.pop_back();
					break;
				}
				
				B_format.push_back(FString::Chr(SignStack.back()));
				SignStack.pop_back();
			}
		}
	}
	//表达式末尾的操作数没有运算符来触发提交，这里补一次
	FlushNumber();
	//栈已空说明转换完成
	if (SignStack.empty())return true;
	//把栈里剩余的运算符依次追加到后缀数组
	for (auto it = SignStack.rbegin(); it != SignStack.rend(); ++it)
	{
		//还有左括号没闭合，表达式非法
		if (*it=='(')
		{
			Error();
			return false;
		}
		B_format.push_back(FString::Chr(*it));
	}
	return true;
}

//对后缀表达式求值，结果写入 result
void UUW_Calculator::Calculate()
{
	//按运算符做一次二元运算，left 在左、right 在右
	auto calculate=[](double right,double left,const FString& sign)
	{
		if (sign=="+")return left+right;
		if (sign=="-")return left-right;
		if (sign=="*")return left*right;
		if (sign=="/")return left/right;
		UE_LOG(LogTemp,Error,TEXT("[Calculator] calculate 收到非法运算符"));
		return 0.0;
	};
	//清空上一轮留下的数字栈
	NumberStack.clear();
	for (auto it = B_format.begin();it!=B_format.end();++it)
	{
		//后缀数组里只有两类 token：操作数（可带一个小数点或前导负号）和单字符运算符
		//取首字符，空 token 按 '\0' 处理
		const TCHAR FirstChar = (*it).IsEmpty() ? TEXT('\0') : (*it)[0];
		//首字符是正负号就跳过它，从下一位开始判断类型
		const int32 ClassIndex = (FirstChar == TEXT('+') || FirstChar == TEXT('-')) ? 1 : 0;
		//跳过之后取不到字符，说明这是单独的运算符（如 "-"），按运算符处理
		const TCHAR ClassChar = (*it).Len() > ClassIndex ? (*it)[ClassIndex] : TEXT('\0');
		const bool bIsOperand = FChar::IsDigit(ClassChar) || ClassChar == TEXT('.');

		//遇到操作数：先检查小数点个数，再压入栈
		if (bIsOperand)
		{
			//一个数最多一个小数点，多了就是非法输入
			int32 DotCount = 0;
			for (int32 Index = 0; Index < it->Len(); ++Index)
			{
				if ((*it)[Index] == TEXT('.'))
				{
					++DotCount;
				}
			}
			if (DotCount > 1)
			{
				Error();
				return;
			}
			PushStack(CharToNumber(*it));
		}
		//遇到运算符，弹出两个数运算再压回栈
		else
		{
			//栈里不够两个操作数，说明后缀表达式非法
			if (NumberStack.size() < 2)
			{
				Error();
				return;
			}
			//先弹出的是右操作数，后弹出的是左操作数
			double right = NumberStack.back();
			NumberStack.pop_back();
			double left = NumberStack.back();
			NumberStack.pop_back();
			//除数为 0 直接报错
			if (right==0&&*it=="/")
			{
				Error();
				return ;
			}
			PushStack(calculate(right,left,*it));
		}
	}
	//算完栈里必须恰好剩一个数，否则说明操作数和运算符数量对不上
	if (NumberStack.size() != 1)
	{
		Error();
		return;
	}
	//保留 10 位有效数字输出，兼顾精度和浮点舍入
	result=FString::Printf(TEXT("=%.10g"),NumberStack[0]);
}




