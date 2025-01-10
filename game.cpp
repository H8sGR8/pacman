#include "map.h"
#include "pacman.h"
#include "defines.h"

#include <QApplication>
#include <QGridLayout>
#include <QPushButton>
#include <QDesktopWidget>
#include <QPalette>
#include <QLabel>
#include <string>

class Game : public QWidget{

	private:
		QPushButton *quit;
		QLabel *points;
		QLabel *health;
		QLabel *gameOver;
		virtual void updatePoints(int);
		virtual void updateHealth(int);
		virtual void connectObjects();
		virtual void setupGame();
	public:
		Map *map;
		Game(QWidget *parent = nullptr);
};

void Game::updatePoints(int newPoints){
	points->setText(QString::fromStdString("Points: " + to_string(newPoints)));
}

void Game::updateHealth(int newHealth){
	health->setText(QString::fromStdString("Health: " + to_string(newHealth)));
	if(newHealth == 0) gameOver->setVisible(true);
}

void Game::connectObjects(){
	connect(quit, &QPushButton::clicked, qApp, &QApplication::quit);
	connect(map->player, &Pacman::pointsChanged, this, &Game::updatePoints);
	connect(map->player, &Pacman::healthChanged, this, &Game::updateHealth);
}

void Game::setupGame(){
	QGridLayout	*grid = new QGridLayout;
	grid->addWidget(map, MAP_POS);
	grid->addWidget(points, POINTS_POS);
	grid->addWidget(health, HEALTH_POS);
	grid->addWidget(quit, QUIT_POS);
	grid->addWidget(gameOver, GAME_OVER_POS);
	setLayout(grid);
}

Game::Game(QWidget *parent) : QWidget(parent){
	setPalette(QPalette(QColor(GRAY)));
	resize(1200, 1000);
	move(QApplication::desktop()->geometry().center() - rect().center());
	map = new Map();
	quit = new QPushButton("QUIT");
	points = new QLabel;
	health = new QLabel;
	gameOver = new QLabel;
	points->setText("Points: 0");
	health->setText("Health: 3");
	gameOver->setText("Game Over :(");
	gameOver->setVisible(false);
	gameOver->setStyleSheet("font: 40pt; color: red;");
	connectObjects();
	setupGame();
}

int main(int argc, char *argv[])
{
	QApplication app(argc, argv);
	Game game;
	game.map->player->setFocus();
	game.show();
	return app.exec();
}
