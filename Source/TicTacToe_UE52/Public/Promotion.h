// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "TTT_GameMode.h"

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

	
	UFUNCTION()
		void OnButtonQueenClick();
};
