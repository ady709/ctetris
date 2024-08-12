/*
 * Piece.h
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */

#ifndef SRC_PIECE_H_
#define SRC_PIECE_H_

#include "Block.h"
#include <vector>
#include <SFML/Graphics.hpp>
namespace tetris{

struct Pos{
	int r; int c;
	Pos(int rr, int cc) : r(rr), c(cc){}
	Pos() : r(0),c(1){};
	sf::Vector2f operator*(float a) const {
		sf::Vector2f p(1.f,1.f);
		p.y = r*a;
		p.x = c*a;
		return p;
	}
	bool operator==(Pos p) const {
		return r==p.r && c==p.c;
	}
};

struct Limits {
	std::vector<int> left;
	std::vector<int> bottom;
	std::vector<int> right;

};


class Piece{
public:
	//Piece();
	Piece(std::vector<std::vector<Block>> map, Pos pos={0,0}); //construct from map
	Piece(int mapNr=0);

	void setMap(int MapNr=0);
	int getMapNr() const;

	static size_t getLongestDim();

	void goLeft();
	void goRight();
	void fall();

	void shiftPos(Pos pos);
	Piece getRotated() const;

	const std::vector<std::vector<Block>>& getMap() const;
	const Pos getPos() const;
	const Pos getDims() const;
	void setPos(Pos p);
	const Limits& getLimits() const;


private:
	std::vector<std::vector<Block>> map;
	int r;
	int c;
	int width;
	int height;
	int mapNr;
	Limits limits;
	void init();
	void setLimits();
};

}




#endif /* SRC_PIECE_H_ */
