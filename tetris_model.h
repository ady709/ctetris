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
	const Pos getSize() const;
	const int32_t getTimer() const;
	const bool isLanded() const;
	void clearLanded();

private:
	std::vector<std::vector<Block>> map;
	Piece piece;
	Piece nextPiece;
	const int rows;
	const int columns;
	int32_t timer;
	int32_t initialTimer;
	std::vector<size_t> rowsToRemove;
	int removedRows, score, level;
	int *combos;
	size_t combos_size;
	bool landed;
};





}// end namespace tetris
#endif /* SRC_TETRIS_MODEL_H_ */
