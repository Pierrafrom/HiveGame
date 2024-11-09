#ifndef GAME_H
#define GAME_H

#include "Player.h"
#include "Board.h"
#include <cstddef> // For size_t 
#include <vector>
#include <stack>

namespace hive::models {
	class Game {
	private:
		// define board
		Board* board;

		// define vector with the 2 players 
		std::vector<Player*> players;

		// define current player index based on the vector player position
		size_t currentPlayerIndex;

		// define turn number
		int turnNumber;

		// Private constructor to enforce singleton pattern
		Game() = default;

		// define game state history with a stack
		std::stack<GameState*> gameStateHistory;

	public:
		/**************************************************************************
		 * Singleton pattern
		 *************************************************************************/
		static Game& getInstance();

		// Delete copy constructor and assignment operator to enforce singleton
		Game(const Game&) = delete;
		Game& operator=(const Game&) = delete;


		/**************************************************************************
		* Game methods
		*************************************************************************/

		// define start game function
		void startGame();

		// define play turn function
		// TODO
		void playTurn();

		// define undo a play function
		void undoMove();

		// define save game state function
		void saveGameState();

		// define restore game state function
		
		// define is game over function 
		// define get winner function 

		// define set board state function
		void setBoardState(const Board& bs) {
			if (board != nullptr) delete board; // free board memory
			// need to add a surcharge to 'Board(bs)' and REMOVE SINGLETON
			board = new Board(bs); // asign board with the new bs

		}

		/**************************************************************************
		* Services methods
		*************************************************************************/

		// define set turn number function
		void setTurnNumber(const int n) {
			turnNumber = n;
		}

		// define up turn number function
		void upTurnNumber() {
			turnNumber++;
		}

		// define down turn number function
		void downTurnNumber() {
			turnNumber--;
		}

		// define set current index player function
		void setCurrentPlayerIndex(const int i) {
			currentPlayerIndex = i;
		};

		// define get current player function
		Player* getCurrentPlayer() {
			return players[currentPlayerIndex];
		}

		// define next player function
		void nextPayer() {
			setCurrentPlayerIndex((currentPlayerIndex == 1) ? 2 : 1);
		};

		// Destructor function
		~Game() {
			delete board;
			while (!gameStateHistory.empty()) {
				gameStateHistory.pop();
			}
			// need to be complete
		};
	};
}

#endif // GAME_H