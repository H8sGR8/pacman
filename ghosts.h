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

#define BLINKY_WAITING_TIME 0
#define INKY_WAITING_TIME 5
#define PINKY_WAITING_TIME 10
#define CLYDE_WAITING_TIME 15

class Ghost : public Sprite{
    protected:
        QColor orginalColor;
        Pacman *player;
        int stage;
        int previousStage;
        int framesInStage;
        int framesVunarable;
        position scoutTarget;
        bool active;
        int waitingTime;
        virtual void paintEvent(QPaintEvent*);
        virtual void colideWithPlayer();
        virtual void beFrightened();
        virtual int getTileInFront(int);
        virtual void addPathOption(vector<pair<int, double>>&, int);
        virtual void getPathOptions(vector<pair<int, double>>&);
        virtual void comparePaths(vector<pair<int, double>>&);
        virtual void turnWhenNoOption();
        virtual void choosePath();
        virtual void waitToGetFree(int);
    public slots:
        virtual void startPlaying();
        virtual void getVunerable(); 
        virtual void moveSprite();
    public:
        Ghost(pair<int, int>, Pacman*, QWidget*parent = nullptr);

};

class Pink : public Ghost{
    public slots:
        virtual void moveSprite();
    public:
        Pink(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Orange : public Ghost{
    public slots:
        virtual void moveSprite();
    public:
        Orange(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Cyan : public Ghost{
    public slots:
        virtual void moveSprite();
    public:
        Cyan(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Red : public Ghost{
    public slots:
        virtual void moveSprite();
    public:
        Red(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};