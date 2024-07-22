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

void tick();
Model model(playRows, playColumns);
sf::Clock tickc;

int main(){

	enum GameStatus {stopped, running, paused};
	GameStatus gameStatus = stopped;

	sf::RenderWindow window(sf::VideoMode(playColumns * blockSize + nextPieceBlockSize * nextPieceViewBlocks + 40, playRows * blockSize), "Tetris", sf::Style::Default);
	View view(model);


	//timers
	sf::Clock leftrightc, upc, downc;
	sf::Clock framec;
	sf::Time framet = framec.restart();


	//keyboard booleans
	bool spaceDown=false, left=false, right=false, down=false, up = false;

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
			}
		} //end event handling

		//keyboard
		//Space - Play / Pause
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space) && !spaceDown){
			if (gameStatus == stopped) {
				model.init();
				view.makePiece();
				gameStatus = running;
				tickc.restart();
			} else if (gameStatus == running){
				gameStatus = paused;
			} else if (gameStatus == paused){
				gameStatus = running;
				tickc.restart();
			}
			std::cout << "GameStatus: " << gameStatus << std::endl;
			spaceDown = true;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) spaceDown = false;


		if (gameStatus == running) {
			//Left
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !left && !right){
				left = true;
				model.goLeft();
				leftrightc.restart();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
				left = false;
			//Right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !left && !right){
				right = true;
				model.goRight();
				leftrightc.restart();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
				right = false;
			// Down - fall
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down){
				down = true;
				tick(); //change timer instead of directly invoking tick
				downc.restart();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
				down = false;
			//Up - rotate
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up){
				up = true;
				model.rotate();
				view.makePiece();
				upc.restart();
			}
			if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
				up = false;
		}

		//repeated controls
		if (left && leftrightc.getElapsedTime().asMilliseconds() >= 200) {
			model.goLeft();
			leftrightc.restart();
		}
		if (right && leftrightc.getElapsedTime().asMilliseconds() >= 200) {
			model.goRight();
			leftrightc.restart();
		}
		if (down && downc.getElapsedTime().asMilliseconds() >= 100) {
			tick();
			downc.restart();
		}
		if (up && upc.getElapsedTime().asMilliseconds() >= 250) {
			model.rotate();
			upc.restart();
		}

		//tick
		if (tickc.getElapsedTime().asMilliseconds() >= model.getTimer() && gameStatus == running)
			tick();

		//test
		if (model.isLanded())
			view.makePiece();

		window.clear();
		//frame timer
		framet = framec.restart();


		//movepiece and reset timer if animating
		view.movePiece(framet);
		if (view.isPieceMoveAnimated())
			tickc.restart();



		view.makeMap();
		view.makeNextPiece();
		view.drawAll(window);

		window.display();



	}//end main loop



	return 0;
} //end main

void tick(){
	model.tick();
	tickc.restart();
}






