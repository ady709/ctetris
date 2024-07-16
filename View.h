/*
 * View.h
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */

#ifndef SRC_VIEW_H_
#define SRC_VIEW_H_

#include "tetris_model.h"
namespace tetris{

class View{
public:
	View(Model& m);
	void updatePiece(sf::RenderWindow& window, int blockSize=25);
	void updateMap(sf::RenderWindow& window, int blockSize=25);

	void makePiece();
	void makeMap();
	void makeNextPiece();
	void map2rect(const std::vector<std::vector<Block>>& blockMap, Pos zeroPoint, int blockSize, std::vector<std::vector<sf::RectangleShape>>& rectMap);
	void drawAll(sf::RenderWindow& window);


private:
	Model& model;
	std::vector<std::vector<sf::RectangleShape>> piece;
	std::vector<std::vector<sf::RectangleShape>> map;
	std::vector<std::vector<sf::RectangleShape>> nextPiece;
};




}




#endif /* SRC_VIEW_H_ */
