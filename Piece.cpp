/*
 * Piece.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */

#include "Piece.h"
#include <iostream>
using namespace tetris;

Piece::Piece(int mapNr){
	setMap(mapNr);
	r = 0;
	c = 0;
}

//construct from map and pos
Piece::Piece(std::vector<std::vector<Block>> m, Pos pos){
        map = m;
        width = map[0].size();
        height = map.size();
        setLimits();
        r = pos.r;
        c= pos.c;

}


void Piece::fall(){
	++r;
}

void Piece::goLeft(){
	--c;
}

void Piece::goRight(){
	++c;
}

void Piece::shiftPos(Pos pos){
	c+=pos.c;
	r+=pos.r;
}

const std::vector<std::vector<Block>>& Piece::getMap() const{
	return map;
}


const Pos Piece::getPos() const{
	Pos p = {r,c};
	return p;
}

const Pos Piece::getDims() const{
	Pos dim = {height, width};
	return dim;
}

void Piece::init(){
	map.clear();
	std::vector<Block> row;

	for (int y=0; y<height; ++y){
		row.clear();
		for (int x=0; x<width; ++x){
			row.push_back(Block());
		}
		map.push_back(row);
	}
}

void Piece::setLimits(){
	limits.left.clear();
	limits.right.clear();
	limits.bottom.clear();
	for (int i=0; i<width; ++i){
		limits.bottom.push_back(0);
	}


	//got through rows
	for (int r=0; r<height; ++r){
		//and find first/last
		int first=width-1, last=0;
		for (int c=0; c<width; ++c){
			if (map[r][c].isOccupied()){
				if (c<first)
					first = c;
				if (c>last)
					last=c;
				if (r>limits.bottom[c])
					limits.bottom[c] = r;
			}
		}
		limits.left.push_back(first);
		limits.right.push_back(last);
	}

//	std::cout << "Limits: bottom\n";
//	for (int i : limits.bottom)
//		std::cout << i << " ";
//	std::cout << std::endl;
}

const Limits& Piece::getLimits() const{
	return limits;
}

Piece Piece::getRotated() const{
	//init rotated map

	int newWidth = height;
	int newHeight = width;
	std::vector<std::vector<Block>> rotmap;
	for (int y=0; y<newHeight; ++y){
		std::vector<Block> newrow;
		for (int x=0; x<newWidth; ++x){
			newrow.push_back(Block());
		}
		rotmap.push_back(newrow);
	}

//	std::cout << "newRows " << rotmap.size() << std::endl;
//	std::cout << "newColss " << rotmap[0].size() << std::endl;

	//fill out the rotated map
	for (int y=0; y<height; ++y){
		//std::cout << "y " << y << std::endl;
		for (int x=0; x<width; ++x){
			//std::cout << "x " << x << std::endl;
			rotmap[x][newWidth-y-1] = map[y][x];
		}
	}


	Piece rotatedPiece(rotmap, getPos());


	return rotatedPiece;
}


void Piece::setMap(int mNr){

	mapNr = mNr;
	sf::Color col;
	switch(mapNr){
	case 1: //T
		width = 2;
		height = 3;
		init();
		col = sf::Color(224, 199, 36, 255);
		map[0][0] = Block(col);
		map[1][0] = Block(col);
		map[1][1] = Block(col);
		map[2][0] = Block(col);
		break;
	case 2: //L
		width = 2;
		height = 3;
		init();
		col = sf::Color(179, 10, 201, 255);
		map[0][0] = Block(col);
		map[1][0] = Block(col);
		map[2][0] = Block(col);
		map[2][1] = Block(col);
		break;
	case 3: //J
		width = 2;
		height = 3;
		init();
		col = sf::Color(255, 122, 69, 255);
		map[0][1] = Block(col);
		map[1][1] = Block(col);
		map[2][1] = Block(col);
		map[2][0] = Block(col);
			break;
	case 4: //I
		width = 1;
		height = 4;
		init();
		col = sf::Color(145, 10, 10, 255);
		map[0][0] = Block(col);
		map[1][0] = Block(col);
		map[2][0] = Block(col);
		map[3][0] = Block(col);
		break;
	case 5: //S
		width = 2;
		height = 3;
		init();
		col = sf::Color(73, 191, 73, 255);
		map[0][0] = Block(col);
		map[1][0] = Block(col);
		map[1][1] = Block(col);
		map[2][1] = Block(col);
		break;
	case 6: //revS
		width = 2;
		height = 3;
		init();
		col = sf::Color(82, 189, 222, 255);
		map[0][1] = Block(col);
		map[1][1] = Block(col);
		map[1][0] = Block(col);
		map[2][0] = Block(col);
		break;
	case 7: //box
		width = 2;
		height = 2;
		init();
		col = sf::Color(25, 31, 209, 255);
		map[0][0] = Block(col);
		map[1][0] = Block(col);
		map[0][1] = Block(col);
		map[1][1] = Block(col);
		break;
	case 0: //blank
	default:
		width = 0;
		height = 0;
		init();
		break;
	}
	setLimits();

}

int Piece::getMapNr() const{
	return mapNr;
}

void Piece::setPos(Pos p){
	r=p.r;
	c=p.c;
}

size_t Piece::getLongestDim(){
	size_t longest = 0;
	int mapnr = 1;
	while (true){
		Piece p(mapnr);
		Pos dims = p.getDims();
		++mapnr;
		if (dims.r == 0)
			break;
		if ((unsigned int)dims.r > longest)
			longest = dims.r;
		if ((unsigned int)dims.c > longest)
			longest = dims.c;
	}
	return longest;
}


