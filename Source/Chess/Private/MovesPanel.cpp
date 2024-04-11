// Copyright © 2024 Andrea Cattarinich

#include "MovesPanel.h"

#include "MoveWidgetEntry.h"
#include "Components/Button.h"
#include "Components/HorizontalBox.h"

void UMovesPanel::NativeConstruct()
{
	Super::NativeConstruct();
}

UScrollBox* UMovesPanel::GetScrollBox() const
{
	return ScrollBox;
}

void UMovesPanel::AddMoveToPanel(const FMove& Move)
{
	// If the move is by the white player
	// => I add new record on the panel
	// record is: [N. white black]
	if(Move.NumberMove % 2 == 1)
	{
		UUserWidget* Widget = CreateWidget(this, MoveWidgetEntryRef);
		ScrollBox->AddChild(Widget);
		
		UMoveWidgetEntry* Entry = Cast<UMoveWidgetEntry>(Widget);
		
		FString Number = FString::FromInt(ceil(Move.NumberMove / 2.0));;

		(ceil(Move.NumberMove / 2.0) < 10)
			? Number += ".  "
			: Number += ".";

		// N.
		Entry->Number->SetText(FText::FromString(Number));

		// white
		Entry->SetData(Move,0);

		// black (hidden)
		Entry->Btn2->SetVisibility(ESlateVisibility::Hidden);
	}
	// If the move is by the black player
	else
	{
		UWidget* LastChild = ScrollBox->GetChildAt(GetScrollBox()->GetChildrenCount() - 1);
		UMoveWidgetEntry* Entry = Cast<UMoveWidgetEntry>(LastChild);

		// black (visible)
		Entry->Btn2->SetVisibility(ESlateVisibility::Visible);

		// set
		Entry->SetData(Move,1);
	}
}

void UMovesPanel::PopFromPanel()
{
	// If scroll box has element(s)
	// => remove one from it
	if (ScrollBox->GetChildrenCount() > 0)
	{
		UWidget* LastChild = ScrollBox->GetChildAt(ScrollBox->GetChildrenCount() - 1);

		const UMoveWidgetEntry* Entry = Cast<UMoveWidgetEntry>(LastChild);

		if(Entry->Btn2->GetVisibility() == ESlateVisibility::Hidden)
		{
			LastChild->RemoveFromParent();
		}
		else
		{
			Entry->Btn2->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}