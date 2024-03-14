// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/VerticalBox.h"
#include "ListOfMovesWidget.generated.h"

/**
 * 
 */
UCLASS()
class TICTACTOE_UE52_API UListOfMovesWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* VerticalBox;

	UPROPERTY(EditAnywhere)
		TSubclassOf<class UButtonWidget> ButtonWidgetRef;
	
};
