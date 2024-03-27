// Fill out your copyright notice in the Description page of Project Settings.

/*** Oggetto persistente per tutta la durata del gioco ***/
//#include "..\Public\Chess_GameInstance.h"

#include "Chess_GameInstance.h"

void UChess_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

void UChess_GameInstance::SetOpponent(const int32 SelectedOpp)
{
	Difficulty = SelectedOpp;
}

int32 UChess_GameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UChess_GameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

FString UChess_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UChess_GameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UChess_GameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

