#include "Score.h"
#include <fstream>

#include <iostream>

int scramble = 3234;

using namespace tetris;
using std::ifstream;
using std::ofstream;
using std::vector;
using std::cout;
using std::endl;
using std::stoi;
using std::stoul;

Score::Score(){
    //load old score from file
    ifstream file("score.dat");
    string fileText;
    std::getline(file, fileText); //the file contains no newline because it was scrambled and \n became something else
    file.close();
    //the file is scrambled, decrypt:
    string decriptedText;
    for (char c : fileText){
        decriptedText += static_cast<char>(static_cast<int>((c)-scramble));
    }
    cout << "decripted score:\n"<< decriptedText;

    //sort the data to vectors of strings
    vector<vector<string>> dat;
    vector<string> row;
    string temp;
    for (int i=0; i<decriptedText.size(); ++i){
        char c = decriptedText[i];
        if (c=='\n' || i+1 == decriptedText.size()){
            row.push_back(temp);
            dat.push_back(row);
            row.clear();
            temp="";
            continue;
        }
        if (c==';'){
            row.push_back(temp);
            temp="";
            continue;
        }
        temp+=c;
    }
    //sort the vector of string to vector of records
    for (vector<string> s : dat){
        record r;
        r.name = s[0];
        r.score = stoi(s[1]);
        r.level = stoi(s[2]);
        r.seed = stoul(s[3]);
        data.push_back(r);
    }
}

int Score::checkScore(int score) const {
    if (score == 0) return 0;
    int pos = data.size()+1; //0;

    for (int i=0; i<data.size(); ++i){
        if (i>10) 
            break;
        if (score > data[i].score){
            pos = i+1;
            break;
        }
    }
    if (pos>10) pos=0;

    return pos;
}

bool Score::saveScore(int pos, string name, int score, int level, unsigned long seed){
    if (pos==0 || pos >10)
        return false;

    record r;
    r.score = score;
    r.level = level;
    r.seed = seed;
    r.name = name;
    data.insert(data.begin()+(pos-1), r);
    
    //keep data length = 10
    while (data.size()>10){
        data.pop_back();
    }

    //save to file
    saveFile();

    return true;
}

bool Score::saveFile(){
    // name;score;level;seed
    ofstream file("score.dat");
    
    string encoded;
    for (record r : data){
        encoded += r.name;
        encoded += ";";
        encoded += std::to_string(r.score);
        encoded += ";";
        encoded += std::to_string(r.level);
        encoded += ";";
        encoded += std::to_string(r.seed);
        encoded += "\n";
    }

    //scamble the string
    string scrambled;
    for (char c : encoded){
        scrambled += static_cast<char>(static_cast<int>(c)+scramble);
    }

    file << scrambled;
    file.close();
    return true;
}


void Score::fillLines(){
    lines.clear();
    for (int i=0; i<data.size(); ++i){
        string s;
        s = std::to_string(i+1) + ". " + data[i].name + " : " + std::to_string(data[i].score);
        lines.push_back(s);
    }
}

std::vector<string> Score::getLines(){
    fillLines();
    return lines;
}

unsigned long Score::getSeed(int pos){
    if (pos < 0 || pos >= data.size()){
        return 0;
    }
    return data.at(pos).seed;
}