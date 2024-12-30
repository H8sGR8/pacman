#include "sprite.h"
#include <QDebug>

Sprite::Sprite(QWidget* parent) : QWidget(parent){
    
}

void Sprite::paintEvent(QPaintEvent* /*evenet*/){}

void Sprite::setStartPos(int y, int x){
    cords.x = x;
    cords.y = y;
    move(cords.x * 30 - 4, cords.y * 30 - 4);
    previousPosition.x = pos().x();
    previousPosition.y = pos().y();
}

void Sprite::moveSprite(){
    if(start){
        setStartPos(1, 1);
        start = false;
    }
    switch(currentDirection){
        case(NO_DIR):
            break;
        case(UP):
            if(simpleMap[cords.y - 1][cords.x] == 1){
                currentDirection = NO_DIR;
                nextDirection = NO_DIR;
                break;
            }
            move(cords.x * 30 - 4, pos().y() - 1);
            break;
        case(LEFT):
            if(simpleMap[cords.y][cords.x - 1] == 1){
                currentDirection = NO_DIR;
                nextDirection = NO_DIR;
                break;
            }
            move(pos().x() - 1, cords.y * 30 - 4);
            break;
        case(DOWN):
            if(simpleMap[cords.y + 1][cords.x] == 1){
                currentDirection = NO_DIR;
                nextDirection = NO_DIR;
                break;
            }
            move(cords.x * 30 - 4, pos().y() + 1);
            break;
        case(RIGHT):
            if(simpleMap[cords.y][cords.x + 1] == 1){
                currentDirection = NO_DIR;
                nextDirection = NO_DIR;
                break;
            }
            move(pos().x() + 1, cords.y * 30 - 4);
            break;
    }
    if(abs(previousPosition.x - pos().x()) == 30){
        cords.x += (previousPosition.x - pos().x() < 0)? 1 : -1;
        previousPosition.x = pos().x(); 
    }
    else if(abs(previousPosition.y - pos().y()) == 30){
        cords.y += (previousPosition.y - pos().y() < 0)? 1 : -1;
        previousPosition.y = pos().y(); 
    }
    //tp case
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
    update(rect());
}