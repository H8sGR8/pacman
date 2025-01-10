#include "ghost.h"

#include <QPainter>
#include <cmath>
#include <QDebug>

Ghost::Ghost(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Sprite(startingPoint, parent){
    stage = SCOUT;
    framesInStage = 0;
    this->player = player;
    active = false;
    waitingTime = 0;
    posToGoToJail = false;
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
    if(getTileInFront(pathDirection) == PATH || getTileInFront(pathDirection) == CROSSROAD){
        option.y = getYOfNTileInFront(pathDirection, 1);
        option.x = getXOfNTileInFront(pathDirection, 1);
        pathOptions.push_back({pathDirection, sqrt(pow(target.x - option.x, 2) + pow(target.y - option.y, 2))});
    }
}

void Ghost::getPathOptions(vector<pair<int, double>> &pathOptions){
    switch(currentDirection){
        case(NO_DIR):
            addPathOption(pathOptions, LEFT);
            addPathOption(pathOptions, UP);
            addPathOption(pathOptions, RIGHT);
            addPathOption(pathOptions, DOWN);
            break;
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
    nextDirection = RIGHT;
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
    if(play && abs(pos().x() - player->pos().x()) <= 10 && abs(pos().y() - player->pos().y()) <= 10){
        if(stage == FRIGHTENED){
            player->increasePoints(200);
            play = false;
            target.x = 14;
            target.y = 11;
            if(abs(pos().y() - previousPosition.y) % 15 != 0 && (currentDirection == DOWN || currentDirection == UP))
                move(cords.x * 30, previousPosition.y - 15);
            if(abs(pos().x() - previousPosition.x) % 15 != 0 && (currentDirection == LEFT || currentDirection == RIGHT))
                move(previousPosition.x - 15, cords.y * 30);
            step *= 3;
            currentDirection = NO_DIR;
            color = orginalColor;
            vector<pair<int, double>> pathOptions;
            getPathOptions(pathOptions);
            comparePaths(pathOptions);
            currentDirection = nextDirection;
        }
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
            setStartPos(11, 14);
            vector<pair<int, double>> pathOptions;
            currentDirection = NO_DIR;
            getPathOptions(pathOptions);
            comparePaths(pathOptions);
            currentDirection = nextDirection;
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

void Ghost::scout(){}

void Ghost::hase(int targetY, int targetX){
    if(stage == HASE){
        target.x = targetX;
        target.y = targetY;
    }
}

void Ghost::changeToScout(){
    if(stage == HASE && framesInStage == 20 * FRAMES_PER_SECOND){
        stage = SCOUT;
        framesInStage = 0;
        scout();
    }
}

void Ghost::goToJail(){
    if(pos().y() == 11 * 30 && pos().x() == 14 * 30 - 15){
        currentDirection = NO_DIR;
        nextDirection = NO_DIR;
        posToGoToJail = true;
    }
    if(posToGoToJail){
        if(pos().y() < 14 * 30) move(pos().x(), pos().y() + step);
        else{
            play = true;
            active = false;
            posToGoToJail = false;
            step = STEP;
            scout();
        }
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
        if(play){
            changeToHase();
            hase(calculateHaseTargetY(), calculateHaseTargetX());
        }
        else goToJail();
    }
}

void Ghost::restartPosition(){
    color = orginalColor;
    play = false;
    active = false;
    currentDirection = NO_DIR;
    stage = SCOUT;
    step = STEP;
    framesInStage = 0;
    waitingTime = 0;
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
