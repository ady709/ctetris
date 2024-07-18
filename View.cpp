/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */
#include "View.h"
using namespace tetris;

View::View(Model& m) : model(m), blockSize(25) {

}

void View::map2rect(const std::vector<std::vector<Block>>& blockMap, Pos zeroPoint, int blockSize, std::vector<std::vector<sf::RectangleShape>>& rectMap){
	//for each block in map, make rectangle
	rectMap.clear();
	std::vector<sf::RectangleShape> rectRow;
	int r=0;
	for (std::vector<Block> row : blockMap){
		rectRow.clear();
		int c=0;
		for (Block b : row){
			if (b.isOccupied()){
				sf::RectangleShape rect(sf::Vector2f(blockSize, blockSize));
				rect.setPosition((float) (zeroPoint.c+c)*blockSize, (float) (zeroPoint.r+r)*blockSize);
				rect.setFillColor(b.getColor());
				rect.setOutlineColor(sf::Color(sf::Color::Green));
				rect.setOutlineThickness(-1.f);
				rectRow.push_back(rect);
			}
			++c;
		}
		rectMap.push_back(rectRow);
		++r;
	}
}


void View::makePiece(){
	map2rect(model.getPiece().getMap(), model.getPiece().getPos(), blockSize, piece);
}
void View::makeMap(){
	Pos pos = {0,0};
	map2rect(model.getMap(), pos, blockSize, map);

}
void View::makeNextPiece(){
	Pos mapsize = model.getSize();
	Pos pos = {2, mapsize.c+1};
	map2rect(model.getNextPiece().getMap(), pos, blockSize, nextPiece);
}

void View::drawAll(sf::RenderWindow& window){

	//box around play area
	sf::RectangleShape bx(sf::Vector2f(model.getSize().c*blockSize, model.getSize().r*blockSize));
	bx.setFillColor(sf::Color(12,12,12,255));
	bx.setOutlineColor(sf::Color(sf::Color::White));
	bx.setOutlineThickness(-1.f);
	//bx.setPosition(1.f, 1.f);
	window.draw(bx);


	std::vector<std::vector<sf::RectangleShape>>* objects[] = {&map, &piece, &nextPiece};
	for (auto& obj : objects){
		for (auto row : *obj){
			for (sf::RectangleShape b : row){
				window.draw(b);
			}
		}
	}
}






