// Fill out your copyright notice in the Description page of Project Settings.

#include "MovesPanel.h"

#include "MoveWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMovesPanel::NativeConstruct()
{
	Super::NativeConstruct();

	// Here is where I typically bind delegates, and set up default appearance
	/*
	if (TitleLabel)
	{
		TitleLabel->SetText(FText::FromString(TEXT("Analysis of Moves")));
	}

	GenerateRandom();

	GenerateButton->OnClicked.AddUniqueDynamic(this, &UMovesPanel::OnGenerateButtonClicked);
	*/

	
	/*
	if(MoveWidgetRef)
	{
		UUserWidget* widget = CreateWidget(this, MoveWidgetRef);
		VerticalBox->AddChildToVerticalBox(widget);
		UMoveWidget* move = Cast<UMoveWidget>(widget);
		move->SetData(0);
	}
	*/
}

void UMovesPanel::AddMoveToPanel(const FMove& Move)
{
	if(MoveWidgetRef)
	{
		UUserWidget* widget = CreateWidget(this, MoveWidgetRef);
		VerticalBox->AddChild(widget);
		//ListOfMoves->AddItem(widget);
		UMoveWidget* move = Cast<UMoveWidget>(widget);
		move->SetData(Move);
	}
}

void UMovesPanel::PopFromPanel()
{
	if (VerticalBox->GetChildrenCount() > 0)
	{
		//UWidget* LastChild = VerticalBox->GetChildAt(VerticalBox->GetChildrenCount() - 1);
		VerticalBox->RemoveChildAt(VerticalBox->GetChildrenCount()-1);
		//LastChild->RemoveFromParent();
	}
}

void UMovesPanel::GenerateRandom()
{
	const int32 RandomNumber = FMath::RandRange(0,100);

	//TitleLabel->SetText(FText::AsNumber(RandomNumber));
}

void UMovesPanel::OnGenerateButtonClicked()
{
	GenerateRandom();
}