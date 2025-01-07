#include "ghosts.h"

#include <QPainter>
#include <cmath>
#include <QDebug>

Ghost::Ghost(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Sprite(startingPoint, parent){
    stage = SCOUT;
    framesVunarable = 0;
    this->player = player;
    active = false;
    waitingTime = 0;
    for(int i = 1; i < 30; i++) for(int j = 1; j < 27; j++) if(
        (simpleMap[i + 1][j] == 0 && simpleMap[i][j + 1] == 0 && simpleMap[i][j - 1] == 0) ||
        (simpleMap[i + 1][j] == 0 && simpleMap[i][j + 1] == 0 && simpleMap[i - 1][j] == 0) ||
        (simpleMap[i + 1][j] == 0 && simpleMap[i][j - 1] == 0 && simpleMap[i - 1][j] == 0) ||
        (simpleMap[i - 1][j] == 0 && simpleMap[i][j + 1] == 0 && simpleMap[i][j - 1] == 0)) 
            simpleMap[i][j] = 2;
}

void Ghost::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0,0,40,40),0,180*16);
    painter.drawRect(0, 20, 40, 20);
    painter.setBrush(QColor("#ffffff"));
    painter.drawPie(QRect(10,10,8,10),0,360*16);
    painter.drawPie(QRect(22,10,8,10),0,360*16);
    painter.setBrush(QColor("#000000"));
    painter.drawPie(QRect(12,12,5,5),0,360*16);
    painter.drawPie(QRect(24,12,5,5),0,360*16);
}

int Ghost::getTileInFront(int direction){
    return simpleMap[cords.y + ((direction == UP)? -1 : ((direction == DOWN)? 1 : 0))]
                [cords.x + ((direction == LEFT)? -1 : ((direction == RIGHT)? 1 : 0))];
}

void Ghost::addPathOption(vector<pair<int, double>> &pathOptions, int pathDirection){
    position option;
    if(getTileInFront(pathDirection) == PATH){
        option.y = cords.y + ((pathDirection == UP)? -1 : ((pathDirection == DOWN)? 1 : 0));
        option.x = cords.x + ((pathDirection == LEFT)? -1 : ((pathDirection == RIGHT)? 1 : 0));
        pathOptions.push_back({pathDirection, sqrt(pow(scoutTarget.x - option.x, 2) + pow(scoutTarget.y - option.y, 2))});
    }
}

void Ghost::getPathOptions(vector<pair<int, double>> &pathOptions){
    switch(currentDirection){
        case(UP):
            addPathOption(pathOptions, LEFT);
            addPathOption(pathOptions, UP);
            addPathOption(pathOptions, RIGHT);
            break;
        case(LEFT):
            addPathOption(pathOptions, LEFT);
            addPathOption(pathOptions, UP);
            addPathOption(pathOptions, DOWN);
            break;
        case(DOWN):
            addPathOption(pathOptions, LEFT);
            addPathOption(pathOptions, DOWN);
            addPathOption(pathOptions, RIGHT);
            break;
        case(RIGHT):
            addPathOption(pathOptions, DOWN);
            addPathOption(pathOptions, UP);
            addPathOption(pathOptions, RIGHT);
            break;
    }
}

void Ghost::comparePaths(vector<pair<int, double>> &pathOptions){
    double minDistance = INFINITY;
        for(unsigned int i = 0; i < pathOptions.size(); i++){
            if(pathOptions.at(i).second < minDistance){
                nextDirection = pathOptions.at(i).first;
                minDistance = pathOptions.at(i).second;
                }
            else if(pathOptions.at(i).second == minDistance && pathOptions.at(i).first < nextDirection) nextDirection = pathOptions.at(i).first;
        }
}

