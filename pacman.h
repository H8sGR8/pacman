#pragma once

#include "sprite.h"
#include "points.h"

class Pacman : public Sprite{

    Q_OBJECT

    protected:
        virtual void paintEvent(QPaintEvent*);
        virtual void increasePoints(int);
    public slots:
        void moveSprite();
    signals:
        void pointsChanged(int);
        void attackGhosts();
    public:
        int points;
        Point* pointsMap[31][28];
        void keyPressEvent(QKeyEvent*);
        Pacman(pair<int, int>, QWidget* parent = nullptr);
};