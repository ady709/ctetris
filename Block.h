/*
 * Block.h
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */

#ifndef SRC_BLOCK_H_
#define SRC_BLOCK_H_
#include <SFML/Graphics.hpp>

namespace tetris {

class Block {
public:
	Block(sf::Color c);
	Block();
	const sf::Color& getColor();
	const bool isOccupied() const;

private:
	sf::Color color;
	bool occupied;
};

}




#endif /* SRC_BLOCK_H_ */
