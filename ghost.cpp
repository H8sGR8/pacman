#include "ghost.h"
#include "defines.h"

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
    for(int i = 1; i < SIMPLE_MAP_HEIGHT - 1; i++) for(int j = 1; j < SIMPLE_MAP_WIDHT - 1; j++) if(
        (simpleMap[i + 1][j] == PATH && simpleMap[i][j + 1] == PATH && simpleMap[i][j - 1] == PATH) ||
        (simpleMap[i + 1][j] == PATH && simpleMap[i][j + 1] == PATH && simpleMap[i - 1][j] == PATH) ||
        (simpleMap[i + 1][j] == PATH && simpleMap[i][j - 1] == PATH && simpleMap[i - 1][j] == PATH) ||
        (simpleMap[i - 1][j] == PATH && simpleMap[i][j + 1] == PATH && simpleMap[i][j - 1] == PATH)) 
            simpleMap[i][j] = CROSSROAD;
}

void Ghost::drawEyes(QPainter& painter){
    painter.setBrush(QColor(BLACK));
    switch(currentDirection){
        case(NO_DIR):
            painter.drawPie(QRect(LEFT_PUPIL_POS_X, LEFT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            painter.drawPie(QRect(RIGHT_PUPIL_POS_X, RIGHT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
        case(UP):
            painter.drawPie(QRect(LEFT_PUPIL_POS_X, LEFT_PUPIL_POS_Y - PUPIL_SHIFT, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            painter.drawPie(QRect(RIGHT_PUPIL_POS_X, RIGHT_PUPIL_POS_Y - PUPIL_SHIFT, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
        case(DOWN):
            painter.drawPie(QRect(LEFT_PUPIL_POS_X, LEFT_PUPIL_POS_Y + PUPIL_SHIFT, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            painter.drawPie(QRect(RIGHT_PUPIL_POS_X, RIGHT_PUPIL_POS_Y + PUPIL_SHIFT, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
        case(LEFT):
            painter.drawPie(QRect(LEFT_PUPIL_POS_X - PUPIL_SHIFT, LEFT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            painter.drawPie(QRect(RIGHT_PUPIL_POS_X - PUPIL_SHIFT, RIGHT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
        case(RIGHT):
            painter.drawPie(QRect(LEFT_PUPIL_POS_X + PUPIL_SHIFT, LEFT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            painter.drawPie(QRect(RIGHT_PUPIL_POS_X + PUPIL_SHIFT, RIGHT_PUPIL_POS_Y, PUPIL_SIZE, PUPIL_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
    }
}

void Ghost::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0, 0, SPRITE_SIZE, SPRITE_SIZE), 0, HALF_CIRCLE * ANGLE_MULTIPLIER);
    painter.drawRect(0, SPRITE_SIZE / 2, SPRITE_SIZE, SPRITE_SIZE / 2);
    painter.setBrush(QColor(WHITE));
    painter.drawPie(QRect(LEFT_EYE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
    painter.drawPie(QRect(RIGHT_EYE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
    drawEyes(painter);
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
            case(UP): case(DOWN):
                if(simpleMap[cords.y][cords.x - 1] == PATH) nextDirection = LEFT;
                else nextDirection = RIGHT;
                break;
            case(LEFT): case(RIGHT):
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
    if(stage == FRIGHTENED && framesInStage == FRIGHTENED_TIME * FRAMES_PER_SECOND){
        color = orginalColor;
        stage = previousStage;
        framesInStage = framesInPreviousStage;
        update();
    }
}

void Ghost::colideWithPlayerWhileFrightened(){
    player->increasePoints(GHOST_VALUE);
    play = false;
    target.x = OUTSIDE_CAGE_POS_X;
    target.y = OUTSIDE_CAGE_POS_Y;
    if(abs(pos().y() - previousPosition.y) % (TILE_SIZE / 2) != 0 && (currentDirection == DOWN || currentDirection == UP))
        move(cords.x * TILE_SIZE, previousPosition.y - (TILE_SIZE / 2));
    if(abs(pos().x() - previousPosition.x) % (TILE_SIZE / 2) != 0 && (currentDirection == LEFT || currentDirection == RIGHT))
        move(previousPosition.x - (TILE_SIZE / 2), cords.y * TILE_SIZE);
    step *= 3;
    currentDirection = NO_DIR;
    color = orginalColor;
    vector<pair<int, double>> pathOptions;
    getPathOptions(pathOptions);
    comparePaths(pathOptions);
    currentDirection = nextDirection;
}

void Ghost::colideWithPlayer(){
    if(play && abs(pos().x() - player->pos().x()) <= COLISION_ERROR && abs(pos().y() - player->pos().y()) <= COLISION_ERROR){
        if(stage == FRIGHTENED) colideWithPlayerWhileFrightened();
        else player->decreaseHealth();
    }
}

void Ghost::waitToGetFree(bool waitWhile){
    if(!play || active) return;
    waitingTime++;
    if(waitWhile) return;
    if(pos().x() != CENTER_CAGE_POS_X * TILE_SIZE - (TILE_SIZE / 2)){
        if(pos().x() < CENTER_CAGE_POS_X * TILE_SIZE - (TILE_SIZE / 2)) move(pos().x() + step, pos().y());
        else move(pos().x() - step, cords.y * TILE_SIZE);
    }
    else if(pos().y() != OUTSIDE_CAGE_POS_Y * TILE_SIZE) move(pos().x(), pos().y() - step);
    else{
        active = true;
        setStartPos(OUTSIDE_CAGE_POS_Y, OUTSIDE_CAGE_POS_X);
        vector<pair<int, double>> pathOptions;
        currentDirection = NO_DIR;
        getPathOptions(pathOptions);
        comparePaths(pathOptions);
        currentDirection = nextDirection;
    }
}

int Ghost::calculateHaseTargetX(){return 0;}

int Ghost::calculateHaseTargetY(){return 0;}

void Ghost::scout(){}

void Ghost::hase(){
    if(stage == HASE){
        target.x = calculateHaseTargetX();
        target.y = calculateHaseTargetY();
    }
}

void Ghost::changeToScout(){
    if(stage == HASE && framesInStage == HASEING_TIME * FRAMES_PER_SECOND){
        stage = SCOUT;
        framesInStage = 0;
        scout();
    }
}

void Ghost::changeToHase(){
    if(stage == SCOUT && framesInStage == SCOUTING_TIME * FRAMES_PER_SECOND){
        stage = HASE;
        framesInStage = 0;
    }
}

void Ghost::goToJail(){
    if(pos().y() == OUTSIDE_CAGE_POS_Y * TILE_SIZE && pos().x() == OUTSIDE_CAGE_POS_X * TILE_SIZE - (TILE_SIZE / 2)){
        currentDirection = NO_DIR;
        nextDirection = NO_DIR;
        posToGoToJail = true;
    }
    if(!posToGoToJail) return;
    if(pos().y() < CENTER_CAGE_POS_Y * TILE_SIZE) move(pos().x(), pos().y() + step);
    else{
        play = true;
        active = false;
        posToGoToJail = false;
        step = STEP;
        scout();
    }
}

void Ghost::moveSprite(){
    if(start) setStartPos(cords.y, cords.x);
    if(!active) return;
    framesInStage++;
    makeStep();
    changeCords();
    teleport();
    choosePath();
    changeDirection();
    beFrightened();
    colideWithPlayer();
    if(play){
        changeToScout();
        changeToHase();
        hase();
    }
    else goToJail();
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
    if(!active) return;
    color = QColor(BLUE);
    previousStage = stage;
    framesInPreviousStage = framesInStage;
    stage = FRIGHTENED;
    framesInStage = 0;
    update();
}

void Ghost::startPlaying(){
    play = true;
}
