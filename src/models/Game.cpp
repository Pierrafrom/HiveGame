#include <iostream>
#include "models/Game.h"
#include "models/GameState.h"

namespace hive::models {
	/**************************************************************************
	 * Singleton pattern
	 *************************************************************************/
	Game& Game::getInstance() {
		static Game instance;
		return instance;
	}

	/**************************************************************************
	* Game methods
	*************************************************************************/

	// function startGame test
	void Game::startGame() {
		board = &Board::getInstance();
		board->clear();

		players.push_back(new Player("Joueur1"));
		players.push_back(new Player("Joueur2"));

		currentPlayerIndex = 0;
		turnNumber = 0;

		saveGameState();
	}

	// function to undo a move
	void Game::undoMove() {
		GameState* temp = gameStateHistory.top();
		setBoardState(temp->getBoardState());
		setCurrentPlayerIndex(temp->getPlayerIndex());
		setTurnNumber(temp->getTurnNumber());
		delete temp;
		gameStateHistory.pop();
	}
		
	// function to save the actual state and add it to the history
	void Game::saveGameState() {
		GameState* newState = new GameState(*board, currentPlayerIndex, turnNumber);
		gameStateHistory.push(newState);
	}
}