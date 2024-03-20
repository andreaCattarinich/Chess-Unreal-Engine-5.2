// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveWidget.h"

void UMoveWidget::SetData(const FMove& Move)
{
	MoveToGo = Move.NumberMove; // TODO: implementare sta roba (o variabile incrementale)
					// oppure aggiungere un membro a FMove che tiene conto della mossa

	const FString StartAsString =	FString::Printf(TEXT("(%d,%d)"), int(Move.Start.X), int(Move.Start.Y));
	const FString EndAsString   =	FString::Printf(TEXT("(%d,%d)"), int(Move.End.X), int(Move.End.Y));
	
	GameMode = Cast<ATTT_GameMode>(GetWorld()->GetAuthGameMode());

	APiece* StartPiece = *GameMode->GField->PieceMap.Find(Move.IDPiece);
	
	FString StringMove = "";
	FString Name = TypeToChar(StartPiece->GetPieceType());
	
	StringMove += Name;
	if (Move.IDPieceEaten != -1)
	{
		StringMove += "x";
	}

	StringMove += PositionToStringMove(Move.End);

	FString MoveToGoString = FString::FromInt(MoveToGo);
	MoveToGoString += ".";
	Number->SetText(FText::FromString(MoveToGoString));


	TextLabel->SetText(FText::FromString(StringMove));
	Btn->OnClicked.AddDynamic(this, &UMoveWidget::OnBtnClick);
}

FString UMoveWidget::TypeToChar(EPieceType Type) const
{
	switch (Type)
	{
	case EPieceType::PAWN:	return FString("");
	case EPieceType::ROOK:	return FString("R");
	case EPieceType::KNIGHT:return FString("N");
	case EPieceType::BISHOP:return FString("B");
	case EPieceType::QUEEN:	return FString("Q");
	case EPieceType::KING:	return FString("K");
	default:				return FString("Unknown");
	}
}

FString UMoveWidget::PositionToStringMove(FVector2D Position) const
{
	FString X = FString::FromInt(Position.X + 1);
	int32 IntPositionY = FMath::RoundToInt(Position.Y);
	FString Y = IntToLetter(IntPositionY);

	return Y + X;

}

FString UMoveWidget::IntToLetter(int32 Value) const
{
	FString Letter;
	switch (Value)
	{
	case 0:
		Letter = "a";
		break;
	case 1:
		Letter = "b";
		break;
	case 2:
		Letter = "c";
		break;
	case 3:
		Letter = "d";
		break;
	case 4:
		Letter = "e";
		break;
	case 5:
		Letter = "f";
		break;
	case 6:
		Letter = "g";
		break;
	case 7:
		Letter = "h";
		break;
	default:
		Letter = "Invalid";
		break;
	}
	return Letter;
}

void UMoveWidget::OnBtnClick()
{
	UE_LOG(LogTemp, Warning, TEXT("Move pressed: %d"), MoveToGo);

	int32 index;

	// Se è una mossa nera
	// => vado indietro fino alla mossa successiva bianca
	if (MoveToGo % 2 == 0)
	{
		index = MoveToGo + 1;
	}
	else
	{
		index = MoveToGo;
	}

	while (GameMode->Moves.Num() > index)
	{
		GameMode->UndoMove(true);
	}
}

