/*
 * View.cpp
 *
 *  Created on: Jul 9, 2024
 *      Author: ady709
 */

#include "View.h"
#include <iostream>
#include <vector>
#include <sstream>
using namespace tetris;
using std::to_string; // required
using std::vector; //required
using std::string; //required
using std::stringstream; //required
using std::cout;
using std::endl;

View::View(Model& m) : model(m) {
	blockSize = 25.f;
	//piece;
	piecePos = sf::Vector2f(-1.f, -1.f);
	pieceMoveSpeed = 0.f;
	pieceMoveAnimating = false;

	//nextPiece;
	nextPiecePos = Pos(2, model.getSize().c+1)*blockSize;


	oldModelPos = Pos(-1, -1);

	animRunning = false;
	initialAnimStep = 0.f;
	animStep = 0.f;
	currentAnimNr = 0;
	
	labelFont = sf::Font();
	//labelFont.loadFromFile("arial.ttf");
	vector<string> fontList = {	"font.ttf",
								"/usr/share/fonts/truetype/freefont/FreeSerifBold.ttf",
								"/usr/share/fonts/truetype/freefont/FreeSansOblique.ttf",
								"/usr/share/fonts/truetype/freefont/FreeSerif.ttf"};
								// add windows path
	bool fontLoaded = false;
	for (string f : fontList){
		if (labelFont.loadFromFile(f)) {
			fontLoaded = true;
			break;
		}
	}
	if (!fontLoaded) cout << "Error loading font!\n";

	float nextToPlayField = (float) (blockSize*model.getSize().c)+blockSize;
	//Texts
	//nextLabel is also used to display welcome text and Game Over!
	nextLabel = sf::Text("", labelFont, blockSize);
	nextLabel.setFillColor(sf::Color(194, 21, 21));
	nextLabel.setPosition(sf::Vector2f(nextToPlayField, (float) blockSize*0.7f));

	vector<float> rowPos;
	rowPos.push_back((float) (Piece::getLongestDim())*blockSize + nextPiecePos.y + blockSize);
	size_t currentPos = 0;

	scoreLabelT = sf::Text("Score:", labelFont, blockSize);
	scoreLabelT.setFillColor(sf::Color(194, 21, 21));
	scoreLabelT.setOrigin(0.f, (float) blockSize);
	scoreLabelT.setPosition(sf::Vector2f(nextToPlayField, rowPos[currentPos]));
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);

	levelLabelT = sf::Text("Level:",labelFont, blockSize);
	levelLabelT.setFillColor(sf::Color(194, 21, 21));
	levelLabelT.setOrigin(0.f, (float) blockSize);
	levelLabelT.setPosition(sf::Vector2f(nextToPlayField, rowPos[currentPos]));
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);
	
	rowsLabelT = sf::Text("Total rows:",labelFont, blockSize);
	rowsLabelT.setFillColor(sf::Color(194, 21, 21));
	rowsLabelT.setOrigin(0.f, (float) blockSize);
	rowsLabelT.setPosition(sf::Vector2f(nextToPlayField, rowPos[currentPos]));
	++currentPos;

	float nextToLabels = (float) nextToPlayField + getWidestLabel() + (float) blockSize*0.4f;
	//Values
	currentPos = 0;
	scoreLabelV = sf::Text("123",labelFont, blockSize*0.8f);
	scoreLabelV.setFillColor(sf::Color(194, 21, 21));
	scoreLabelV.setOrigin(0.f, (float) blockSize*0.8f);
	scoreLabelV.setPosition(sf::Vector2f(nextToLabels, rowPos[currentPos]));
	++currentPos;

	levelLabelV = sf::Text("123",labelFont, blockSize*0.8f);
	levelLabelV.setFillColor(sf::Color(194, 21, 21));
	levelLabelV.setOrigin(0.f, (float) blockSize*0.8f);
	levelLabelV.setPosition(sf::Vector2f(nextToLabels, rowPos[currentPos]));
	++currentPos;

	rowsLabelV = sf::Text("123",labelFont, blockSize*0.8f);
	rowsLabelV.setFillColor(sf::Color(194, 21, 21));
	rowsLabelV.setOrigin(0.f, (float) blockSize*0.8f);
	rowsLabelV.setPosition(sf::Vector2f(nextToLabels, rowPos[currentPos]));
	++currentPos;

	//combos
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);

	combosT = sf::Text("Combos (1,2,3,4..)",labelFont, blockSize);
	combosT.setFillColor(sf::Color(194, 21, 21));
	combosT.setOrigin(0.f, (float) blockSize);
	combosT.setPosition(sf::Vector2f(nextToPlayField, rowPos[currentPos]));
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);

	combosV = sf::Text("1,2,3,4",labelFont, blockSize);
	combosV.setFillColor(sf::Color(194, 21, 21));
	combosV.setOrigin(0.f, (float) blockSize);
	combosV.setPosition(sf::Vector2f(nextToPlayField+blockSize, rowPos[currentPos]));
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);

	//highScore
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize);
	highScoreLabel = sf::Text("High score:", labelFont, blockSize*0.8);
	highScoreLabel.setFillColor(sf::Color(194, 21, 21));
	highScoreLabel.setOrigin(0.f, (float) blockSize*0.8);
	highScoreLabel.setPosition(sf::Vector2f(nextToPlayField, rowPos[currentPos]));
	++currentPos;
	rowPos.push_back(rowPos[currentPos-1]+(float)blockSize*0.8);

	for (int i=0; i<View::highScoreRows; ++i){
		highScore[i] = sf::Text("",labelFont, blockSize*0.6f);
		highScore[i].setFillColor(sf::Color(194, 21, 21));
		highScore[i].setOrigin(0.f, blockSize*0.6f);
		highScore[i].setPosition(nextToPlayField, rowPos[currentPos]);
		++currentPos;
		rowPos.push_back(rowPos[currentPos-1]+(float)blockSize*0.6f);
	}
	highScoreUsed = 0;
	highScoreHighlighted = -1;
	highScoreSelected = -1;

}

