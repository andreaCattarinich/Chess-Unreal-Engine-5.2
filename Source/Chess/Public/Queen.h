// Copyright © 2024 Andrea Cattarinich

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Queen.generated.h"

UCLASS()
class CHESS_API AQueen : public APiece
{
	GENERATED_BODY()
	
public:
    // ************ CONSTRUCTORS ************
    AQueen();

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
