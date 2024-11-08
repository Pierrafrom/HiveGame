#include <iostream>
#include "models/Game.h";

namespace hive::models {
	GameState::GameState(const Board& board, const size_t currentPlayerIndex, const int turnNumber) : boardState(board), currentIndexPlayer(currentPlayerIndex), turnNumber(turnNumber) {};
	
	void Game::undoMove() {
		GameState* temp = gameStateHistory.top();
		setBoardState(temp->getBoardState());
		setCurrentIndexPlayer(temp->getPlayerIndex());
		setTurnNumber(temp->getTurnNumber());
		delete temp;
		gameStateHistory.pop();
	}

	void Game::nextPayer() {
		setCurrentIndexPlayer((currentIndexPlayer == 1) ? 2 : 1);
	}
}