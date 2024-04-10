// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Pawns.generated.h"

UCLASS()
class CHESS_API APawns : public APiece
{
	GENERATED_BODY()

public:
    // ************ CONSTRUCTORS ************
    APawns();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;



    // ************ ATTRIBUTES ************	
    


    // ************ SETTERS ************	



    // ************ GETTERS ************	



    // ************ METHODS ************	
    virtual TArray<FVector2D> PieceLegalMoves() override;

    TArray<FVector2D> GetForwardMoves() const;
    TArray<FVector2D> GetDiagonalCapturesIfExists() const;
};
