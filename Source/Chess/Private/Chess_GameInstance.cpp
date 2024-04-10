// Copyright © 2024 Andrea Cattarinich

#include "Chess_GameInstance.h"

void UChess_GameInstance::SetTurnMessage(FString Message)
{
	CurrentTurnMessage = Message;
}

void UChess_GameInstance::SetOpponent(const int32 SelectedOpp)
{
	Difficulty = SelectedOpp;
}

int32 UChess_GameInstance::GetScoreHumanPlayer() const
{
	return ScoreHumanPlayer;
}

int32 UChess_GameInstance::GetScoreAiPlayer() const
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

