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
#define PINKY_WAITING_TIME 5
#define INKY_WAITING_TIME 10
#define CLYDE_WAITING_TIME 15

#define BLINKY_INKY_SCOUT_TARGET_X 25
#define BLINKY_PINKY_SCOUT_TARGEY_Y 0
#define PINKY_CLYDE_SCOUT_TARGET_X 2
#define INKY_CLYDE_SCOUT_TARGEY_Y 30

class Ghost : public Sprite{
    protected:
        QColor orginalColor;
        Pacman *player;
        int stage;
        int previousStage;
        int framesInStage;
        int framesInPreviousStage;
        position target;
        bool active;
        int waitingTime;
        virtual void paintEvent(QPaintEvent*);
        virtual void colideWithPlayer();
        virtual void beFrightened();
        virtual void addPathOption(vector<pair<int, double>>&, int);
        virtual void getPathOptions(vector<pair<int, double>>&);
        virtual void comparePaths(vector<pair<int, double>>&);
        virtual void turnWhenNoOption();
        virtual void choosePath();
        virtual void waitToGetFree(bool);
        virtual void hase(int, int);
        virtual void changeToHase();
        virtual void changeToScout(int, int);
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void startPlaying();
        virtual void getVunerable(); 
        virtual void moveSprite();
    public:
        Ghost(pair<int, int>, Pacman*, QWidget*parent = nullptr);

};

class Pink : public Ghost{
    protected:
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void moveSprite();
    public:
        Pink(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Orange : public Ghost{
    protected:
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void moveSprite();
    public:
        Orange(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Red : public Ghost{
    protected:
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void moveSprite();
    public:
        Red(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Cyan : public Ghost{
    protected:
        Red *blinky;
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void moveSprite();
    public:
        Cyan(pair<int, int>, Pacman*, Red*, QWidget*parent = nullptr);
};