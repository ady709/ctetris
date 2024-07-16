/*
 * tetris.cpp
 *
 *  Created on: Jul 7, 2024
 *      Author: ady709
 */
#include <iostream>
#include <SFML/Graphics.hpp>
#include "tetris_model.h"
#include "View.h"

//game and view properties
const int playRows = 21;
const int playColumns = 10;
const int blockSize = 25;
const int nextPieceBlockSize = 25;
const int nextPieceViewBlocks = 4;
using namespace tetris;

int main(){

	Model model(playRows, playColumns);


	sf::RenderWindow window(sf::VideoMode(playColumns * blockSize + nextPieceBlockSize * nextPieceViewBlocks + 40, playRows * blockSize), "Tetris", sf::Style::Default);
	View view(model);


	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}

	        //keyboard
	        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
	        	model.goLeft();
	        }
	        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
	        	model.goRight();
	        }
	        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)){
	        	model.tick();
	        }
	        else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)){
	        	model.rotate();
	        }
	        else {

	        }

		} //end event handling

		window.clear();

		//view.updatePiece(window, blockSize);
		//view.updateMap(window, blockSize);
		view.makePiece();
		view.makeMap();
		view.drawAll(window);

		window.display();



	}//end main loop



	return 0;
} //end main


void drawPiece(Model& model){


}