void Ghost::turnWhenNoOption(){
    if(getTileInFront(currentDirection) == WALL && pos().x() == previousPosition.x && pos().y() == previousPosition.y 
            && simpleMap[cords.y][cords.x] == PATH){
        switch(currentDirection){
            case(UP):
                if(simpleMap[cords.y][cords.x - 1] == PATH) nextDirection = LEFT;
                else nextDirection = RIGHT;
                break;
            case(LEFT):
                if(simpleMap[cords.y - 1][cords.x] == PATH) nextDirection = UP;
                else nextDirection = DOWN;
                break;
            case(DOWN):
                if(simpleMap[cords.y][cords.x - 1] == PATH) nextDirection = LEFT;
                else nextDirection = RIGHT;
                break;
            case(RIGHT):
                if(simpleMap[cords.y - 1][cords.x] == PATH) nextDirection = UP;
                else nextDirection = DOWN;
                break;
        }
    }
}

void Ghost::choosePath(){
    if(simpleMap[cords.y][cords.x] == CROSSROAD && pos().x() == previousPosition.x && pos().y() == previousPosition.y){
        vector<pair<int, double>> pathOptions;
        getPathOptions(pathOptions);
        comparePaths(pathOptions);
    }
    else turnWhenNoOption();
}

void Ghost::beFrightened(){
    if(stage == FRIGHTENED) framesVunarable++;
    if(framesVunarable == 5 * FRAMES_PER_SECOND){
        color = orginalColor;
        stage = previousStage;
        framesVunarable = 0;
        update();
    }
}

void Ghost::colideWithPlayer(){
    if(abs(pos().x() - player->pos().x()) <= 10 && abs(pos().y() - player->pos().y()) <= 10){
        if(stage == FRIGHTENED) player->increasePoints(200);
        else player->decreaseHealth();
    }
}

void Ghost::waitToGetFree(int timeToWait){
    if(waitingTime < timeToWait * FRAMES_PER_SECOND) waitingTime++;
    else{
        if(pos().x() != 14 * 30 - 15){
            if(pos().x() < 14 * 30 - 15) move(pos().x() + step, pos().y());
            else move(pos().x() - step, cords.y * 30);
        }
        else if(pos().y() != 11 * 30) move(pos().x(), pos().y() - step);
        else{
            active = true;
            currentDirection = nextDirection;
            nextDirection = NO_DIR;
            setStartPos(11, 14);
        }
    }
}

void Ghost::moveSprite(){
    if(start) setStartPos(cords.y, cords.x);
    if(active){
        makeStep();
        changeCords();
        teleport();
        choosePath();
        changeDirection();
        beFrightened();
        colideWithPlayer();
    }
}

void Ghost::getVunerable(){
    color = QColor("#111155");
    previousStage = stage;
    stage = FRIGHTENED;
    update();
}

void Ghost::startPlaying(){
    play = true;
}

Pink::Pink(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff6688");
    orginalColor = color;
    scoutTarget.y = 0;
    scoutTarget.x = 2;
    nextDirection = LEFT;
}

void Pink::moveSprite(){
    Ghost::moveSprite();
    if(play && !active)waitToGetFree(PINKY_WAITING_TIME);
}

Orange::Orange(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff8800");
    orginalColor = color;
    scoutTarget.y = 30;
    scoutTarget.x = 2;
    nextDirection = LEFT;
}

void Orange::moveSprite(){
    Ghost::moveSprite();
    if(play && !active)waitToGetFree(CLYDE_WAITING_TIME);
}

Cyan::Cyan(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#00ffff");
    orginalColor = color;
    scoutTarget.y = 30;
    scoutTarget.x = 25;
    nextDirection = RIGHT;
}

void Cyan::moveSprite(){
    Ghost::moveSprite();
    if(play && !active)waitToGetFree(INKY_WAITING_TIME);
}

Red::Red(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff0000");
    orginalColor = color;
    scoutTarget.y = 0;
    scoutTarget.x = 25;
    nextDirection = RIGHT;
}

void Red::moveSprite(){
    Ghost::moveSprite();
    if(play && !active)waitToGetFree(BLINKY_WAITING_TIME);
}