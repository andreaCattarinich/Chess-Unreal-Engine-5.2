// Fill out your copyright notice in the Description page of Project Settings.

#include "MovesPanel.h"

#include "MoveWidget.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UMovesPanel::NativeConstruct()
{
	Super::NativeConstruct();

}

UScrollBox* UMovesPanel::GetVerticalBox() const
{
	return VerticalBox;
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