void View::updateHighScoreLabels(std::vector<std::string> lines){
	highScoreUsed = 0;
	for (int i=0; i<View::highScoreRows; ++i){
		if (i<lines.size()){
			highScore[i].setString(lines[i]);
			++highScoreUsed;
		} else {
			highScore[i].setString("");
		}
	}
}

void View::mouseInHighScore(sf::Vector2f mousePos){
	highScoreHighlighted = -1;
	for (int i=0; i<highScoreUsed; ++i){
		sf::FloatRect bounds = highScore[i].getGlobalBounds();
		if (bounds.contains(mousePos)){
			if(i != highScoreSelected){
				highScore[i].setFillColor(sf::Color(128, 118, 31,255));
			}
			highScoreHighlighted = i;
		}
		else{
			if(i != highScoreSelected){
				highScore[i].setFillColor(sf::Color(194, 21, 21));
			}
		}
	}
}

int View::selectHighScore(){
	if (highScoreHighlighted == highScoreSelected){
		highScore[highScoreSelected].setFillColor(sf::Color(194, 21, 21));
		highScoreSelected = -1;
		return highScoreSelected;
	}
	if (highScoreHighlighted > -1){
		highScore[highScoreHighlighted].setFillColor(sf::Color(222, 201, 20,255));
		highScore[highScoreSelected].setFillColor(sf::Color(194, 21, 21));
		highScoreSelected = highScoreHighlighted;
		return highScoreSelected;
	}
	return 0;
}

float View::getWidestLabel() const{
	//sf::Text textLabels[] = {scoreLabelT, levelLabelT};
	float longestText = 0;
	float length = 0;
	for (auto t : textLabels){
		length = t->getGlobalBounds().width;
		if (length > longestText) longestText = length;
	}
	return longestText;
}

void View::map2rect(const std::vector<std::vector<Block>>& blockMap, sf::Vector2f zeroPoint, std::vector<std::vector<sf::RectangleShape>>& rectMap){
	//for each block in map, make rectangle
	rectMap.clear();
	std::vector<sf::RectangleShape> rectRow;
	int r=0;
	for (std::vector<Block> row : blockMap){
		rectRow.clear();
		int c=0;
		for (Block b : row){
			if (b.isOccupied()){
				sf::RectangleShape rect(sf::Vector2f(blockSize, blockSize));
				rect.setPosition((float) (zeroPoint.x+c*blockSize), (float) (zeroPoint.y+r*blockSize));
				rect.setFillColor(b.getColor());
				rect.setOutlineColor(sf::Color(sf::Color::Black));
				rect.setOutlineThickness(-0.f);
				rectRow.push_back(rect);
			}
			++c;
		}
		rectMap.push_back(rectRow);
		++r;
	}
}


