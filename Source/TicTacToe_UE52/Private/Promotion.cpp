// Fill out your copyright notice in the Description page of Project Settings.


#include "Promotion.h"

void UPromotion::OnButtonQueenClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("QUEEN PRESSED"));
	if (ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::QUEEN);
	}
}

void UPromotion::OnButtonBishopClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("BISHOP PRESSED"));	
	if (ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::BISHOP);
	}
}

void UPromotion::OnButtonKnightClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("KNIGHT PRESSED"));
	if (ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::KNIGHT);
	}
}

void UPromotion::OnButtonRookClick()
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("ROOK PRESSED"));
	if (ATTT_GameMode* GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::ROOK);
	}
}

void UPromotion::NativeConstruct()
{
	Super::NativeConstruct();
	ButtonQueen->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonQueenClick);
	ButtonBishop->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonBishopClick);
	ButtonKnight->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonKnightClick);
	ButtonRook->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonRookClick);
}
