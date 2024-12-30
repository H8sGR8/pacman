#pragma once

#include "sprite.h"

class Pacman : public Sprite{

    protected:
        virtual void paintEvent(QPaintEvent*);
    public:
        void keyPressEvent(QKeyEvent*);
        Pacman(QWidget* parent = 0);
};