void View::makePiece(){
	piecePos = model.getPiece().getPos() * blockSize;
	map2rect(model.getPiece().getMap(), piecePos, piece);
	oldModelPos = model.getPiece().getPos();
}
void View::makeMap(){
	Pos pos = {0,0};
	map2rect(model.getMap(), pos*blockSize, map);

}
void View::makeNextPiece(){
	map2rect(model.getNextPiece().getMap(), nextPiecePos, nextPiece);
}

void View::drawAll(sf::RenderWindow& window){

	//box around play area
	sf::RectangleShape bx(sf::Vector2f(model.getSize().c*blockSize, model.getSize().r*blockSize));
	bx.setFillColor(sf::Color(121,121,121,255));
	bx.setOutlineColor(sf::Color(sf::Color::White));
	bx.setOutlineThickness(-1.f);
	//bx.setPosition(1.f, 1.f);
	window.draw(bx);

	//draw all rectangles, using pointer array to loop through all objects
	std::vector<std::vector<sf::RectangleShape>>* objects[] = {&map, &piece, &nextPiece};
	for (auto& obj : objects){
		for (auto row : *obj){
			for (sf::RectangleShape b : row){
				window.draw(b);
			}
		}
	}

	//draw texts
	//Next:
	window.draw(nextLabel);
	//score
	scoreLabelV.setString(to_string(model.getScore()));
	window.draw(scoreLabelT);
	window.draw(scoreLabelV);
	//Level
	levelLabelV.setString(to_string(model.getLevel()));
	window.draw(levelLabelT);
	window.draw(levelLabelV);
	//rows removed
	rowsLabelV.setString(to_string(model.getRemovedRows()));
	window.draw(rowsLabelT);
	window.draw(rowsLabelV);
	//Combos
	stringstream ss;
	for (size_t i=0; i<model.getCombosSize(); ++i){
		ss << model.getCombos()[i];
		if (i<model.getCombosSize()-1)
			ss << ",";
	}
	combosV.setString(ss.str());
	window.draw(combosT);
	window.draw(combosV);
	//highscore
	if (highScoreUsed){
		window.draw(highScoreLabel);
		for (int i=0; i<View::highScoreRows; ++i){
			if (highScore[i].getString()!="")
				window.draw(highScore[i]);
		}
	}

}


void View::movePiece(sf::Time frameTime){
	static float speedx;
	static float speedy;

	Pos newModelPos = model.getPiece().getPos();
	sf::Vector2f newPos = newModelPos * blockSize;

	if ( !(piecePos == newPos) ){
		pieceMoveAnimating = true;

		//distance between old and new pos
		float dx = (float) newPos.x - piecePos.x;
		float dy = (float) newPos.y - piecePos.y;

		//speed of movement
		speedx = (dx / 6e4) * frameTime.asMicroseconds();
		speedy = (dy / 6e4) * frameTime.asMicroseconds();

		piecePos.x += speedx;
		piecePos.y += speedy;

		map2rect(model.getPiece().getMap(), piecePos, piece);

		if (abs(newPos.x - piecePos.x) < 1.f && abs(newPos.y - piecePos.y) <1.f ){
			makePiece();
		}

	}
	//resume ticks postponed during animation downwards
	if (abs(newPos.y - piecePos.y) <1.f ){
		pieceMoveAnimating = false;
	}

}


void View::pieceOnNextPiece(){
	piecePos.x = nextPiecePos.x;
	piecePos.y = nextPiecePos.y;
}


void View::doAnim(sf::Time frameTime){
	static int64_t animTime = 5e5;

	float delta = (float) (initialAnimStep/(float)animTime) * (float)frameTime.asMicroseconds();
	animStep -= delta;

	if (currentAnimNr==1 || currentAnimNr==2){

		for (size_t rtr : model.getRowsToRemove()){
			for (sf::RectangleShape& rect : map[rtr]){
				if (currentAnimNr==1){
					sf::Color c = rect.getFillColor();
					sf::Color n(c.r, c.g, c.b, (uint8_t)animStep);
					rect.setFillColor(n);
				} else if (currentAnimNr==2){
					sf::Vector2f size(rect.getSize());
					sf::Vector2f pos(rect.getPosition());
					size.x -= delta;
					size.y -= delta;
					pos.x += delta/2;
					pos.y += delta/2;
					rect.setSize(size);
					rect.setPosition(pos);
				}
			}
		}
		//finish
		if (animStep < 1.f){
			animRunning = false;
			model.removeRowsToRemove();
		}
	}

}

