// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chess_GameMode.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "MoveWidgetEntry.generated.h"

UCLASS()
class CHESS_API UMoveWidgetEntry : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(meta =(BindWidget))
	UHorizontalBox* HorizontalBox;
	
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Number;
	
	UPROPERTY(meta =(BindWidget))
	UButton* Btn1;
	
	UPROPERTY(meta =(BindWidget))
	UTextBlock* TextLabel1;

	UPROPERTY(meta =(BindWidget))
	UButton* Btn2;

	UPROPERTY(meta =(BindWidget))
	UTextBlock* TextLabel2;

	bool CanClick = true;
	
	int NumberMove;

	void SetData(const FMove& Move, int32 FirstOrSecond);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AChess_GameMode* GameMode;

	FString TypeToChar(EPieceType Type) const;

	FString PositionToStringMove(FVector2D Position) const;

	FString IntToLetter(int32 Value) const;

	UFUNCTION()
	void OnBtnClick();
	
	void DelayedUndo();

};
