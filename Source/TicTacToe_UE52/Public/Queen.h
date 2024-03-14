// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Piece.h"

#include "CoreMinimal.h"
#include "Queen.generated.h"

UCLASS()
class TICTACTOE_UE52_API AQueen : public APiece
{
	GENERATED_BODY()
	
public:
    // ************ CONSTRUCTORS ************
    AQueen();
    //~AQueen();

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
