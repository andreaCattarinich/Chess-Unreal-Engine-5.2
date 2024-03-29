// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Knight.generated.h"

UCLASS()
class CHESS_API AKnight : public APiece
{
	GENERATED_BODY()

public:
    // ************ CONSTRUCTORS ************
    AKnight();
    //~AKnight();

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
