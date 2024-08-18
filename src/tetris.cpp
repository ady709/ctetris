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
#include "Piece.h"
#include<ctime>

//game and view properties
const int playRows = 21;
const int playColumns = 10;
const int blockSize = 25;
const float initialSidePeriod = 200.f, initialDownPeriod = 100.f;
float sidePeriod = initialSidePeriod, downPeriod = initialDownPeriod;


using namespace tetris;

void tick();
Model model(playRows, playColumns);
sf::Clock tickc;

int main(){

	enum GameStatus {stopped, running, paused};
	GameStatus gameStatus = stopped;
	
	View view(model);
	unsigned int windowWidth = (playColumns * blockSize) + (Piece::getLongestDim()*blockSize) + (view.getWidestLabel()) + 0;
	sf::RenderWindow window(sf::VideoMode(windowWidth, playRows * blockSize), "Tetris", sf::Style::Default);
	


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
				srand(time(0));
				model.init();
				view.makePiece();
				view.pieceOnNextPiece();
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
			//Right
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !left && !right){
				right = true;
				model.goRight();
				leftrightc.restart();
			}
			// Down - fall
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && !down){
				down = true;
				tick(); //change timer instead of directly invoking tick
				downc.restart();
			}
			//Up - rotate
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) && !up){
				up = true;
				model.rotate();
				view.makePiece();
				upc.restart();
			}

		}
		//clear keys
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
			left = false;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
			right = false;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
			down = false;
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
			up = false;

		//clear periods
		if (!left && !right){
			sidePeriod = initialSidePeriod;
		}
		if (!down){
			downPeriod = initialDownPeriod;
		}

		//repeated controls
		if (left && leftrightc.getElapsedTime().asMilliseconds() >= (int32_t) sidePeriod) {
			model.goLeft();
			sidePeriod -= (initialSidePeriod*0.2f);
			if (sidePeriod < initialSidePeriod* 0.5f ) sidePeriod = initialSidePeriod*0.5f;
			leftrightc.restart();
		}

		if (right && leftrightc.getElapsedTime().asMilliseconds() >= (int32_t) sidePeriod) {
			model.goRight();
			sidePeriod -= (initialSidePeriod*0.2f);
			if (sidePeriod < initialSidePeriod* 0.5f ) sidePeriod = initialSidePeriod*0.5f;
			leftrightc.restart();
		}
		if (down && downc.getElapsedTime().asMilliseconds() >= (int32_t) downPeriod) {
			tick();
			downPeriod -= (initialDownPeriod*0.2f);
			if (downPeriod < initialDownPeriod* 0.4f ) downPeriod = initialDownPeriod*0.4f;
			downc.restart();
		}
		if (up && upc.getElapsedTime().asMilliseconds() >= 250) {
			model.rotate();
			upc.restart();
		}

		//tick ***************************************************************************************8
		if (tickc.getElapsedTime().asMilliseconds() >= model.getTimer() && gameStatus == running){
			tick();
		}
		//stop if game over
		if (model.isGameOver()){
			gameStatus = stopped;
		}

		//move new piece over next piece for it to nicely slide to position from there
		if (model.isLanded()){
			view.pieceOnNextPiece();
			model.clearLanded();
		}

		//movepiece and reset timer if animating
		view.movePiece(framet);
		if (view.isPieceMoveAnimated())
			tickc.restart();


		//re-draw window
		window.clear();

		//draw map
		if (!view.isAnimRunning()) {
			view.makeMap();
		}
		//animation of disappearing complete rows
		if (model.getRowsToRemove().size() > 0 && !view.isAnimRunning()){
			view.setAnim(model.getRowsToRemove().size()%2+1);
		}
		if (view.isAnimRunning()){
			view.doAnim(framet);
		}
		//draw the rest
		view.makeNextPiece();
		view.drawAll(window);

		window.display();

		//frame timer
		framet = framec.restart();

	}//end main loop



	return 0;
} //end main

void tick(){
	model.tick();
	tickc.restart();
}






