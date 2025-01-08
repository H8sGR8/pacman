#pragma once

#include "sprite.h"
#include "points.h"

class Pacman : public Sprite{

    Q_OBJECT

    protected:
        int points;
        int health;
        virtual void paintEvent(QPaintEvent*);
        void keyPressEvent(QKeyEvent*);
    public slots:
        void moveSprite();
        void increaseBigPointsColected();
    signals:
        void startPlaying();
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