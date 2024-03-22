// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Promotion.generated.h"

UCLASS(Abstract)
class UPromotion : public UUserWidget
{
	GENERATED_BODY()
protected:
	virtual void NativeConstruct() override;
};
