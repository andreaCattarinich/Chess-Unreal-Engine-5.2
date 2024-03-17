// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TTT_GameMode.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "MoveWidget.generated.h"

UCLASS()
class TICTACTOE_UE52_API UMoveWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta =(BindWidget))
	UButton* Btn;

	UPROPERTY(meta =(BindWidget))
	UTextBlock* TextLabel;

	int MoveToGo;

	void SetData(const FMove& Move);

	UFUNCTION()
	void OnBtnClick();
};