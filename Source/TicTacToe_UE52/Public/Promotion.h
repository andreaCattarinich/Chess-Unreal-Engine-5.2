// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Chess_GameMode.h"

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Promotion.generated.h"

UCLASS(Abstract)
class UPromotion : public UUserWidget
{
	GENERATED_BODY()


protected:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonQueen;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonBishop;
	
	UPROPERTY(meta = (BindWidget))
	UButton* ButtonKnight;

	UPROPERTY(meta = (BindWidget))
	UButton* ButtonRook;
	
	UFUNCTION()
	void OnButtonQueenClick();
		
	UFUNCTION()
	void OnButtonBishopClick();


	UFUNCTION()
	void OnButtonKnightClick();

	UFUNCTION()
	void OnButtonRookClick();

};
