// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveWidget.h"

void UMoveWidget::SetData(const FMove& Move)
{
	NumberMove = Move.NumberMove;
	
	GameMode = Cast<AChess_GameMode>(GetWorld()->GetAuthGameMode());

	const APiece* StartPiece = *GameMode->GField->PieceMap.Find(Move.IDPiece);
	
	FString StringMove = "";
	FString Name = TypeToChar(StartPiece->GetPieceType());
	StringMove += Name;

	EPieceType PieceTypes[] = { EPieceType::ROOK, EPieceType::KNIGHT, EPieceType::BISHOP };

	for (const EPieceType& CurrentPieceType : PieceTypes)
	{
	    if (StartPiece->GetPieceType() == CurrentPieceType)
	    {
	        const APiece* OtherPiece = nullptr;
	        for (const auto& CurrentTile : GameMode->GField->TileArray)
	        {
	            const APiece* CurrentPiece = CurrentTile->GetPiece();
	            const int32 CurrOwner = (*GameMode->GField->TileMap.Find(Move.End))->GetTileOwner();
	            
	            if (CurrentTile->GetTileOwner() == CurrOwner &&
	                CurrentPiece->GetPieceType() == CurrentPieceType &&
	                CurrentPiece->GetPieceID() != Move.IDPiece)
	            {
	                OtherPiece = CurrentPiece;
	                break;
	            }
	        }
	        
	        if (OtherPiece)
	        {
	            const FMove LastMove = GameMode->Moves.Pop();
	            GameMode->Moves.Add(LastMove);
	            
	            GameMode->UndoMove();
	            
	            GameMode->GField->SetSelectedTile(OtherPiece->GetGridPosition());
	            const TArray<FVector2d> LegalMovesOfOtherPiece = GameMode->GField->LegalMoves(OtherPiece->GetGridPosition());
	            //GameMode->GField->ResetGameStatusField();

	            GameMode->GField->SetSelectedTile(LastMove.Start);
	            GameMode->DoMove(LastMove.End);
	           //GameMode->GField->ResetGameStatusField();

	            if (LegalMovesOfOtherPiece.Contains(Move.End))
	            {
	                if (Move.Start.Y == OtherPiece->GetGridPosition().Y)
	                {
	                    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CASO SPECIALE 2"));
	                    StringMove += FString::FromInt(Move.Start.X + 1);
	                }
	                else
	                {
	                    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("CASO SPECIALE 1"));
	                    StringMove += IntToLetter(static_cast<int>(Move.Start.Y));	
	                }
	            }
	        }
	        break;
	    }
	}
	
	if (Move.IDPieceEaten != -1)
	{
		if(StartPiece->GetPieceType() == EPieceType::PAWN)
		{
			StringMove += IntToLetter(static_cast<int>(Move.Start.Y));
			StringMove += "x";
		}
		else
		{
			StringMove += "x";
		}
	}

	StringMove += PositionToStringMove(Move.End);
	/*
	int32 Player = (*GameMode->GField->TileMap.Find(Move.End))->GetTileOwner();

	if(GameMode->IsWinMove(Player))
	{
		StringMove += "#";
	}
	*/
	bool bIsPatta = true;
	const int32 Player = (*GameMode->GField->TileMap.Find(Move.End))->GetTileOwner();
	for(const auto& CurrentTile : GameMode->GField->TileArray)
	{
		if(CurrentTile->GetTileOwner() == Player)
		{
			GameMode->GField->SetSelectedTile(CurrentTile->GetGridPosition());

			TArray<FVector2D> CurrentLegalMoves;
			
			//GameMode->CurrentPlayer = GameMode->GetNextPlayer(GameMode->CurrentPlayer);
			CurrentLegalMoves = GameMode->GField->LegalMoves(CurrentTile->GetGridPosition());
			//GameMode->CurrentPlayer = GameMode->GetNextPlayer(GameMode->CurrentPlayer);
			
			if(CurrentLegalMoves.Contains(GameMode->GField->GetKingPosition((GameMode->GetNextPlayer(GameMode->CurrentPlayer)))))
			{
				
				if(GameMode->IsWinnerMove(Player, false))
				{
					bIsPatta = false;
					StringMove += "#";
					if(Player == 0)
					{
						StringMove += " 1-0";
					}
					else
					{
						StringMove += " 0-1";
					}
				}
				else
				{
					StringMove += "+";
				}
			}
		}
	}

	if(GameMode->IsWinnerMove(Player, false) && bIsPatta)
	{
		StringMove += " 1/2-1/2";
	}




	
	
	FString NumberMoveString = "";
	if(NumberMove % 2 == 1)
	{
		NumberMoveString = FString::FromInt(ceil(static_cast<float>(NumberMove)/2));
		NumberMoveString += ".";
	}
	else
	{
		NumberMoveString += "   ";
	}
	
	Number->SetText(FText::FromString(NumberMoveString));


	TextLabel->SetText(FText::FromString(StringMove));
	Btn->OnClicked.AddDynamic(this, &UMoveWidget::OnBtnClick);
}

FString UMoveWidget::TypeToChar(const EPieceType Type) const
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

FString UMoveWidget::IntToLetter(const int32 Value) const
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
	// Black must do the move. After then you can reset the field
	if(GameMode->Moves.Num() % 2 == 1)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Move pressed: %d"), NumberMove);
	int32 index;

	// Se è una mossa nera
	// => vado indietro fino alla mossa successiva bianca
	if (NumberMove % 2 == 0)
	{
		index = NumberMove;
	}
	else
	{
		index = NumberMove + 1;
	}

	while (GameMode->Moves.Num() > index)
	{
		GameMode->GField->ResetGameStatusField();
		GameMode->UndoMove(true);
	}
}

