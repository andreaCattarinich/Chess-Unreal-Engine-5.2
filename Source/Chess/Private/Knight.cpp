// Fill out your copyright notice in the Description page of Project Settings.

#include "Knight.h"

AKnight::AKnight()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::KNIGHT;
    PieceValue = 30;
}

void AKnight::BeginPlay()
{
    Super::BeginPlay();
}

void AKnight::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> AKnight::PieceLegalMoves()
{
    const TArray<FVector2D> Directions = {
        FVector2D(2,-1),
        FVector2D(2, 1),
        FVector2D(-2,-1),
        FVector2D(-2, 1),
        FVector2D(1, -2),
        FVector2D(-1, -2),
        FVector2D(1,2),
        FVector2D(-1,2)
    };

    // False indicate that the knight cannot continue until encounter an obstacle
    // So, the piece do one move in all Directions
    return SearchLegalMoves(Directions, false);
}