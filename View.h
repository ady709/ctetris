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


private:
	Model& model;
	std::vector<sf::RectangleShape> piece;
	std::vector<std::vector<sf::RectangleShape>> map;
};




}




#endif /* SRC_VIEW_H_ */
