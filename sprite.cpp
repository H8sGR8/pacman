#include "sprite.h"
#include "defines.h"

#include <QDebug>

Sprite::Sprite(pair<int, int> startingPoint, QWidget* parent) : QWidget(parent){
    cords.y = startingPoint.first;
    cords.x = startingPoint.second;
    currentDirection = NO_DIR;
    nextDirection = NO_DIR;
    start = true;
    step = STEP;
    play = false;
    map.open("map.txt");
    string row;
    for(int i = 0; i < MAP_HEIGHT; i++){
        getline(map, row);
        if((i - 1) % SCALE != 0) continue;
        for (int j = 1; j < MAP_WIDTH - 1; j+=SCALE){
            if(row[j] == '#' || row[j] == '^') simpleMap[(i - 1) / SCALE][(j - 1) / SCALE] = WALL;
            else simpleMap[(i - 1) / SCALE][(j - 1) / SCALE] = PATH;
        }
    }
}

void Sprite::paintEvent(QPaintEvent* /*evenet*/){}

void Sprite::setStartPos(int y, int x){
    cords.y = y;
    cords.x = x; 
    move(cords.x * TILE_SIZE - TILE_SIZE / 2, cords.y * TILE_SIZE);
    previousPosition.x = pos().x() + TILE_SIZE / 2;
    previousPosition.y = pos().y();
    start = false;
}

void Sprite::stopSprite(){
    if(getTileInFront(currentDirection) == 1){ 
        currentDirection = NO_DIR;
        nextDirection = NO_DIR;
        update();
    }
}

void Sprite::makeStep(){
    switch(currentDirection){
        case(NO_DIR):
            break;
        case(UP):
            move(cords.x * TILE_SIZE, pos().y() - step);
            break;
        case(LEFT):
            move(pos().x() - step, cords.y * TILE_SIZE);
            break;
        case(DOWN):
            move(cords.x * TILE_SIZE, pos().y() + step);
            break;
        case(RIGHT):
            move(pos().x() + step, cords.y * TILE_SIZE);
            break;
    }
}

void Sprite::changeCords(){
    if(abs(previousPosition.x - pos().x()) == TILE_SIZE){
        cords.x += (previousPosition.x - pos().x() < 0)? 1 : -1;
        previousPosition.x = pos().x(); 
    }
    else if(abs(previousPosition.y - pos().y()) == TILE_SIZE){
        cords.y += (previousPosition.y - pos().y() < 0)? 1 : -1;
        previousPosition.y = pos().y(); 
    }
}

void Sprite::changeDirection(){
    switch(nextDirection){
        case(NO_DIR):
            break;
        case(UP): case(DOWN):
            if(getTileInFront(nextDirection) == 0 && currentDirection != nextDirection && previousPosition.x == pos().x())
                currentDirection = nextDirection;
            break;
        case(LEFT): case(RIGHT):
            if(getTileInFront(nextDirection) == 0 && currentDirection != nextDirection && previousPosition.y == pos().y())
                currentDirection = nextDirection;
            break;
    }
}

void Sprite::teleport(){
    if(cords.x == 0 && currentDirection == LEFT){
        cords.x = SIMPLE_MAP_WIDHT - 1;
        move(cords.x * TILE_SIZE, cords.y * TILE_SIZE);
        previousPosition.x = pos().x();
        previousPosition.y = pos().y();
    }
    else if(cords.x == SIMPLE_MAP_WIDHT - 1 && currentDirection == RIGHT){
        cords.x = 0;
        move(cords.x * TILE_SIZE, cords.y * TILE_SIZE);
        previousPosition.x = pos().x();
        previousPosition.y = pos().y();
    }
}

int Sprite::getYOfNTileInFront(int direction, int n){
    return cords.y + ((direction == UP)? -n : ((direction == DOWN)? n : 0));
}

int Sprite::getXOfNTileInFront(int direction, int n){
    return cords.x + ((direction == LEFT)? -n : ((direction == RIGHT)? n : 0));
}

int Sprite::getTileInFront(int direction){
    return simpleMap[getYOfNTileInFront(direction, 1)][getXOfNTileInFront(direction, 1)];
}

void Sprite::moveSprite(){
    if(start) setStartPos(cords.y, cords.x);
    stopSprite();
    makeStep();
    changeCords();
    teleport();
    changeDirection();
}