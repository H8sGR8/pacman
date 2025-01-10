#include "pacman.h"
#include "defines.h"

#include <QPainter>
#include <QKeyEvent>

Pacman::Pacman(pair<int, int> startingPoint, QWidget* parent) : Sprite(startingPoint, parent){
    color = QColor(YELLOW);
    points = 0;
    pointsColected = 0;
    bigPointsColected = 0;
    health = MAX_HEALTH;
    openingMouth = true;
    mouthWidth = 0;
    for(int i = 0; i < SIMPLE_MAP_HEIGHT; i++) for(int j = 0; j < SIMPLE_MAP_WIDHT; j++) pointsMap[i][j] = NULL;
}

void Pacman::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    switch(currentDirection){
        case(NO_DIR):
            painter.drawPie(QRect(0,0,SPRITE_SIZE,SPRITE_SIZE), 0, FULL_CIRCLE * ANGLE_MULTIPLIER);
            break;
        case(LEFT):
            painter.drawPie(QRect(0,0,SPRITE_SIZE,SPRITE_SIZE),
                            (HALF_CIRCLE + mouthWidth) * ANGLE_MULTIPLIER, (FULL_CIRCLE - mouthWidth * 2) * ANGLE_MULTIPLIER);
            break;
        case(UP):
            painter.drawPie(QRect(0,0,SPRITE_SIZE,SPRITE_SIZE),
                            (QUATER_CIRCLE + mouthWidth) * ANGLE_MULTIPLIER, (FULL_CIRCLE - mouthWidth * 2) * ANGLE_MULTIPLIER);
            break;
        case(DOWN):
            painter.drawPie(QRect(0,0,SPRITE_SIZE,SPRITE_SIZE),
                            (THREE_QUATERS_CIRCLE + mouthWidth) * ANGLE_MULTIPLIER, (FULL_CIRCLE - mouthWidth * 2) * ANGLE_MULTIPLIER);
            break;
        case(RIGHT):
            painter.drawPie(QRect(0,0,SPRITE_SIZE,SPRITE_SIZE),
                            mouthWidth * ANGLE_MULTIPLIER, (FULL_CIRCLE - mouthWidth * 2) * ANGLE_MULTIPLIER);
            break;
    }
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
    setStartPos(PACMAN_START_POS_Y, PACMAN_START_POS_X);
    currentDirection = NO_DIR;
    nextDirection = NO_DIR;
    play = false;
}

void Pacman::increaseBigPointsColected(){
    bigPointsColected++;
    emit attackGhosts();
}

void Pacman::moveMouth(){
    if(currentDirection == NO_DIR) return;
    if(mouthWidth == MOUTH_ANGLE_STEP * 3) openingMouth = false;
    if(mouthWidth == 0) openingMouth = true;
    mouthWidth += (openingMouth)? MOUTH_ANGLE_STEP : -MOUTH_ANGLE_STEP;
}

void Pacman::eatPoint(){
    if(pointsMap[cords.y][cords.x] != NULL && !pointsMap[cords.y][cords.x]->colected){
        pointsColected++;
        increasePoints(pointsMap[cords.y][cords.x]->getEaten());
        pointsMap[cords.y][cords.x]->colected = true;
    }
}

void Pacman::goToNextLevel(){
    if(pointsColected < COLECTABLE_POINTS) return;
    for(int i = 0; i < SIMPLE_MAP_HEIGHT; i++) for(int j = 0; j < SIMPLE_MAP_WIDHT; j++) if(pointsMap[i][j] != NULL){
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
    moveMouth();
    eatPoint();
    goToNextLevel();
    loseGame();
}