#include "map.h"
#include "points.h"
#include "ghosts.h"

#include <QColor>
#include <QPainter>
#include <QGridLayout>
#include <vector>

Map::Block::Block(QWidget *parent) : QWidget(parent){
    setMaximumSize(10, 10);
    setMinimumSize(10, 10);
}

void Map::Block::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawRect(0, 0, 10, 10);
}

Map::Wall::Wall(QWidget *parent) : Block(parent){
    color = QColor("#111155");
}

Map::Path::Path(QWidget *parent) : Block(parent){
    color = QColor("#0d0d1d");
}

Map::Blockade::Blockade(QWidget *parent) : Block(parent){
    color = QColor("#ff0000");
}

Map::Map(QWidget *parent) : QWidget(parent){
    setMinimumSize(850, 940);
    setMaximumSize(850, 940);
    mapFile.open("map.txt");
    string row;
    vector<pair<int, int>> cordsBigPoint;
    player = new Pacman({17, 14});
    Red *blinky = new Red({11, 14}, player);
    connect(player, &Pacman::attackGhosts, blinky, &Ghost::getVunerable);
    Orange *clyde = new Orange({14, 16}, player);
    connect(player, &Pacman::attackGhosts, clyde, &Ghost::getVunerable);
    Cyan *inky = new Cyan({14, 14}, player);
    connect(player, &Pacman::attackGhosts, inky, &Ghost::getVunerable);
    Pink *pinky = new Pink({14, 12}, player);
    connect(player, &Pacman::attackGhosts, pinky, &Ghost::getVunerable);
    QGridLayout *grid = new QGridLayout;
    for(int i = 0; i < 93; i++){
        getline(mapFile, row);
        for(int j = 0; j < 84; j++){
            if(row[j] == '#') grid->addWidget(new Wall, i, j);
            else if(row[j] == '^') grid->addWidget(new Blockade, i, j);
            else {
                grid->addWidget(new Path, i, j);
                if (row[j] == '*'){
                    player->pointsMap[(i - 1) / 3][(j - 1) / 3] = new SmallPoint;
                    grid->addWidget(player->pointsMap[(i - 1) / 3][(j - 1) / 3], i, j);
                    
                }
                else if(row[j] == '0') cordsBigPoint.push_back({i, j});
            }
        }
    }
    for(unsigned int i = 0; i < cordsBigPoint.size(); i++){
        BigPoint *bp = new BigPoint;
        connect(bp, &BigPoint::eaten, player, &Pacman::attackGhosts);
        player->pointsMap[(cordsBigPoint.at(i).first - 1) / 3][(cordsBigPoint.at(i).second - 1) / 3] = bp;
        grid->addWidget(bp, 
        cordsBigPoint.at(i).first - 1, cordsBigPoint.at(i).second - 1, cordsBigPoint.at(i).first + 1, cordsBigPoint.at(i).second + 1);
    }
    grid->addWidget(player, 2, 2, 5, 5);
    grid->addWidget(blinky, 2, 2, 5, 5);
    grid->addWidget(pinky, 2, 2, 5, 5);
    grid->addWidget(inky, 2, 2, 5, 5);
    grid->addWidget(clyde, 2, 2, 5, 5);
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    setLayout(grid);
    gameTimer = new QTimer(this);
    connect(gameTimer, &QTimer::timeout, player, &Pacman::moveSprite);
    connect(gameTimer, &QTimer::timeout, blinky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, pinky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, inky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, clyde, &Ghost::moveSprite);
    gameTimer->start(50);
}