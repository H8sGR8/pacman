#include "ghosts.h"

#include <QPainter>
#include <cmath>

Ghost::Ghost(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Sprite(startingPoint, parent){
    stage = SCOUT;
    framesInStage = 0;
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

void Ghost::addPathOption(vector<pair<int, double>> &pathOptions, int pathDirection){
    position option;
    if(getTileInFront(pathDirection) == PATH){
        option.y = getYOfNTileInFront(pathDirection, 1);
        option.x = getXOfNTileInFront(pathDirection, 1);
        pathOptions.push_back({pathDirection, sqrt(pow(target.x - option.x, 2) + pow(target.y - option.y, 2))});
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
    if(stage == FRIGHTENED && framesInStage == 5 * FRAMES_PER_SECOND){
        color = orginalColor;
        stage = previousStage;
        framesInStage = framesInPreviousStage;
        update();
    }
}

void Ghost::colideWithPlayer(){
    if(abs(pos().x() - player->pos().x()) <= 10 && abs(pos().y() - player->pos().y()) <= 10){
        if(stage == FRIGHTENED) player->increasePoints(200);
        else player->decreaseHealth();
    }
}

void Ghost::waitToGetFree(bool waitWhile){
    waitingTime++;
    if(waitWhile);
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

void Ghost::changeToHase(){
    if(stage == SCOUT && framesInStage == 7 * FRAMES_PER_SECOND){
        stage = HASE;
        framesInStage = 0;
    }
}

int Ghost::calculateHaseTargetX(){return 0;}

int Ghost::calculateHaseTargetY(){return 0;}

void Ghost::hase(int targetY, int targetX){
    if(stage == HASE){
        target.x = targetX;
        target.y = targetY;
    }
}

void Ghost::changeToScout(int targetY, int targetX){
    if(stage == HASE && framesInStage == 20 * FRAMES_PER_SECOND){
        stage = SCOUT;
        framesInStage = 0;
        target.x = targetX;
        target.y = targetY;
    }
}

void Ghost::moveSprite(){
    if(start) setStartPos(cords.y, cords.x);
    if(active){
        framesInStage++;
        makeStep();
        changeCords();
        teleport();
        choosePath();
        changeDirection();
        beFrightened();
        colideWithPlayer();
    }
    changeToHase();
    hase(calculateHaseTargetY(), calculateHaseTargetX());
}

void Ghost::getVunerable(){
    if(active){
        color = QColor("#111155");
        previousStage = stage;
        framesInPreviousStage = framesInStage;
        stage = FRIGHTENED;
        framesInStage = 0;
        update();
    }
}

void Ghost::startPlaying(){
    play = true;
}

Pink::Pink(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff6688");
    orginalColor = color;
    nextDirection = LEFT;
    target.x = PINKY_CLYDE_SCOUT_TARGET_X;
    target.y = BLINKY_PINKY_SCOUT_TARGEY_Y;
}

int Pink::calculateHaseTargetX(){
    return player->getXOfNTileInFront(player->currentDirection, 4);
}

int Pink::calculateHaseTargetY(){
    return player->getYOfNTileInFront(player->currentDirection, 4);
}

void Pink::moveSprite(){
    Ghost::moveSprite();
    changeToScout(BLINKY_PINKY_SCOUT_TARGEY_Y, PINKY_CLYDE_SCOUT_TARGET_X);
    if(play && !active)waitToGetFree(waitingTime < PINKY_WAITING_TIME * FRAMES_PER_SECOND);
}

Orange::Orange(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff8800");
    orginalColor = color;
    nextDirection = LEFT;
    target.x = PINKY_CLYDE_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

int Orange::calculateHaseTargetX(){
    return (sqrt(pow(player->cords.x, 2) + pow(cords.x, 2)) < 8)? PINKY_CLYDE_SCOUT_TARGET_X : player->cords.x;
}

int Orange::calculateHaseTargetY(){
    return (sqrt(pow(player->cords.y, 2) + pow(cords.y, 2)) < 8)? INKY_CLYDE_SCOUT_TARGEY_Y : player->cords.y;
}

void Orange::moveSprite(){
    Ghost::moveSprite();
    changeToScout(INKY_CLYDE_SCOUT_TARGEY_Y, PINKY_CLYDE_SCOUT_TARGET_X);
    if(play && !active)waitToGetFree(player->bigPointsColected < 3);
}

Cyan::Cyan(pair<int, int> startingPoint, Pacman *player, Red *blinky, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#00ffff");
    orginalColor = color;
    this->blinky = blinky;
    nextDirection = RIGHT;
    target.x = BLINKY_INKY_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

int Cyan::calculateHaseTargetX(){
    return 2 * player->getXOfNTileInFront(player->currentDirection, 2) - blinky->cords.x;
}

int Cyan::calculateHaseTargetY(){
    return 2 * player->getYOfNTileInFront(player->currentDirection, 2) - blinky->cords.y;
}

void Cyan::moveSprite(){
    Ghost::moveSprite();
    changeToScout(INKY_CLYDE_SCOUT_TARGEY_Y, BLINKY_INKY_SCOUT_TARGET_X);
    if(play && !active)waitToGetFree(player->pointsColected < 30);
}

Red::Red(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff0000");
    orginalColor = color;
    nextDirection = RIGHT;
    target.x = BLINKY_INKY_SCOUT_TARGET_X;
    target.y = BLINKY_PINKY_SCOUT_TARGEY_Y;
}

int Red::calculateHaseTargetX(){
    return player->cords.x;
}

int Red::calculateHaseTargetY(){
    return player->cords.y;
}

void Red::moveSprite(){
    Ghost::moveSprite();
    changeToScout(BLINKY_PINKY_SCOUT_TARGEY_Y, BLINKY_INKY_SCOUT_TARGET_X);
    if(play && !active)waitToGetFree(waitingTime < BLINKY_WAITING_TIME * FRAMES_PER_SECOND);
}