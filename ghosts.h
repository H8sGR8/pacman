#pragma once

#include "sprite.h"
#include "pacman.h"

#include <vector>

#define SCOUT 1
#define HASE 2
#define FRIGHTENED 3

#define FRAMES_PER_SECOND 20

#define PATH 0
#define WALL 1 
#define CROSSROAD 2

class Ghost : public Sprite{
    protected:
        QColor orginalColor;
        Pacman *player;
        int stage;
        int previousStage;
        int framesInStage;
        int framesVunarable;
        position target;
        bool active;
        virtual void paintEvent(QPaintEvent*);
        virtual void colideWithPlayer();
        virtual void beFrightened();
        virtual int getTileInFront(int);
        virtual void addPathOption(vector<pair<int, double>>&, int);
        virtual void choosePath();
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