// Fill out your copyright notice in the Description page of Project Settings.

#include "Pawns.h"
#include "..\Public\Chess_GameMode.h"

APawns::APawns()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::PAWN;
    PieceValue = 10;
}

void APawns::BeginPlay()
{
    Super::BeginPlay();
}

void APawns::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> APawns::PieceLegalMoves()
{
    TArray<FVector2D> LegalMoves;
    
    LegalMoves += GetForwardMoves();
    LegalMoves += GetDiagonalCapturesIfExists();

    return LegalMoves;
}

TArray<FVector2D> APawns::GetForwardMoves()
{
    TArray<FVector2D> ForwardMoves;
    
    TArray<FVector2D> StandardDirections = {
        FVector2D(1, 0),    // UP-1
        FVector2D(2, 0),    // UP-2
    };
    
    // Condition that change white -> black moves
    if (GetOwner() == 1)
    {
        for (FVector2D& Direction : StandardDirections)
        {
            Direction *= -1;
        }
    }

    bool bCanMoveUp = true;
    for (const FVector2D& Direction : StandardDirections)
    {
        const FVector2D PawnPosition = GetGridPosition();
        FVector2D Position = GetGridPosition();
        Position += Direction;
        
        if (GameMode->GField->IsValidPosition(Position) && bCanMoveUp)
        {
            const ATile* CurrTile = *GameMode->GField->TileMap.Find(Position);

            if (CurrTile->GetTileStatus() == ETileStatus::EMPTY)
            {
                if (GetOwner() == 0 && PawnPosition.X != 1)
                {
                    bCanMoveUp = false;
                }
                else if (GetOwner() == 1 && PawnPosition.X != GameMode->GField->Size - 2)
                {
                    bCanMoveUp = false;
                }
                ForwardMoves.Add(Position);
            }
            
            if(CurrTile->GetTileStatus() == ETileStatus::OCCUPIED)
            {
                bCanMoveUp = false;
            }
        }
    }
    
    return ForwardMoves;
}

TArray<FVector2D> APawns::GetDiagonalCapturesIfExists()
{
    TArray<FVector2D> DiagonalMoves;
    
    // MOSSE CHE MANGIANO LA PEDINA AVVERSARIA
    TArray<FVector2D> Directions = {
        FVector2D(1, -1),   // UP-LEFT
        FVector2D(1, 1),    // UP-RIGHT
    };

    // Condition that change white -> black moves
    if (GetOwner() == 1)
    {
        for (FVector2D& Direction : Directions)
        {
            Direction *= -1;
        }
    }

    for (const FVector2D& Direction : Directions)
    {
        FVector2D Position = GetGridPosition();
        Position += Direction;
 
        if (GameMode->GField->IsValidPosition(Position))
        {
            ATile* CurrTile = *GameMode->GField->TileMap.Find(Position);

            if (CurrTile->GetTileStatus() == ETileStatus::OCCUPIED)
            {
                if (CurrTile->GetOwner() == ((GameMode->CurrentPlayer) ^ 1))
                {
                    DiagonalMoves.Add(Position);
                }
            }
        }
    }

    return DiagonalMoves;
}

