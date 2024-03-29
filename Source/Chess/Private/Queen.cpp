// Fill out your copyright notice in the Description page of Project Settings.

#include "Queen.h"

AQueen::AQueen()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::QUEEN;
    PieceValue = 90;
}

void AQueen::BeginPlay()
{
    Super::BeginPlay();
}

void AQueen::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> AQueen::PieceLegalMoves()
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

    return SearchLegalMoves(Directions);
}