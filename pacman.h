#pragma once

#include "sprite.h"
#include "points.h"

class Pacman : public Sprite{

    Q_OBJECT

    protected:
        int points;
        int health;
        virtual void paintEvent(QPaintEvent*);
        virtual void keyPressEvent(QKeyEvent*);
        virtual void goToNextLevel();
        virtual void eatPoint();
        virtual void loseGame();
    public slots:
        virtual void moveSprite();
        virtual void increaseBigPointsColected();
    signals:
        void endGame();
        void startPlaying();
        void restartPosition();
        void pointsChanged(int);
        void healthChanged(int);
        void attackGhosts();
    public:
        int pointsColected;
        int bigPointsColected;
        Point* pointsMap[31][28];
        virtual void decreaseHealth();
        virtual void increasePoints(int);
        Pacman(pair<int, int>, QWidget* parent = nullptr);
};