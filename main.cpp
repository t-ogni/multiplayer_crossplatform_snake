const int MAX_SNAKES = 4;
const int MAX_TAIL = 100;
const int MAX_FRUITS = 10;

#include <iostream>
#include <string>
#include <ctime>

#ifdef _WIN32
    #include "conio.h"
#elif
    #include "linuxGetch.h"
#endif

#include "kbhit3.h"
#include "Ccolors.h"

template <typename T>
void sleep(T togo){
    // microseconds to milliseconds
    togo = togo*1000+clock();
    while(clock() < togo);
    return;
}

enum moves {
        Forward = 0,
        Backward,
        Left,
        Right,
        KEYS_LENGTH
    };

class snake{
// variables //
public:
    int x, y;
    std::string name;
    int tail[MAX_TAIL][2];
    int tailLength = 0;
    char headSymbol = '@', tailSymbol = '#';
    int savedKey = Forward;
    std::string color = colors::FG::Green;
    bool isAlive = true;

private:
    int keyMoves[KEYS_LENGTH] = {49, 50, 51, 52};


// functions //
public:
    snake(){}

    snake(int x, int y, std::string newName){
        this-> x = x;
        this-> y = y;
        //1-st segment is head
        this-> tail[tailLength][0] = x;
        this-> tail[tailLength][1] = y;
        this-> name = newName;
    };

    void setKeys(std::string controls) {
        keyMoves[Forward] = controls[Forward];
        keyMoves[Backward] = controls[Backward];
        keyMoves[Left] = controls[Left];
        keyMoves[Right] = controls[Right];
    }

    void customising(char head='@', char tail='#',std::string color = colors::FG::Green){
        this-> headSymbol = head;
        this-> tailSymbol = tail;
        this-> color = color;
    }

    void moveTo(int x, int y){
        for (int Segment = tailLength; Segment >= 0; Segment--)
        {
            if(tailLength < MAX_TAIL){
                tail[Segment+1][0] = tail[Segment][0];
                tail[Segment+1][1] = tail[Segment][1];
            }
        }

        // head
        tail[0][0] = x;
        tail[0][1] = y;
        this-> x = x;
        this-> y = y;
        return;
    }

    void previousMove(){
        handleKey(savedKey);
    }

    void handleKey(int key){
        if(key == keyMoves[Forward]){
                this-> moveTo(this-> x, this-> y-1);
                savedKey = key;
        } else if (key == keyMoves[Backward]){
                this-> moveTo(this-> x, this-> y+1);
                savedKey = key;
        } else if (key == keyMoves[Left]){
                this-> moveTo(this-> x-1, this-> y);
                savedKey = key;
        } else if (key == keyMoves[Right]){
                this-> moveTo(this-> x+1, this-> y);
                savedKey = key;
        }
    }

    void addSegment(){
        tailLength++;
        tail[tailLength][0] = -1;
        tail[tailLength][1] = -1;
    }

    bool isCollisedItself(){
        for (int i = 0; i < tailLength; i++)
        {
            if(this-> x == tail[i][0] &&
                this-> y == tail[i][1]){
                    return true;
                }
        }

        return false;
    }

};

class board{
public:
    int width, height;
    int snakesOnboard = 0;
    snake *snakes[MAX_SNAKES] {};

private:
    int fruits[MAX_FRUITS][2];
    int fruitsInGame = 0;

public:
    board(int w, int h){
        width = w;
        height = h;
    }

    bool addSnake(snake *snakePtr){
        if( snakesOnboard < MAX_SNAKES){
            for (int i = 0; i < snakesOnboard; i++)
            {
                if(snakes[i]-> name == snakePtr -> name)
                    return false;
            }

            snakes[snakesOnboard++] = snakePtr;
            return true;
        } else {
            return false;
        }
    }

    void draw(){
        bool needToDraw = true;

        system("clear");

        for (int i = 0; i < snakesOnboard; i++)
        {
            std::cout << snakes[i]-> name << " ("<<i<<"): "<<
                    snakes[i]-> x << " - " << snakes[i]-> y  << " | ";
        }
        std::cout << std::endl;
        // -4 because need to draw 4 more "#" of 2 corners
        for (int i = -4; i < this->width; i++)
        {
            std::cout << colors::FG::White << colors::BG::White << "#" << colors::Clear << colors::BG::Black;
        }

        std::cout << "\n";

        for (int y = 0; y < this-> height; y++)
        {
            std::cout << colors::FG::White << colors::BG::White << "##" << colors::Clear << colors::BG::Black;
            for (int x = 0; x < this-> width; x++)
            {
                for (int i = 0; i < this-> snakesOnboard; i++)
                {
                    if (snakes[i]->x == x && snakes[i]->y == y &&
                                needToDraw)
                    {
                        std::cout << snakes[i]->color << snakes[i]->headSymbol << colors::Clear << colors::BG::Black;
                        needToDraw = false;
                        break;
                    } else	{
                        for (int Segment = 0; Segment < snakes[i]->tailLength+1; Segment++)
                        {
                            if( snakes[i]->tail[Segment][0] == x &&
                                snakes[i]->tail[Segment][1] == y &&
                                needToDraw)
                            {
                                std::cout << snakes[i]->color << snakes[i]->tailSymbol << colors::Clear << colors::BG::Black;
                                needToDraw = false;
                                break;
                            }
                        }
                        for (int i = 0; i < fruitsInGame; i++)
                        {
                            if(fruits[i][0] == x &&
                                fruits[i][1] == y &&
                                needToDraw){
                                    std::cout << colors::FG::Red << "F" << colors::Clear << colors::BG::Black;
                                    needToDraw = false;
                                    break;
                            }
                        }
                    }
                }
                if (needToDraw)
                {
                    std::cout << " ";
                } else {
                    needToDraw = true;
                }

            }
            std::cout << colors::FG::White << colors::BG::White << "##" << colors::Clear << colors::BG::Black << "\n";
        }

        // -4 because need to draw 4 more "#" of 2 corners
        for (int i = -4; i < this->width; i++)
        {
            std::cout << colors::FG::White << colors::BG::White << "#" << colors::Clear << colors::BG::Black;
        }
        std::cout << "\n";
    }

