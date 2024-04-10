// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Bishop.generated.h"

UCLASS()
class CHESS_API ABishop : public APiece
{
    GENERATED_BODY()
public:
    // ************ CONSTRUCTORS ************
    ABishop();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;



    // ************ ATTRIBUTES ************	



    // ************ SETTERS ************	



    // ************ GETTERS ************	



    // ************ METHODS ************
    virtual TArray<FVector2D> PieceLegalMoves() override;
};
