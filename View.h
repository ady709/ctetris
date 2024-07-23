/*
 * View.h
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */

#ifndef SRC_VIEW_H_
#define SRC_VIEW_H_

#include "tetris_model.h"
//#include "Piece.h"
namespace tetris{

class View{
public:
	View(Model& m);

	void makePiece();
	void makeMap();
	void makeNextPiece();
	void map2rect(const std::vector<std::vector<Block>>& blockMap, sf::Vector2f zeroPoint, std::vector<std::vector<sf::RectangleShape>>& rectMap);
	void drawAll(sf::RenderWindow& window);
	void movePiece(sf::Time frameTime);
	bool isPieceMoveAnimated(){return pieceMoveAnimating;};
	void pieceOnNextPiece();
	void anim1();


private:
	Model& model;
	std::vector<std::vector<sf::RectangleShape>> piece;
	sf::Vector2f piecePos;
	float pieceMoveSpeed;
	bool pieceMoveAnimating;

	std::vector<std::vector<sf::RectangleShape>> map;
	std::vector<std::vector<sf::RectangleShape>> nextPiece;
	sf::Vector2f nextPiecePos;
	float blockSize;
	Pos oldModelPos;
	sf::Vector2f reqdPos;

};




}




#endif /* SRC_VIEW_H_ */
