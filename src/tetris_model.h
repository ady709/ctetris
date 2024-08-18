/*
 * tetris_model.h
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */
#ifndef SRC_TETRIS_MODEL_H_
#define SRC_TETRIS_MODEL_H_

#include "Block.h"
#include "Piece.h"
#include <vector>


namespace tetris {
class Model{
public:
	Model(const int playRows, const int playColumn);
	void init();
	void tick();
	void goLeft();
	void goRight();
	void rotate();
	const Piece& getPiece() const;
	const Piece& getNextPiece() const;
	const std::vector<std::vector<Block>>& getMap() const;
	
	/*Returns dims of the playfield in rows and columns
	  Return type is Pos, members of Pos are .r and .c*/
	const Pos getSize() const;
	
	const int32_t getTimer() const;
	const bool isLanded() const;
	void clearLanded();
	const std::vector<size_t> getRowsToRemove() const {return rowsToRemove;}
	void removeRowsToRemove();
	const bool isGameOver() const {return gameOver;}
	const int getScore() const {return score;}
	const int getLevel() const {return level;}
	const int getRemovedRows() const {return removedRows;}
	const size_t getCombosSize() const {return combos_size;}
	const int* getCombos() const {return combos;}


private:
	std::vector<std::vector<Block>> map;
	Piece piece;
	const int rows;
	Piece nextPiece;
	const int columns;
	int32_t timer;
	int32_t initialTimer;
	std::vector<size_t> rowsToRemove;
	int removedRows, score, level;
	int *combos;
	size_t combos_size;
	bool landed;
	bool gameOver;
	const bool fitsIn(const Piece& pc) const;
};





}// end namespace tetris
#endif /* SRC_TETRIS_MODEL_H_ */
