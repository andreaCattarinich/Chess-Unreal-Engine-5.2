// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonWidget.h"

void UButtonWidget::SetData(int value)
{
	MoveToGo = value;
	//Text->SetText(FText::FromString(FString::FromInt(value)));
	Btn->OnClicked.AddDynamic(this, &UButtonWidget::OnBtnClick);
}

void UButtonWidget::OnBtnClick()
{
	//UE_LOG(LogTemp, Warning, TEXT("Number is %d"), LevelToGo);
}

