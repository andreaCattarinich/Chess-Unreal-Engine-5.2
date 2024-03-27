// Fill out your copyright notice in the Description page of Project Settings.

/*** Oggetto persistente per tutta la durata del gioco ***/
#include "TTT_GameInstance.h"

void UTTT_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

void UTTT_GameInstance::SetOpponent(const int32 SelectedOpp)
{
	Difficulty = SelectedOpp;
}

int32 UTTT_GameInstance::GetScoreHumanPlayer()
{
	return ScoreHumanPlayer;
}

int32 UTTT_GameInstance::GetScoreAiPlayer()
{
	return ScoreAiPlayer;
}

FString UTTT_GameInstance::GetTurnMessage()
{
	return CurrentTurnMessage;
}

void UTTT_GameInstance::IncrementScoreHumanPlayer()
{
	ScoreHumanPlayer += 1;
}

void UTTT_GameInstance::IncrementScoreAiPlayer()
{
	ScoreAiPlayer += 1;
}

