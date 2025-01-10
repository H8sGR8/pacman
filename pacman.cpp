#include "pacman.h"

#include <QPainter>
#include <QKeyEvent>

Pacman::Pacman(pair<int, int> startingPoint, QWidget* parent) : Sprite(startingPoint, parent){
    color = QColor("#ffff00");
    points = 0;
    pointsColected = 0;
    bigPointsColected = 0;
    health = 3;
    for(int i = 0; i < 31; i++) for(int j = 0; j < 28; j++) pointsMap[i][j] = NULL;
}

void Pacman::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0,0,40,40), 0, 360 * 16);
}

void Pacman::keyPressEvent(QKeyEvent *event){
    if(event->key() && !play){
        emit startPlaying();
        play = true;
    }
    if(event->key() == Qt::Key_Up){
        if(currentDirection == NO_DIR) currentDirection = UP;
        if(currentDirection == DOWN) nextDirection = NO_DIR;
        else nextDirection = UP;
    }
    if(event->key() == Qt::Key_Left){
        if(currentDirection == NO_DIR) currentDirection = LEFT;
        if(currentDirection == RIGHT) nextDirection = NO_DIR;
        else nextDirection = LEFT;
    }
    if(event->key() == Qt::Key_Down){
        if(currentDirection == NO_DIR) currentDirection = DOWN;
        if(currentDirection == UP) nextDirection = NO_DIR;
        else nextDirection = DOWN;
    }
    if(event->key() == Qt::Key_Right){
        if(currentDirection == NO_DIR) currentDirection = RIGHT;
        if(currentDirection == LEFT) nextDirection = NO_DIR;
        else nextDirection = RIGHT;
    }
}

void Pacman::increasePoints(int additionalPoints){
    points += additionalPoints;
    emit pointsChanged(points);
}

void Pacman::decreaseHealth(){
    health--;
    emit healthChanged(health);
    emit restartPosition();
    setStartPos(17, 14);
    currentDirection = NO_DIR;
    nextDirection = NO_DIR;
    play = false;
}

void Pacman::increaseBigPointsColected(){
    bigPointsColected++;
    emit attackGhosts();
}

void Pacman::eatPoint(){
    if(pointsMap[cords.y][cords.x] != NULL && !pointsMap[cords.y][cords.x]->colected){
        pointsColected++;
        increasePoints(pointsMap[cords.y][cords.x]->getEaten());
        pointsMap[cords.y][cords.x]->colected = true;
    }
}

void Pacman::goToNextLevel(){
    for(int i = 0; i < 31; i++) for(int j = 0; j < 28; j++) if(pointsMap[i][j] != NULL){
        pointsMap[i][j]->colected = false;
        pointsMap[i][j]->setUpdatesEnabled(true);
    }
    emit restartPosition();
    setStartPos(17, 14);
    currentDirection = NO_DIR;
    nextDirection = NO_DIR;
    play = false;
    pointsColected = 0;
    bigPointsColected = 0;
}

void Pacman::loseGame(){
    if(health == 0) emit endGame();
}

void Pacman::moveSprite(){
    Sprite::moveSprite();
    eatPoint();
    if(pointsColected == 246) goToNextLevel();
    loseGame();
}