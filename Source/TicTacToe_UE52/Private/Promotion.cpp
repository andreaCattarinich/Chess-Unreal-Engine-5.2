// Fill out your copyright notice in the Description page of Project Settings.


#include "Promotion.h"

void UPromotion::OnButtonQueenClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("QUEEN PRESSED"));
	if (ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(1);
	}
}

void UPromotion::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonQueen->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonQueenClick);
}
