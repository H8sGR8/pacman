#include "pacman.h"

#include <QPainter>
#include <QKeyEvent>
#include <QDebug>

Pacman::Pacman(pair<int, int> startingPoint, QWidget* parent) : Sprite(startingPoint, parent){
    color = QColor("#ffff00");
    step = STEP;
    points = 0;
    for(int i = 0; i < 31; i++) for(int j = 0; j < 28; j++) pointsMap[i][j] = NULL;
}

void Pacman::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawPie(QRect(0,0,40,40),0,360*16);
}

void Pacman::keyPressEvent(QKeyEvent *event){
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

void Pacman::moveSprite(){
    Sprite::moveSprite();
    if(pointsMap[cords.y][cords.x] != NULL){
        increasePoints(pointsMap[cords.y][cords.x]->getEaten());
        pointsMap[cords.y][cords.x] = NULL;
    }
}