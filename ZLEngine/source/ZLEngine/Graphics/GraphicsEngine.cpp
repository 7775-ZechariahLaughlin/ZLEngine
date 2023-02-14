#include "ZLEngine/Graphics/GraphicsEngine.h"
#include <iostream>
#include "GLEW/glew.h"

using namespace std; 

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
}

GraphicsEngine::~GraphicsEngine()
{
	cout << "Destroyed Graphics Engine... " << endl; 
	//this will handle deleting the SDL window from memory
	SDL_DestroyWindow(SdlWindow);
	//destroy the GL context for SDL
	SDL_GL_DeleteContext(SdlGLContext);
	//close the SDL framework
	SDL_Quit();
}

bool GraphicsEngine::InitGE(const char* WTitle, bool bFullScreen, int WWidth, int WHeight)
{
	//make sure SDL Initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL Initialisation Failed: " << SDL_GetError() << endl;
		return false;
	}

	//use OpenGL 3.1 and set default attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);

	//set the fullscreen flag
	int FullscreenFlag = 0;

	if (bFullScreen) {
		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN;
	}
	else {

		FullscreenFlag = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;
	}


	//create the SDL2 window

	SdlWindow = SDL_CreateWindow(
		WTitle, //title of window
		SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, //position of window
		WWidth, WHeight, //dimensions of window
		FullscreenFlag
	);

	//if the window fails
	if (SdlWindow == nullptr) {
		cout << "SDL Window Failed: " << SDL_GetError() << endl;
		return false;
	}

	//add all the GL attributes to the window
	SdlGLContext = SDL_GL_CreateContext(SdlWindow);

	if (SdlGLContext == NULL) {
		cout << "SDL GL Context Failed: " << SDL_GetError() << endl;
		return false;
	}
	
	//to make glew work we need to mark experimental true
	glewExperimental = GL_TRUE;

	const GLenum InitGLEW = glewInit();

	if (InitGLEW != GLEW_OK) {

		cout << "GLEW failed: " << glewGetErrorString(InitGLEW) << endl;
	}

	return true;
}

void GraphicsEngine::PresentGraphics()
{
	//present the new graphics using opengl
	SDL_GL_SwapWindow(SdlWindow);
}

void GraphicsEngine::ClearGraphics()
{
	//set the background colour
	glClearColor(0.23f, 0.38f, 0.47f, 1.0f);

	//clear the screen
	glClear(GL_COLOR_BUFFER_BIT);
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}
