/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */

#include "View.h"
#include <iostream>

using namespace tetris;
using std::cout;
using std::endl;

View::View(Model& m) : model(m) {
	//piece;
	piecePos = sf::Vector2f(-1.f, -1.f);
	pieceMoveSpeed = 0.f;
	pieceMoveAnimating = false;

	//map;
	//nextPiece;
	nextPiecePos = Pos(2, model.getSize().c+1)*25;

	blockSize = 25.f;
	oldModelPos = Pos(-1, -1);
	reqdPos = sf::Vector2f(-1.f, -1.f);;

	animRunning = false;
	initialAnimStep = 0.f;
	animStep = 0.f;
	currentAnimNr = 0;
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
				rect.setOutlineColor(sf::Color(sf::Color::Black));
				rect.setOutlineThickness(-0.f);
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
	map2rect(model.getNextPiece().getMap(), nextPiecePos, nextPiece);
}

void View::drawAll(sf::RenderWindow& window){

	//box around play area
	sf::RectangleShape bx(sf::Vector2f(model.getSize().c*blockSize, model.getSize().r*blockSize));
	bx.setFillColor(sf::Color(121,121,121,255));
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
	sf::Vector2f newPos = newModelPos * blockSize;

	if ( !(piecePos == newPos) ){
		pieceMoveAnimating = true;

		//distance between old and new pos
		float dx = (float) newPos.x - piecePos.x;
		float dy = (float) newPos.y - piecePos.y;

		//speed of movement
		speedx = (dx / 6e4) * frameTime.asMicroseconds();
		speedy = (dy / 6e4) * frameTime.asMicroseconds();

		piecePos.x += speedx;
		piecePos.y += speedy;

		map2rect(model.getPiece().getMap(), piecePos, piece);

		if (abs(newPos.x - piecePos.x) < 1.f && abs(newPos.y - piecePos.y) <1.f ){
			makePiece();
		}

	}
	//resume ticks postponed during animation downwards
	if (abs(newPos.y - piecePos.y) <1.f ){
		pieceMoveAnimating = false;
	}

}


void View::pieceOnNextPiece(){
	piecePos.x = nextPiecePos.x;
	piecePos.y = nextPiecePos.y;
}


void View::doAnim(sf::Time frameTime){
	static int64_t animTime = 5e5;

	float delta = (float) (initialAnimStep/(float)animTime) * (float)frameTime.asMicroseconds();
	animStep -= delta;

	if (currentAnimNr==1 || currentAnimNr==2){

		for (size_t rtr : model.getRowsToRemove()){
			for (sf::RectangleShape& rect : map[rtr]){
				if (currentAnimNr==1){
					sf::Color c = rect.getFillColor();
					sf::Color n(c.r, c.g, c.b, (uint8_t)animStep);
					rect.setFillColor(n);
				} else if (currentAnimNr==2){
					sf::Vector2f size(rect.getSize());
					sf::Vector2f pos(rect.getPosition());
					size.x -= delta;
					size.y -= delta;
					pos.x += delta/2;
					pos.y += delta/2;
					rect.setSize(size);
					rect.setPosition(pos);
				}
			}
		}
		//finish
		if (animStep < 1.f){
			animRunning = false;
			model.removeRowsToRemove();
		}
	}

}

void View::setAnim(int animNr){
	currentAnimNr = animNr;
	switch (animNr) {
	case 1:
		initialAnimStep = 255.f;
		break;
	case 2:
		initialAnimStep = blockSize;
		break;
	}

	animStep = initialAnimStep;
	animRunning = true;
}


