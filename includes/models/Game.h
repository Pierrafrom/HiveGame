#ifndef GAME_H
#define GAME_H

#include "Player.h";
#include "Board.h";
#include <cstddef> // For size_t 
#include <vector>

namespace hive::models {
	class Game {
	private:
		// define board
		Board* board;

		// define vector with the 2 players 
		std::vector<Player*> players;

		// define current player index based on the vector player position
		size_t currentIndexPlayer;

		// define turn number
		int turnNumber;

		// define game state history with a stack
		std::stack<GameState*> gameStateHistory;

		// define undo count 
		size_t undoCount;

		// Private constructor to enforce singleton pattern
		Game() = default;

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
			// define play turn function
			
			// define undo a play function
			void undoMove();

			// define save game state function
			void saveGameState() {
				GameState* newState = new GameState(*board, currentIndexPlayer, turnNumber);
				gameStateHistory.push(newState);
			}

			// define restore game state function
			// define is game over function 
			// define get winner function 
		
			// define set board state function
			void setBoardState(const Board& bs) {
				if (board != nullptr) delete board; // free board memory
				// need to add a surcharge for 'Board(bs)' and REMOVE SINGLETON
				board = new Board(bs); // asign board with the new bs

			}

			// define set turn number function
			void setTurnNumber(const int number) {
				turnNumber = number;
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
			void setCurrentIndexPlayer(const int index) {
				currentIndexPlayer = index;
			};

			// define get current player function
			Player* getCurrentPlayer() {
				return players[currentIndexPlayer];
			}
		
			// define next player function
			void nextPayer();

			// Destructor
			~Game() {
				delete board;
				while (!gameStateHistory.empty()) {
					gameStateHistory.pop();
				}
			};
	};

	class GameState {
			// define board state
			Board boardState;

			// define current player index
			size_t currentIndexPlayer;

			// define turn number
			int turnNumber;

		public:
			// actual game state function  
			GameState(const Board& board, const size_t currentPlayerIndex, const int turnNumber);
		
			// get board state function
			const Board& getBoardState() {
				return boardState;
			};
		
			// get current player index function
			int getPlayerIndex() {
				return currentIndexPlayer;
			};

			// get turn number function
			int getTurnNumber() {
				return turnNumber;
			}
	};
}

#endif // GAME_H