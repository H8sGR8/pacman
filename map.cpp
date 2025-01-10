#include "map.h"
#include "points.h"
#include "defines.h"

#include <QPainter>

Map::Block::Block(QWidget *parent) : QWidget(parent){
    setMaximumSize(BLOCK_SIZE, BLOCK_SIZE);
    setMinimumSize(BLOCK_SIZE, BLOCK_SIZE);
}

void Map::Block::paintEvent(QPaintEvent* /*event*/){
    QPainter painter(this);
    painter.setPen(Qt::NoPen);
    painter.setBrush(QColor(color));
    painter.drawRect(0, 0, BLOCK_SIZE, BLOCK_SIZE);
}

Map::Wall::Wall(QWidget *parent) : Block(parent){
    color = QColor(BLUE);
}

Map::Path::Path(QWidget *parent) : Block(parent){
    color = QColor(DARK_BLUE);
}

Map::Blockade::Blockade(QWidget *parent) : Block(parent){
    color = QColor(RED);
}

void Map::endGame(){
    setVisible(false);
}

void Map::connectObjects(){
    connect(player, &Pacman::endGame, this, &Map::endGame);
    connect(player, &Pacman::attackGhosts, blinky, &Red::getVunerable);
    connect(player, &Pacman::restartPosition, blinky, &Red::restartPosition);
    connect(player, &Pacman::startPlaying, blinky, &Ghost::startPlaying);
    connect(player, &Pacman::attackGhosts, clyde, &Orange::getVunerable);
    connect(player, &Pacman::restartPosition, clyde, &Orange::restartPosition);
    connect(player, &Pacman::startPlaying, clyde, &Ghost::startPlaying);
    connect(player, &Pacman::attackGhosts, inky, &Cyan::getVunerable);
    connect(player, &Pacman::restartPosition, inky, &Cyan::restartPosition);
    connect(player, &Pacman::startPlaying, inky, &Ghost::startPlaying);
    connect(player, &Pacman::attackGhosts, pinky, &Pink::getVunerable);
    connect(player, &Pacman::restartPosition, pinky, &Pink::restartPosition);
    connect(player, &Pacman::startPlaying, pinky, &Ghost::startPlaying);
    connect(gameTimer, &QTimer::timeout, player, &Pacman::moveSprite);
    connect(gameTimer, &QTimer::timeout, blinky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, pinky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, inky, &Ghost::moveSprite);
    connect(gameTimer, &QTimer::timeout, clyde, &Ghost::moveSprite);
}

void Map::createMap(vector<pair<int, int>> &cordsBigPoint){
    string row;
    for(int i = 0; i < MAP_HEIGHT; i++){
        getline(mapFile, row);
        for(int j = 0; j < MAP_WIDTH; j++){
            if(row[j] == '#') grid->addWidget(new Wall, i, j);
            else if(row[j] == '^') grid->addWidget(new Blockade, i, j);
            else {
                grid->addWidget(new Path, i, j);
                if (row[j] == '*'){
                    player->pointsMap[(i - 1) / SCALE][(j - 1) / SCALE] = new SmallPoint;
                    grid->addWidget(player->pointsMap[(i - 1) / SCALE][(j - 1) / SCALE], i, j);
                    
                }
                else if(row[j] == '0') cordsBigPoint.push_back({i, j});
            }
        }
    }
}

void Map::addBigPoints(vector<pair<int, int>> &cordsBigPoint){
    for(unsigned int i = 0; i < cordsBigPoint.size(); i++){
        BigPoint *bp = new BigPoint;
        connect(bp, &BigPoint::eaten, player, &Pacman::increaseBigPointsColected);
        player->pointsMap[(cordsBigPoint.at(i).first - 1) / SCALE][(cordsBigPoint.at(i).second - 1) / SCALE] = bp;
        grid->addWidget(bp, 
        cordsBigPoint.at(i).first - 1, cordsBigPoint.at(i).second - 1, cordsBigPoint.at(i).first + 1, cordsBigPoint.at(i).second + 1);
    }
}

void Map::addSprites(){
    grid->addWidget(player, 0, 0, (SPRITE_SIZE / BLOCK_SIZE), (SPRITE_SIZE / BLOCK_SIZE));
    grid->addWidget(blinky, 0, 0, (SPRITE_SIZE / BLOCK_SIZE), (SPRITE_SIZE / BLOCK_SIZE));
    grid->addWidget(pinky, 0, 0, (SPRITE_SIZE / BLOCK_SIZE), (SPRITE_SIZE / BLOCK_SIZE));
    grid->addWidget(inky, 0, 0, (SPRITE_SIZE / BLOCK_SIZE), (SPRITE_SIZE / BLOCK_SIZE));
    grid->addWidget(clyde, 0, 0, (SPRITE_SIZE / BLOCK_SIZE), (SPRITE_SIZE / BLOCK_SIZE));
}

void Map::setupMap(){
    vector<pair<int, int>> cordsBigPoint;
    connectObjects();
    createMap(cordsBigPoint);
    addBigPoints(cordsBigPoint);
    addSprites();
    grid->setHorizontalSpacing(0);
    grid->setVerticalSpacing(0);
    setLayout(grid);
    setMinimumSize(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS);
    setMaximumSize(MAP_WIDTH_PIXELS, MAP_HEIGHT_PIXELS);
}


Map::Map(QWidget *parent) : QWidget(parent){
    gameTimer = new QTimer(this);
    player = new Pacman({PACMAN_START_POS_Y, PACMAN_START_POS_X});
    blinky = new Red({OUTSIDE_CAGE_POS_Y, OUTSIDE_CAGE_POS_X}, player);
    clyde = new Orange({RIGHT_CAGE_POS_Y, RIGHT_CAGE_POS_X}, player);
    inky = new Cyan({LEFT_CAGE_POS_Y, LEFT_CAGE_POS_X}, player, blinky);
    pinky = new Pink({CENTER_CAGE_POS_Y, CENTER_CAGE_POS_X}, player);
    grid = new QGridLayout;
    mapFile.open("map.txt");
    setupMap();
    gameTimer->start(REFRESH_FREQUENCY);
}