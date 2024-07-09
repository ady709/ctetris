/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */
#include "View.h"
using namespace tetris;

View::View(Model& m) : model(m) {

}

void View::updatePiece(sf::RenderWindow& window, int blockSize){
	//clear existing
	piece.clear();

	//make new
	//reference to piece.map
	const std::vector<std::vector<Block>>& m = model.getPiece().getMap();
	// position of the piece
	Pos p = model.getPiece().getPos();
	//for each block in map, make rectangle
	int r=0;
	for (std::vector<Block> row : m){
		int c=0;
		for (Block b : row){
			if (b.isOccupied()){
				sf::RectangleShape rect(sf::Vector2f(blockSize, blockSize));

				rect.setPosition((float) (p.c+c)*blockSize, (float) (p.r+r)*blockSize);
				rect.setFillColor(b.getColor());
				piece.push_back(sf::RectangleShape(rect)); //TODO is it needed to save piece rectangles into vector? Would it be needed in future?
				window.draw(rect);


			}
			++c;
		}
		++r;
	}
}


void View::updateMap(sf::RenderWindow& window, int blockSize){

	//clear existing
	map.clear();

	//make new
	//reference to piece.map
	const std::vector<std::vector<Block>>& m = model.getMap();

	//for each block in map, make rectangle
	int r=0;
	for (std::vector<Block> row : m){
		int c=0;
		for (Block b : row){
			if (b.isOccupied()){
				sf::RectangleShape rect(sf::Vector2f(blockSize, blockSize));
				rect.setPosition((float) c*blockSize, (float) r*blockSize);
				rect.setFillColor(b.getColor());
				piece.push_back(sf::RectangleShape(rect)); //TODO is it needed to save piece rectangles into vector? Would it be needed in future?
				window.draw(rect);


			}
			++c;
		}
		++r;
	}
}