    void handleKboard(int key){
        for (int i = 0; i < snakesOnboard; i++)
        {
            //if(snakes[i]->isAlive){
                snakes[i]->handleKey(key);
            //}
        }

    }

    void addFruit(){
        if(fruitsInGame < MAX_FRUITS ){
            fruits[fruitsInGame][0] = rand() % width;
            fruits[fruitsInGame][1] = rand() % height;
            fruitsInGame++;
        }
    }

    void replaceFruit(int id){
        for (int i = id; i < fruitsInGame; i++)
        {
            fruits[i][0] = fruits[i+1][0];
            fruits[i][1] = fruits[i+1][1];
        }
        fruitsInGame--;
    }

    snake *getSnakeByName(std::string name){
        for (int i = 0; i < snakesOnboard; i++)
        {
            if(snakes[i]-> name == name){
                return snakes[i];
            }
        }
        return nullptr;
    }

    void nextTurns(){
        for (int i = 0; i < snakesOnboard; i++)
        {
            snakes[i]-> previousMove();
        }
    }

    void eatingCheck(){
        for (int f = 0; f < fruitsInGame; f++)
        {

            for (int i = 0; i < snakesOnboard; i++)
            {
                if (snakes[i]->x == fruits[f][0] &&
                    snakes[i]->y == fruits[f][1])
                {
                    snakes[i]-> addSegment();
                    replaceFruit(f);
                }
            }
        }
    }

    void diedsCheck(){
        for (int i = 0; i < snakesOnboard; i++)
        {
            if(snakes[i]->isCollisedItself()){
                snakes[i]->isAlive = false;
            }
            if(snakes[i]->x < 0 ||
                snakes[i]->x >= width ||
                snakes[i]->y < 0 ||
                snakes[i]->y >= height){
                    snakes[i]->isAlive = false;
            }

        }

    }

    ~board(){
        for (int i = 0; i < snakesOnboard; i++)
        {
            // clear a used memory
            delete snakes[i];
            snakes[i] = nullptr;
        }

    }
};

int main() {
    srand(time(0));
    int w,h;
    std::cout << "width: ";
    std::cin >> w;
    std::cout << "height: ";
    std::cin >> h;
    board *gameboard = new board(w, h);
    gameboard-> addFruit();
    gameboard-> addFruit();
    while(true){
        while(!kbhit()){
            gameboard-> eatingCheck();
            gameboard-> diedsCheck();
            if (rand() % 100 == 0)
                gameboard-> addFruit();
            gameboard-> draw();

            sleep(50);
            if(!kbhit()) // checking for excepting 2 moves a row
                gameboard-> nextTurns();
        }
        int moveTo = getch();
        switch(moveTo){
            case 'p':
                std::cout << "paused";
                while(!kbhit());
                break;
            case '+':{
                system("clear");
                std::cout << "adding new snake, Name? \n >>> ";
                std::string tmpname;
                std::cin >> tmpname;
                snake *sn = new snake(gameboard->width/2,
                                    gameboard->height/2, tmpname);

                if(!gameboard-> addSnake(sn)){
                    std::cout << "cannot add snake." << std::endl;
                    while(!kbhit());
                    break;
                }
                std::cout << "type here control buttons (up, down, left, right) \n :";
                std::string buttons;
                std::cin >> buttons;
                sn-> setKeys(buttons);
                break;
                }
            case '/':{
                system("clear");
                std::cout << "Customisation. \n Your (snake) name? \n >>> ";

                std::string tmpname;
                std::cin >> tmpname;
                snake *sn = gameboard-> getSnakeByName(tmpname);
                char tmphead, tmptail;
                int ncolor;
                std::cout << "\n head: ";
                std::cin >> tmphead;
                std::cout << "\n tail: ";
                std::cin >> tmptail;
                std::cout << "\n 1 - red \n 2 - green \n 3 - yellow \n 4 - blue \n 5 - pink \n 6 - cyan \n 7 - white \n color: ";
                std::cin >>ncolor;
                std::string color;

                // also can made simpler, write here all posibles. But i think its not good
                switch(ncolor){
                    case 1:
                        color = colors::FG::Red;
                        break;
                    case 2:
                        color = colors::FG::Green;
                        break;
                    case 3:
                        color = colors::FG::Yellow;
                        break;
                    case 4:
                        color = colors::FG::Blue;
                        break;
                    case 5:
                        color = colors::FG::Pink;
                        break;
                    case 6:
                        color = colors::FG::Cyan;
                        break;
                    case 7:
                        color = colors::FG::White;
                        break;
                    default:
                        color = colors::FG::Green;
                }
                sn-> customising(tmphead, tmptail, color);
                }
            default:
                gameboard->handleKboard(moveTo);
                break;
        }

    }

    delete gameboard;
    gameboard = nullptr;

    return 0;
}
