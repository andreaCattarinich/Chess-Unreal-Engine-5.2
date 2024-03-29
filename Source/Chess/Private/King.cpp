// Fill out your copyright notice in the Description page of Project Settings.

#include "King.h"

AKing::AKing()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::KING;
    PieceValue = 900;
}

void AKing::BeginPlay()
{
    Super::BeginPlay();
}

void AKing::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> AKing::PieceLegalMoves()
{
    const TArray<FVector2D> Directions = {
        FVector2D(1, 0),    // UP
        FVector2D(-1, 0),   // DOWN
        FVector2D(0, 1),    // RIGHT
        FVector2D(0, -1),   // LEFT
        FVector2D(1, -1),   // UP-LEFT
        FVector2D(1, 1),    // UP-RIGHT
        FVector2D(-1, -1),  // DOWN-LEFT
        FVector2D(-1, 1)    // DOWN-RIGHT
    };

    // False indicate that the knight cannot continue until encounter an obstacle
    // So, the piece do one move in all Directions
    return SearchLegalMoves(Directions, false);
}