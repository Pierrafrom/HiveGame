#ifndef GAMESTATE_H
#define GAMESTATE_H

#include "Board.h"

namespace hive::models {
	class GameState {
		// define board state
		Board boardState;

		// define current player index
		size_t currentPlayerIndex;

		// define turn number
		int turnNumber;

	public:
		// actual game state function  
		GameState(const Board& b, const int c, const int t) : boardState(b), currentPlayerIndex(c), turnNumber(t) {};

		// get board state function
		Board& getBoardState() {
			return boardState;
		};

		// get current player index function
		int getPlayerIndex() {
			return currentPlayerIndex;
		};

		// get turn number function
		int getTurnNumber() {
			return turnNumber;
		}
	};
}

#endif // GAME_H