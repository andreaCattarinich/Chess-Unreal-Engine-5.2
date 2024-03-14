// Fill out your copyright notice in the Description page of Project Settings.

#include "Rook.h"

ARook::ARook()
{
    // Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;
    PieceType = EPieceType::ROOK;
    PieceValue = 50;
}

void ARook::BeginPlay()
{
    Super::BeginPlay();
}

void ARook::Tick(const float DeltaTime)
{
    Super::Tick(DeltaTime);
}

TArray<FVector2D> ARook::PieceLegalMoves()
{
    const TArray<FVector2D> Directions = {
        FVector2D(1, 0),  // UP
        FVector2D(-1, 0), // DOWN
        FVector2D(0, 1),  // RIGHT
        FVector2D(0, -1)  // LEFT
    };

    return SearchLegalMoves(Directions);
}