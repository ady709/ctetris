#ifndef SCORE_H_
#define SCORE_H_

#include <vector>
#include <string>

using std::vector;
using std::string;

namespace tetris
{

struct record{
    string name;
    int score;
    int level;
    unsigned long seed;
};

class Score{
public:
    Score();
    int checkScore(int score) const;
    bool saveScore(int pos, string name, int score, int level, unsigned long seed);
    vector<string> getLines();
    unsigned long getSeed(int pos);


private:
    bool saveFile();
    vector<record> data;
    vector<string> lines;
    void fillLines();

}; //class Score



} // namespace tetris




#endif //SCORE_H_