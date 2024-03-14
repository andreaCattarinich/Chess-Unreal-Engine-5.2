// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "ButtonWidget.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_UE52_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(meta = (BindWidget))
	UButton* Btn;

	//UPROPERTY(meta = (BindWidget))
	//UTextBlock* Text;

	int MoveToGo;

	void SetData(int value);

	UFUNCTION()
	void OnBtnClick();
};
