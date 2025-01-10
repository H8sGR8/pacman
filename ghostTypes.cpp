#include <ghostTypes.h>

#include <cmath>

Pink::Pink(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff6688");
    orginalColor = color;
    scout();
}

void Pink::scout(){
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
    if(play && active)changeToScout();
    if(play && !active)waitToGetFree(waitingTime < PINKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Pink::restartPosition(){
    Ghost::restartPosition();
    setStartPos(14, 14);
    scout();
}

Orange::Orange(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff8800");
    orginalColor = color;
    scout();
}

void Orange::scout(){
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
    if(play && active)changeToScout();
    if(play && !active)waitToGetFree(player->bigPointsColected < 3 || waitingTime < CLYDE_WAITING_TIME * FRAMES_PER_SECOND);
}

void Orange::restartPosition(){
    Ghost::restartPosition();
    setStartPos(14, 16);
    scout();
}

Cyan::Cyan(pair<int, int> startingPoint, Pacman *player, Red *blinky, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#00ffff");
    orginalColor = color;
    this->blinky = blinky;
    scout();
}

void Cyan::scout(){
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
    if(play && active)changeToScout();
    if(play && !active)waitToGetFree(player->pointsColected < 30 || waitingTime < INKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Cyan::restartPosition(){
    Ghost::restartPosition();
    setStartPos(14, 12);
    target.x = BLINKY_INKY_SCOUT_TARGET_X;
    target.y = INKY_CLYDE_SCOUT_TARGEY_Y;
}

Red::Red(pair<int, int> startingPoint, Pacman *player, QWidget* parent) : Ghost(startingPoint, player, parent){
    color = QColor("#ff0000");
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
    if(play && active)changeToScout();
    if(play && !active)waitToGetFree(waitingTime < BLINKY_WAITING_TIME * FRAMES_PER_SECOND);
}

void Red::restartPosition(){
    Ghost::restartPosition();
    setStartPos(11, 14);
    scout();
}