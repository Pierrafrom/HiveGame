#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>

/**************************************************************************
 * Just create to import it in Game, need to be continued...
 *************************************************************************/

namespace hive::models {
	class Player {
		std::string name;
	public:
		Player(std::string n) : name(n) {};
		~Player() = default;
	};
}

#endif // PLAYER_H