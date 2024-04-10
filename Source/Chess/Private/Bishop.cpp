// Copyright © 2024 Andrea Cattarinich

#include "Bishop.h"

ABishop::ABishop()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::BISHOP;
    PieceValue = 30;
}

void ABishop::BeginPlay()
{
    Super::BeginPlay();
}

void ABishop::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> ABishop::PieceLegalMoves()
{
    const TArray<FVector2D> Directions = {
        FVector2D(1, -1),   // UP-LEFT
        FVector2D(1, 1),    // UP-RIGHT
        FVector2D(-1, -1),  // DOWN-LEFT
        FVector2D(-1, 1)    // DOWN-RIGHT
    };

    return SearchLegalMoves(Directions);
}
