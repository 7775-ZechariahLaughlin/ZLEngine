#include "ZLEngine/Input.h"
#include "ZLEngine/Game.h"
#include <iostream>

using namespace std;

Input::Input()
{
	// assign the current keys by default
	KeyboardState = SDL_GetKeyboardState(NULL);
}

void Input::ProcessInput()
{
	//TODO: Handle inputs
	SDL_Event PollEvent;

	//this is waiting for inputs to be pressed
	while (SDL_PollEvent(&PollEvent)) {
		//checking what input was pressed
		switch (PollEvent.type) {
			// update all the keys on the keyboard with up or down states
		case SDL_KEYDOWN:
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYUP:
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_QUIT: //on close button pressed
			Game::GetGameInstance().CloseApp();
			break;
		default:
			break;
		}
	}
}

bool Input::IsKeyDown(SDL_Scancode Key)
{
	// make sure the keyboard state is set
	if (KeyboardState == nullptr) {
		cout << "Input | Keyboard State is not set." << endl;
		return false;
	}
	// return the result of the key
	return static_cast<bool>(KeyboardState[Key]);
}
