/*
 * Block.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */
#include "Block.h"
using namespace tetris;
Block::Block(sf::Color c){
	occupied = true;
	color = c;
}
Block::Block(){
	occupied = false;
	color = sf::Color(0,0,0,0);
}

const bool Block::isOccupied() const{
	return occupied;
}

const sf::Color& Block::getColor(){
	return color;
}

