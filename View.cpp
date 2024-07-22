/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */
#include "View.h"
#include <iostream>

using namespace tetris;

View::View(Model& m) : model(m) {
	//piece;
	piecePos = sf::Vector2f(-1.f, -1.f);
	pieceMoveSpeed = 0.f;
	pieceMoveAnimating = false;

	//map;
	//nextPiece;
	blockSize = 25.f;
	oldModelPos = Pos(-1, -1);
	reqdPos = sf::Vector2f(-1.f, -1.f);;
}

void View::map2rect(const std::vector<std::vector<Block>>& blockMap, sf::Vector2f zeroPoint, std::vector<std::vector<sf::RectangleShape>>& rectMap){
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
				rect.setPosition((float) (zeroPoint.x+c*blockSize), (float) (zeroPoint.y+r*blockSize));
				rect.setFillColor(b.getColor());
				rect.setOutlineColor(sf::Color(sf::Color::Green));
				rect.setOutlineThickness(0.f);
				rectRow.push_back(rect);
			}
			++c;
		}
		rectMap.push_back(rectRow);
		++r;
	}
}


void View::makePiece(){
	piecePos = model.getPiece().getPos() * blockSize;
	map2rect(model.getPiece().getMap(), piecePos, piece);
	oldModelPos = model.getPiece().getPos();
}
void View::makeMap(){
	Pos pos = {0,0};
	map2rect(model.getMap(), pos*blockSize, map);

}
void View::makeNextPiece(){
	Pos mapsize = model.getSize();
	Pos pos = {2, mapsize.c+1};
	map2rect(model.getNextPiece().getMap(), pos*blockSize, nextPiece);
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


void View::movePiece(sf::Time frameTime){
	static float speedx;
	static float speedy;

	Pos newModelPos = model.getPiece().getPos();
	if ( !(oldModelPos == newModelPos) ){
		sf::Vector2f newPos = newModelPos * blockSize;
		//distance between old and new pos
		float dx = (float) newPos.x - piecePos.x;
		float dy = (float) newPos.y - piecePos.y;

		//speed of movement
		speedx = (dx / 3e4) * frameTime.asMicroseconds();
		speedy = (dy / 3e4) * frameTime.asMicroseconds();

		piecePos.x += speedx;
		piecePos.y += speedy;

		map2rect(model.getPiece().getMap(), piecePos, piece);

		if (abs(newPos.x - piecePos.x) < 1 && abs(newPos.y - piecePos.y) <1 ){
			makePiece();
		}


		//indicate animation downwards to postpone ticks
		if (oldModelPos.r != newModelPos.r){
			pieceMoveAnimating = true;
		}
		else {
			pieceMoveAnimating = false;
		}


	}





	//newpiece = std::vector<std::vector<sf::RectangleShape>>;
	//map2rect(model.getPiece().getMap(), model.getPiece().getPos(), blockSize, piece);
}



