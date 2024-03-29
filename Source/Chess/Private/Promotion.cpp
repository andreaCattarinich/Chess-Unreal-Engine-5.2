// Fill out your copyright notice in the Description page of Project Settings.


#include "Promotion.h"

void UPromotion::OnButtonQueenClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Queen pressed"));
	if (AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::QUEEN);
	}
}

void UPromotion::OnButtonBishopClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Bishop pressed"));	
	if (AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::BISHOP);
	}
}

void UPromotion::OnButtonKnightClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Knight pressed"));
	if (AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()))
	{
		GameMode->SetPromotionChoice(EPieceType::KNIGHT);
	}
}

void UPromotion::OnButtonRookClick()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Rook pressed"));
	if (AChess_GameMode* GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode()))
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
