/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */

#include "View.h"
#include <iostream>

using namespace tetris;
using std::to_string; // required
using std::cout;
using std::endl;

View::View(Model& m) : model(m) {
	blockSize = 25.f;
	//piece;
	piecePos = sf::Vector2f(-1.f, -1.f);
	pieceMoveSpeed = 0.f;
	pieceMoveAnimating = false;

	//nextPiece;
	nextPiecePos = Pos(2, model.getSize().c+1)*blockSize;


	oldModelPos = Pos(-1, -1);

	animRunning = false;
	initialAnimStep = 0.f;
	animStep = 0.f;
	currentAnimNr = 0;
	
	labelFont = sf::Font();
	labelFont.loadFromFile("arial.ttf");

	float nextToPlayField = (float) (blockSize*model.getSize().c)+blockSize;
	//Texts
	nextLabel = sf::Text("", labelFont, blockSize);
	nextLabel.setFillColor(sf::Color(194, 21, 21));
	nextLabel.setPosition(sf::Vector2f(nextToPlayField, (float) blockSize*0.7f));

	float belowNext = (float) (Piece::getLongestDim())*blockSize + nextPiecePos.y + blockSize;

	scoreLabelT = sf::Text("Score:", labelFont, blockSize);
	scoreLabelT.setFillColor(sf::Color(194, 21, 21));
	scoreLabelT.setOrigin(0.f, (float) blockSize);
	scoreLabelT.setPosition(sf::Vector2f(nextToPlayField, belowNext));
	
	levelLabelT = sf::Text("Level:",labelFont, blockSize);
	levelLabelT.setFillColor(sf::Color(194, 21, 21));
	levelLabelT.setOrigin(0.f, (float) blockSize);
	levelLabelT.setPosition(sf::Vector2f(nextToPlayField, belowNext + blockSize));

	float nextToLabels = (float) nextToPlayField + getWidestLabel() + (float) blockSize*0.4f;

	//Values
	scoreLabelV = sf::Text("123", labelFont, blockSize*0.8f);
	scoreLabelV.setFillColor(sf::Color(194, 21, 21));
	scoreLabelV.setOrigin(0.f, (float) blockSize*0.8f);
	scoreLabelV.setPosition(sf::Vector2f(nextToLabels, belowNext));
	
	levelLabelV = sf::Text("123",labelFont, blockSize*0.8f);
	levelLabelV.setFillColor(sf::Color(194, 21, 21));
	levelLabelV.setOrigin(0.f, (float) blockSize*0.8f);
	levelLabelV.setPosition(sf::Vector2f(nextToLabels, belowNext + blockSize));

	// sf::Text rowsLabel;
	// sf::Text singleLabel;
	// sf::Text doubleLabel;
	// sf::Text trippleLabel;
	// sf::Text tetrisLabel;
	
}

float View::getWidestLabel() const{
	//sf::Text textLabels[] = {scoreLabelT, levelLabelT};
	float longestText = 0;
	float length = 0;
	for (auto t : textLabels){
		length = t->getGlobalBounds().width;
		if (length > longestText) longestText = length;
	}
	return longestText;
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

	//draw all rectangles, using pointer array to loop through all objects
	std::vector<std::vector<sf::RectangleShape>>* objects[] = {&map, &piece, &nextPiece};
	for (auto& obj : objects){
		for (auto row : *obj){
			for (sf::RectangleShape b : row){
				window.draw(b);
			}
		}
	}

	//draw texts
	//Next:
	nextLabel.setString("Next:");
	window.draw(nextLabel);
	//score
	scoreLabelV.setString(to_string(model.getScore()));
	window.draw(scoreLabelT);
	window.draw(scoreLabelV);
	//Level
	levelLabelV.setString(to_string(model.getLevel()));
	window.draw(levelLabelT);
	window.draw(levelLabelV);

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


