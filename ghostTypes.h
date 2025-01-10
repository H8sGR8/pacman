#pragma once

#include "ghost.h"

class Pink : public Ghost{
    protected:
        virtual void scout();
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void restartPosition();
        virtual void moveSprite();
    public:
        Pink(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Orange : public Ghost{
    protected:
        virtual void scout();
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void restartPosition();
        virtual void moveSprite();
    public:
        Orange(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Red : public Ghost{
    protected:
        virtual void scout();
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void restartPosition();
        virtual void moveSprite();
    public:
        Red(pair<int, int>, Pacman*, QWidget*parent = nullptr);
};

class Cyan : public Ghost{
    protected:
        Red *blinky;
        virtual void scout();
        virtual int calculateHaseTargetX();
        virtual int calculateHaseTargetY();
    public slots:
        virtual void restartPosition();
        virtual void moveSprite();
    public:
        Cyan(pair<int, int>, Pacman*, Red*, QWidget*parent = nullptr);
};