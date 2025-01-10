#pragma once

#include "sprite.h"
#include "pacman.h"

#include <vector>

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
        bool posToGoToJail;
        virtual void drawEyes(QPainter&);
        virtual void paintEvent(QPaintEvent*);
        virtual void colideWithPlayerWhileFrightened();
        virtual void colideWithPlayer();
        virtual void beFrightened();
        virtual void addPathOption(vector<pair<int, double>>&, int);
        virtual void getPathOptions(vector<pair<int, double>>&);
        virtual void comparePaths(vector<pair<int, double>>&);
        virtual void turnWhenNoOption();
        virtual void choosePath();
        virtual void waitToGetFree(bool);
        virtual void scout();
        virtual void hase();
        virtual void changeToHase();
        virtual void changeToScout();
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
        virtual void goToJail();
    public slots:
        virtual void startPlaying();
        virtual void restartPosition();
        virtual void getVunerable(); 
        virtual void moveSprite();
    public:
        Ghost(pair<int, int>, Pacman*, QWidget*parent = nullptr);

};
