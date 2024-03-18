// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TTT_GameMode.h"
#include "Blueprint/UserWidget.h"
#include "Components/ListView.h"
#include "Components/VerticalBox.h"
#include "MovesPanel.generated.h"

//class ATTT_GameMode;


UCLASS(Abstract)
class TICTACTOE_UE52_API UMovesPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	/****************************/

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveWidget> MoveWidgetRef;

	UPROPERTY(meta = (BindWidget))
	UListView* ListOfMoves;

public:
	void AddMoveToPanel(const FMove& Move);
	void PopFromPanel();
	
protected:

	/****************************/
	void GenerateRandom();
	
	//UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	//class UTextBlock* TitleLabel;

	//UPROPERTY(meta = (BindWidget))
	//class UButton* GenerateButton;

	UFUNCTION()
	void OnGenerateButtonClicked();
};