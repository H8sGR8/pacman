#include "sprite.h"

#include <QDebug>

Sprite::Sprite(pair<int, int> startingPoint, QWidget* parent) : QWidget(parent){
    this->startingPoint = startingPoint;
    map.open("map.txt");
    string row;
    for(int i = 0; i < 93; i++){
        getline(map, row);
        if((i - 1)%3 != 0) continue;
        for (int j = 1; j < 83; j+=3){
            if(row[j] == '#') simpleMap[(i - 1) / 3][(j - 1) / 3] = 1;
            else simpleMap[(i - 1) / 3][(j - 1) / 3] = 0;
        }
    }
}

void Sprite::paintEvent(QPaintEvent* /*evenet*/){}

void Sprite::setStartPos(int y, int x){
    cords.x = x;
    cords.y = y;
    move(cords.x * 30, cords.y * 30);
    previousPosition.x = pos().x();
    previousPosition.y = pos().y();
    start = false;
}

void Sprite::stopSprite(){
    currentDirection = NO_DIR;
    nextDirection = NO_DIR;
    
}

void Sprite::makeStep(){
    switch(currentDirection){
        case(NO_DIR):
            break;
        case(UP):
            if(simpleMap[cords.y - 1][cords.x] == 1){
                stopSprite();
                break; 
            }
            move(cords.x * 30, pos().y() - step);
            break;
        case(LEFT):
            if(simpleMap[cords.y][cords.x - 1] == 1){
                stopSprite();
                break; 
            }
            move(pos().x() - step, cords.y * 30);
            break;
        case(DOWN):
            if(simpleMap[cords.y + 1][cords.x] == 1){
                stopSprite();
                break; 
            }
            move(cords.x * 30, pos().y() + step);
            break;
        case(RIGHT):
            if(simpleMap[cords.y][cords.x + 1] == 1){
                stopSprite();
                break; 
            }
            move(pos().x() + step, cords.y * 30);
            break;
    }
}

void Sprite::changeCords(){
    if(abs(previousPosition.x - pos().x()) == 30){
        cords.x += (previousPosition.x - pos().x() < 0)? 1 : -1;
        previousPosition.x = pos().x(); 
    }
    else if(abs(previousPosition.y - pos().y()) == 30){
        cords.y += (previousPosition.y - pos().y() < 0)? 1 : -1;
        previousPosition.y = pos().y(); 
    }
}

void Sprite::changeDirection(){
    switch(nextDirection){
        case(NO_DIR):
            break;
        case(UP):
            if(simpleMap[cords.y - 1][cords.x] == 0 && currentDirection != UP && previousPosition.x == pos().x()) currentDirection = UP;
            break;
        case(LEFT):
            if(simpleMap[cords.y][cords.x - 1] == 0 && currentDirection != LEFT && previousPosition.y == pos().y()) currentDirection = LEFT;
            break;
        case(DOWN):
            if(simpleMap[cords.y + 1][cords.x] == 0 && currentDirection != DOWN && previousPosition.x == pos().x()) currentDirection = DOWN;
            break;
        case(RIGHT):
            if(simpleMap[cords.y][cords.x + 1] == 0 && currentDirection != RIGHT && previousPosition.y == pos().y()) currentDirection = RIGHT;
            break;
    }
}

void Sprite::moveSprite(){
    if(start) setStartPos(startingPoint.first, startingPoint.second);
    makeStep();
    changeCords();
    //tp case
    changeDirection();
}