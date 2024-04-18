// Copyright © 2024 Andrea Cattarinich

#include "MoveWidgetEntry.h"

void UMoveWidgetEntry::SetData(const FMove& Move)
{
	NumberMove = ceil(Move.NumberMove / 2.0);
	
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

	            GameMode->GField->SetSelectedTile(LastMove.Start);
	            GameMode->DoMove(LastMove.End);

	            if (LegalMovesOfOtherPiece.Contains(Move.End))
	            {
	                if (Move.Start.Y == OtherPiece->GetGridPosition().Y)
	                {
	                    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Special Case 2"));
	                    StringMove += FString::FromInt(Move.Start.X + 1);
	                }
	                else
	                {
	                    //GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Special Case 1"));
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

	bool bIsStalemate = true;
	const int32 Player = (*GameMode->GField->TileMap.Find(Move.End))->GetTileOwner();
	for(const auto& CurrentTile : GameMode->GField->TileArray)
	{
		if(CurrentTile->GetTileOwner() == Player)
		{
			GameMode->GField->SetSelectedTile(CurrentTile->GetGridPosition());

			TArray<FVector2D> CurrentLegalMoves;
			
			CurrentLegalMoves = GameMode->GField->LegalMoves(CurrentTile->GetGridPosition());
			
			if(CurrentLegalMoves.Contains(GameMode->GField->GetKingPosition((GameMode->GetNextPlayer(GameMode->CurrentPlayer)))))
			{
				
				if(GameMode->IsWinnerMove(Player, false))
				{
					bIsStalemate = false;
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

	if(GameMode->IsWinnerMove(Player, false) && bIsStalemate)
	{
		StringMove += " 1/2-1/2";
	}
	
	if(Move.NumberMove % 2 == 1)
	{
		TextLabel1->SetText(FText::FromString(StringMove));
		Btn1->OnClicked.AddDynamic(this, &UMoveWidgetEntry::OnBtnClick);
	}
	else
	{
		TextLabel2->SetText(FText::FromString(StringMove));
		Btn2->OnClicked.AddDynamic(this, &UMoveWidgetEntry::OnBtnClick);
	}
}

FString UMoveWidgetEntry::TypeToChar(const EPieceType Type)
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

FString UMoveWidgetEntry::PositionToStringMove(const FVector2D Position)
{
	const FString X = FString::FromInt(Position.X + 1);
	const int32 IntPositionY = FMath::RoundToInt(Position.Y);
	const FString Y = IntToLetter(IntPositionY);

	return Y + X;

}

FString UMoveWidgetEntry::IntToLetter(const int32 Value)
{
	FString Letter;
	switch (Value)
	{
	case 0: return "a";
	case 1: return "b";
	case 2: return "c";
	case 3: return "d";
	case 4: return "e";
	case 5: return "f";
	case 6: return "g";
	case 7: return "h";
	default: return "Invalid";
	}
}

void UMoveWidgetEntry::OnBtnClick()
{
	if(!CanClick)
	{
		return;
	}
	
	// Black must do the move. After then you can reset the field
	if(GameMode->Moves.Num() % 2 == 1 || GameMode->Moves.Num() == NumberMove*2 || !GameMode->IsGameOver)
	{
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Move pressed: %d"), NumberMove);

	CanClick = false;
	DelayedUndo();
	CanClick = true;
}

void UMoveWidgetEntry::DelayedUndo()
{
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
		{
			GameMode->UndoMove(true);
		
			if(GameMode->Moves.Num() > NumberMove*2)
			{
				DelayedUndo();	
			}
		}, 0.25f, false);
}
