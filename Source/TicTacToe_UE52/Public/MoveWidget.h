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

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Number;

	UPROPERTY(meta =(BindWidget))
	UTextBlock* TextLabel;

	int NumberMove;

	void SetData(const FMove& Move);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ATTT_GameMode* GameMode;

	FString TypeToChar(EPieceType Type) const;

	FString PositionToStringMove(FVector2D Position) const;

	FString IntToLetter(int32 Value) const;

	UFUNCTION()
	void OnBtnClick();
};
