// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameMode.h"
#include "Blueprint/UserWidget.h"

#include "Components/VerticalBox.h"
#include "Components/ScrollBox.h"
#include "MovesPanel.generated.h"

//class AChess_GameMode;


UCLASS(Abstract)
class CHESS_API UMovesPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;
	/****************************/

	UPROPERTY(meta = (BindWidget))
	UScrollBox* VerticalBox;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UMoveWidget> MoveWidgetRef;

	//UPROPERTY(meta = (BindWidget))
	//UListView* ListOfMoves;
	
public:
	UScrollBox* GetVerticalBox() const;
	void AddMoveToPanel(const FMove& Move);
	void PopFromPanel();
};
