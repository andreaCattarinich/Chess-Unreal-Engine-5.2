// Copyright © 2024 Andrea Cattarinich

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
	// ************ ATTRIBUTES ************	
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

	// If the player can click on buttons
	bool CanClick = true;
	
	int32 NumberMove;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AChess_GameMode* GameMode;

	// ************ SETTERS ************	
	void SetData(const FMove& Move);



	// ************ GETTERS ************



	// ************ METHODS ************
	static FString TypeToChar(EPieceType Type);

	static FString PositionToStringMove(FVector2D Position);

	static FString IntToLetter(int32 Value);

	UFUNCTION()
	void OnBtnClick();
	
	void DelayedUndo();

};
