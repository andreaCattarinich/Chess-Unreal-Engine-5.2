// Fill out your copyright notice in the Description page of Project Settings.


#include "ListOfMovesWidget.h"
#include "ButtonWidget.h"

void UListOfMovesWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (ButtonWidgetRef)
	{
		UUserWidget* widget = CreateWidget(this, ButtonWidgetRef);
		VerticalBox->AddChildToVerticalBox(widget);
		UButtonWidget* button = Cast<UButtonWidget>(widget);
		button->SetData(8);
	}
}
