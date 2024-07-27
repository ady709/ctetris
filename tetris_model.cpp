/*
 * tetris_model.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */

#include "tetris_model.h"
#include <iostream>

using namespace tetris;
Model::Model(const int r, const int c) : rows(r), columns(c) {
	//init();
	//values are set in init(), initialization in constructor is just a fake....
	initialTimer = 500;
	timer = initialTimer;
	rowsToRemove.clear();
	removedRows = 0; score = 0; level = 0;
	combos = nullptr;
	combos_size = 0;
	landed = false;
	gameOver = false;
}

void Model::init(){
	//delete pointed to objects if they exist
	nextPiece = Piece(rand()%7+1);
	piece = Piece(rand()%7+1);
	piece.setPos(Pos(0,columns/2-piece.getDims().c/2));

	timer = initialTimer;
	rowsToRemove.clear();
	removedRows = 0; score = 0; level = 1;

	if (combos!=nullptr)
		delete[] combos;
	combos_size = Piece::getLongestDim();
	combos = new int[combos_size] {0};

	//init play area
	map.clear();
	for (int r=0; r<rows; ++r){
		std::vector<Block> row;
		for (int c=0; c<columns; ++c){
			row.push_back(Block());
		}
		map.push_back(row);
	}

	gameOver = false;
}


void Model::goLeft(){
	Pos pos = piece.getPos();
	Pos dim = piece.getDims();
	Limits limits = piece.getLimits();
	bool canGo = true;
	for (int y=0; y<dim.r;++y){
		int x = pos.c+limits.left[y]-1;
		if (!(x>=0) || map[pos.r+y][x].isOccupied())
			canGo = false;
	}
	if (canGo)
		piece.goLeft();
}
void Model::goRight(){
	Pos pos = piece.getPos();
	Pos dim = piece.getDims();
	Limits limits = piece.getLimits();
	bool canGo = true;

	for (int y=0; y<dim.r;++y){
		int x = pos.c+limits.right[y]+1;
		if (!(x<columns) || map[pos.r+y][x].isOccupied())
			canGo = false;
	}
	if (canGo)
		piece.goRight();

}

const Piece& Model::getPiece() const{
	return piece;
}

const Piece& Model::getNextPiece() const{
	return nextPiece;

}

void Model::rotate(){
	Piece rotatedPiece = piece.getRotated();
	Pos newdim = rotatedPiece.getDims();
	//try to rotate around center
	Pos shift = {0, (newdim.r-newdim.c)/2};
	rotatedPiece.shiftPos(shift);

	//fits in map after rotation?
	Pos shifts[] = {{0,0},{0,-1},{0,1},{0,-2} ,{0,2} }; //,{-1,1},{0,-2},{0,2},{-1,-2},{-1,2} };//,{-2,0},{-2,-2},{-2,2}};
	bool fits = true;
	for (Pos shift : shifts){
		rotatedPiece.shiftPos(shift);
		Pos newpos = rotatedPiece.getPos();
		if (newpos.r<0 || newpos.c<0){
			fits = false;
			break;
		}
		fits = true;
		for (int r=0; r<newdim.r; ++r){
			for (int c=0; c<newdim.c; ++c){
				if (newpos.r+newdim.r > rows || newpos.c+newdim.c > columns
						|| (rotatedPiece.getMap()[r][c].isOccupied() && map[newpos.r+r][newpos.c+c].isOccupied()) ){
					fits = false;
					break;
				}
			if (!fits) break;
			}
		}
		if (fits) break;
	}

	if (fits)
		piece = rotatedPiece;

}

const std::vector<std::vector<Block>>& Model::getMap() const{
	return map;
}

const Pos Model::getSize() const{
	return Pos {rows, columns};
}

const int32_t Model::getTimer() const{
	return timer;
}

const bool Model::isLanded() const{
	return landed;
}

void Model::clearLanded(){
	landed = false;
}

void Model::removeRowsToRemove(){
	if (rowsToRemove.size() > 0) {
			removedRows += (int) rowsToRemove.size();
			for (size_t rr : rowsToRemove){
				auto it = map.begin()+rr;
				map.erase(it);
				std::vector<Block> newrow;
				for (int i=0; i<columns; ++i){
					newrow.push_back(Block());
				}
				map.insert(map.begin(), newrow);
			}
			score += rowsToRemove.size() * rowsToRemove.size();
			++combos[rowsToRemove.size()-1];
			rowsToRemove.clear();
			//print to console
			std::cout << "Score: " << score << "\n";
			std::cout << "Combos: ";
			for (size_t i=0; i<combos_size; ++i){
				std::cout << i+1 << ": " << combos[i] << " ";
			}
			std::cout << std::endl;
		}

}

void Model::tick(){
	//check for rowsToBeRemoved as found in previous tick
	//removeRowsToRemove();

	landed = false;
	Pos pos = piece.getPos();
	Pos dim = piece.getDims();
	//check if landed
	for (int x=0; x<dim.c; ++x){
		int py = pos.r + piece.getLimits().bottom[x]+1;
		if (!(py<rows) || map[py][pos.c+x].isOccupied())
			landed = true;
	}
	if (!landed)
		piece.fall();
	else {
		//check for game over
		if (pos.r==0){
			gameOver = true;
			landed=false;
			return;
		}

		//copy piece to playArea
		const std::vector<std::vector<Block>>& piecemap = piece.getMap();
		for (int r=0; r<dim.r; ++r){
			for (int c=0; c<dim.c; ++c){
				if (piecemap[r][c].isOccupied() )
					map[pos.r+r][pos.c+c] = piecemap[r][c];
			}
		}

		//check for complete rows
		rowsToRemove.clear();
		for (size_t r=0; r<(unsigned int)rows; ++r){
			int occupied=0;
			for (Block b : map[r]){
				if (b.isOccupied())	++occupied;
			}
			if (occupied == columns) rowsToRemove.push_back(r);
		}


		//next piece
		piece.setMap(nextPiece.getMapNr());
		piece.setPos(Pos(0,columns/2-piece.getDims().c/2));
		nextPiece.setMap(rand()%7+1);


		//check level
		level = removedRows/30 + 1;
		//set timer
		timer = initialTimer - (level-1) * 50;

	}
}
