// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveWidget.h"

void UMoveWidget::SetData(const FMove& Move)
{
	MoveToGo = 1; // TODO: implementare sta roba (o variabile incrementale)
					// oppure aggiungere un membro a FMove che tiene conto della mossa


	const FString StartAsString =	FString::Printf(TEXT("(%d,%d)"), int(Move.Start.X), int(Move.Start.Y));
	const FString EndAsString   =	FString::Printf(TEXT("(%d,%d)"), int(Move.End.X), int(Move.End.Y));
	
	
	TextLabel->SetText(FText::FromString(StartAsString + EndAsString));
	Btn->OnClicked.AddDynamic(this, &UMoveWidget::OnBtnClick);
}

void UMoveWidget::OnBtnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Move pressed: %d"), MoveToGo);
}

