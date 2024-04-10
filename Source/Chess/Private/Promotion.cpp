// Copyright © 2024 Andrea Cattarinich

#include "Promotion.h"

void UPromotion::OnButtonQueenClick()
{
	GameMode->SetPromotionChoice(EPieceType::QUEEN);
}

void UPromotion::OnButtonBishopClick()
{
	GameMode->SetPromotionChoice(EPieceType::BISHOP);
}

void UPromotion::OnButtonKnightClick()
{
	GameMode->SetPromotionChoice(EPieceType::KNIGHT);
}

void UPromotion::OnButtonRookClick()
{
	GameMode->SetPromotionChoice(EPieceType::ROOK);
}

void UPromotion::NativeConstruct()
{
	Super::NativeConstruct();

	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());
	
	ButtonQueen->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonQueenClick);
	ButtonBishop->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonBishopClick);
	ButtonKnight->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonKnightClick);
	ButtonRook->OnClicked.AddUniqueDynamic(this, &UPromotion::OnButtonRookClick);
}