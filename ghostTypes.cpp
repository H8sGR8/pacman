#include "ghostTypes.h"
#include "defines.h"

#include <cmath>

Pink::Pink(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor(PINK);
    orginalColor = color;
    scout();
}

void Pink::scout(){
    target.x = PINKY_CLYDE_SCOUT_TARGET_X;
    target.y = BLINKY_PINKY_SCOUT_TARGEY_Y;
}

int Pink::calculateHaseTargetX(){
    return player->getXOfNTileInFront(player->currentDirection, TILES_IN_FRON_OF_PACMAN);
}

int Pink::calculateHaseTargetY(){
    return player->getYOfNTileInFront(player->currentDirection, TILES_IN_FRON_OF_PACMAN);
}

void Pink::moveSprite(){
    Ghost::moveSprite();
    waitToGetFree(waitingTime < PINKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Pink::restartPosition(){
    Ghost::restartPosition();
    setStartPos(RIGHT_CAGE_POS_Y, RIGHT_CAGE_POS_Y);
    scout();
}

Orange::Orange(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor(ORANGE);
    orginalColor = color;
    scout();
}

void Orange::scout(){
    target.x = PINKY_CLYDE_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

int Orange::calculateHaseTargetX(){
    return (sqrt(pow(player->cords.x, 2) + pow(cords.x, 2)) < DISTANCE_TO_HASE)? PINKY_CLYDE_SCOUT_TARGET_X : player->cords.x;
}

int Orange::calculateHaseTargetY(){
    return (sqrt(pow(player->cords.y, 2) + pow(cords.y, 2)) < DISTANCE_TO_HASE)? INKY_CLYDE_SCOUT_TARGEY_Y : player->cords.y;
}

void Orange::moveSprite(){
    Ghost::moveSprite();
    waitToGetFree(player->bigPointsColected < BIG_POINTS_COLECTED_TO_FREE || waitingTime < CLYDE_WAITING_TIME * FRAMES_PER_SECOND);
}

void Orange::restartPosition(){
    Ghost::restartPosition();
    setStartPos(RIGHT_CAGE_POS_Y, RIGHT_CAGE_POS_X);
    scout();
}

Cyan::Cyan(pair<int, int> startingPoint, Pacman *player, Red *blinky, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor(CYAN);
    orginalColor = color;
    this->blinky = blinky;
    scout();
}

void Cyan::scout(){
    target.x = BLINKY_INKY_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

int Cyan::calculateHaseTargetX(){
    return 2 * player->getXOfNTileInFront(player->currentDirection, TILES_IN_FRON_OF_PACMAN / 2) - blinky->cords.x;
}

int Cyan::calculateHaseTargetY(){
    return 2 * player->getYOfNTileInFront(player->currentDirection, TILES_IN_FRON_OF_PACMAN / 2) - blinky->cords.y;
}

void Cyan::moveSprite(){
    Ghost::moveSprite();
    waitToGetFree(player->pointsColected < POINTS_COLECTED_TO_FREE || waitingTime < INKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Cyan::restartPosition(){
    Ghost::restartPosition();
    setStartPos(LEFT_CAGE_POS_Y, LEFT_CAGE_POS_X);
    target.x = BLINKY_INKY_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

Red::Red(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor(RED);
    orginalColor = color;
    scout();
}

void Red::scout(){
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
    waitToGetFree(waitingTime < BLINKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Red::restartPosition(){
    Ghost::restartPosition();
    setStartPos(OUTSIDE_CAGE_POS_Y, OUTSIDE_CAGE_POS_X);
    scout();
}