#ifndef GAME_H
#define GAME_H

#include <Debug.h>

#include <iostream>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

#include <Matrix3.h>

using namespace std;
using namespace sf;
using namespace customMatrix;
//using namespace gpp;

class Game
{
public:
	Game();
	~Game();
	void run();
	int numOfVertex = 8;
	int numOfTriangles = 36;
private:
	Window window;
	bool isRunning = false;
	void initialize();
	void update();
	void render();
	void unload();
	Clock clock;
	Time elapsed;
	
	float rotationAngle = 0.0f;
    
};

#endif