void View::setAnim(int animNr){
	currentAnimNr = animNr;
	switch (animNr) {
	case 1:
		initialAnimStep = 255.f;
		break;
	case 2:
		initialAnimStep = blockSize;
		break;
	}

	animStep = initialAnimStep;
	animRunning = true;
}




bool View::inputBox(sf::RenderWindow& window, string message, string& target){
	//bool windowOpen = true;
	//takes full control over the window mainloop!


	sf::Vector2u winSizeu = window.getSize();
	sf::Vector2f winSize = sf::Vector2f((float) winSizeu.x, (float) winSizeu.y);
	//message text
	sf::Text messageText(message, labelFont, blockSize);
	sf::FloatRect messTextSize = messageText.getGlobalBounds();
	//scale down the text untill it fits into window
	while (messTextSize.width > (winSize.x-(float) (blockSize/2)) || messTextSize.height > (winSize.y/3)-(float) (blockSize/2)){
		//messageText.scale(sf::Vector2f(0.99f, 0.99f)); //weird, does not work as expected
		if (messageText.getCharacterSize() > 2)
			messageText.setCharacterSize(messageText.getCharacterSize()-1);
		messTextSize = messageText.getGlobalBounds();
	}
	messageText.setOrigin(messTextSize.width/2.f, 0.f);

	//input text
	sf::Text inputText("",labelFont,blockSize);
	//height of texts
	float height = messTextSize.height + (float) blockSize *2.f;
	//top position
	float top = winSize.y/2 - height/2;
	//underlying rectangle
	sf::RectangleShape backGround(sf::Vector2f(winSize.x-12.f, height));
	backGround.setPosition(6.f,top);
	backGround.setFillColor(sf::Color(189,139,19,200));
	backGround.setOutlineColor(sf::Color(92,69,21,220));
	backGround.setOutlineThickness((float) 0-(blockSize/4));
	//place messageText
	messageText.setPosition(winSize.x/2.f, top+(float) (blockSize/3));
	//input text
	float inputTextTop = top + ((float) (blockSize/2)) + messTextSize.height;

	messageText.setFillColor(sf::Color::Black);
	inputText.setFillColor(sf::Color::Black);

	// //alternative approach
	// sf::Text text("",labelFont,blockSize);
	// text.setFillColor(sf::Color(189,139,19,160));

	std::string input;
    std::string printable = "1234567890qwertyuiop[]asdfghjkl;'\\zxcvbnm,./QWERTYUIOPASDFGHJKLZXCVBNM{}:\"|<>?`~!@#$%^&*()_+ ";
    char c;
	bool enter = false;

	while (window.isOpen()){
		sf::Event event;
		while (window.pollEvent(event)){
			if (event.type == sf::Event::Closed){
				window.close();
				target = input;
				return false;
			}


 			if (event.type == sf::Event::TextEntered)
            {
                c = static_cast<char>(event.text.unicode);
                if (printable.find_first_of(c) != std::string::npos) //only printable characters as defined in the string printable
                {
                    if (input.size() < 16) input += c;
                }
                if (event.text.unicode == 8){ // backspace
                    if (input.size() > 0) input.pop_back();
                }
                if (event.text.unicode == 13){ // enter
                    enter=true;
                }
            }


		} //end event handling

		inputText.setString(input);
		inputText.setOrigin(inputText.getGlobalBounds().width/2, 0);
		inputText.setPosition(winSize.x/2, inputTextTop);


		// text.setString(message + "\n" + input);
		// text.setOrigin(text.getGlobalBounds().width/2, text.getGlobalBounds().height/2);
		// text.setPosition(winSize.x/2, winSize.y/2);

		window.clear();
		drawAll(window);
		// window.draw(text);
		window.draw(backGround);
		window.draw(messageText);
		window.draw(inputText);
		window.display();

		if (enter) break;
	}//end mainloop

	target = input;
	return true;
}
