#pragma once

#include "sprite.h"
#include "pacman.h"

class Ghost : public Sprite{
    protected:
        QColor orginalColor;
        Pacman *player;
        bool vunerable;
        int framesVunarable;
        virtual void paintEvent(QPaintEvent*);
    public slots:
        void getVunerable(); 
        void moveSprite();
    public:
        Ghost(pair<int, int>, Pacman*, QWidget*parent = nullptr);

};

class Pink : public Ghost{
    public:
        Pink(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Orange : public Ghost{
    public:
        Orange(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Cyan : public Ghost{
    public:
        Cyan(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Red : public Ghost{
    public:
        Red(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};