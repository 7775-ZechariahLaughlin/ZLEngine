#include "ZLEngine/Graphics/GraphicsEngine.h"
#include "GLEW/glew.h"
#include "ZLEngine/Graphics/VertexArrayObject.h"
#include "ZLEngine/Graphics/ShaderProgram.h"
#include "GLM/glm.hpp"
#include "GLM/gtc/matrix_transform.hpp"

GraphicsEngine::GraphicsEngine()
{
	SdlWindow = nullptr;
	SdlGLContext = NULL;
	bWireframeMode = false;
}

GraphicsEngine::~GraphicsEngine()
{
	//this will handle deleting the SDL window from memory
	SDL_DestroyWindow(SdlWindow);
	//destroy the GL context for SDL
	SDL_GL_DeleteContext(SdlGLContext);
	//close the SDL framework
	SDL_Quit();

	cout << "Destroyed Graphics Engine... " << endl;
}

bool GraphicsEngine::InitGE(const char* WTitle, bool bFullScreen, int WWidth, int WHeight)
{
	//make sure SDL Initialises
	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		cout << "SDL Initialisation Failed: " << SDL_GetError() << endl;
		return false;
	}

	//use OpenGL 4.6 compatibility and set default attributes
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_COMPATIBILITY);
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

void GraphicsEngine::Draw()
{
	ClearGraphics();

	HandleWireframeMode(false);

	zluint index = 0;
	// TODO: Add anything that renders between these two functions
	for (VAOPtr VAO : VAOs) {
		Shader->RunShader();

		//move the object
		glm::mat4 transform = glm::mat4(1.0f);

		if (index == 0) { // Square
			//move in the x, y or, z direction based on the amount added
			transform = glm::translate(transform, glm::vec3(0.5f, 0.0f, 0.0f));
			// radians is the rotation amount
			// vec3 is the direction to rotate in
			transform = glm::rotate(transform, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f));
			transform = glm::scale(transform, glm::vec3(0.5f, 0.5f, 1.0f));
		} 
		else if (index == 1){ // Triangle
			transform = glm::translate(transform, glm::vec3(-0.5f, 0.0f, 0.0f));
			// x and y will work for our 2D shapes
			// z must be larger than 0 or you won't see the object (1 is default)
			transform = glm::scale(transform, glm::vec3(1.25f, 1.25f, 1.0f));
		}

		Shader->SetMat4("transform", transform);
		//draw each VAO
		VAO->Draw();

		index++;
	}

	PresentGraphics();
}

SDL_Window* GraphicsEngine::GetWindow() const
{
	return SdlWindow;
}

void GraphicsEngine::CreateVAO(GeometricShapes Shape)
{
	//create a new VAO as a shared pointer
	VAOPtr NewVAO = make_shared<VAO>(Shape);
	//add it to the stack
	VAOs.push_back(NewVAO);
}

void GraphicsEngine::CreateShader(VFShaderParams ShaderFilePaths)
{
	// create a new shader class
	ShaderPtr NewShader = make_shared<ShaderProgram>();

	// initialise the shader into OpenGL using the file paths
	NewShader->InitVFShader(ShaderFilePaths);

	// add the shader to our graphics engine
	Shader = NewShader;


}

void GraphicsEngine::HandleWireframeMode(bool bShowWireframeMode)
{
	// if wireframe mode is set, change it and vice versa
	if (bShowWireframeMode != bWireframeMode) {
		bWireframeMode = bShowWireframeMode;

		//change how OpenGL renders between the vertices
		if (bWireframeMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}

		cout << "Wireframe mode updated..." << endl;
	}
}
