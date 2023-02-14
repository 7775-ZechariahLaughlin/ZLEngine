#pragma once

#include "SDL2/SDL.h"

class GraphicsEngine {

public: 
	GraphicsEngine();
	~GraphicsEngine();

	//create the window
	bool InitGE(const char* WTitle, bool bFullScreen, int WWidth, int WHeight);

	//show the newest frame graphics
	void PresentGraphics();
	//clear the old frame graphics
	void ClearGraphics();

	//return the sdl window
	SDL_Window* GetWindow() const;

private: 
	//this will hold the window
	SDL_Window* SdlWindow;
	//this will allow OpenGL to work in SDL
	SDL_GLContext SdlGLContext;
};