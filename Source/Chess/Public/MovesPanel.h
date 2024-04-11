// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "CoreMinimal.h"
#include "Chess_GameMode.h"
#include "Blueprint/UserWidget.h"

#include "Components/ScrollBox.h"
#include "MovesPanel.generated.h"

UCLASS(Abstract)
class CHESS_API UMovesPanel : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	// ************ CONSTRUCTORS ************
	virtual void NativeConstruct() override;

	

	// ************ ATTRIBUTES ************	
	// List with scroll bar
	UPROPERTY(meta = (BindWidget))
		UScrollBox* ScrollBox;

	// Reference to the entry widget
	UPROPERTY(EditAnywhere)
		TSubclassOf<class UMoveWidgetEntry> MoveWidgetEntryRef;
	
	// ************ SETTERS ************	


	
	// ************ GETTERS ************	
public:
	UScrollBox* GetScrollBox() const;

	

	// ************ METHODS ************
	void AddMoveToPanel(const FMove& Move);
	
	void PopFromPanel();
};
