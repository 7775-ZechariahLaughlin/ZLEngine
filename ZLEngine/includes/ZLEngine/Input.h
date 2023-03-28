#pragma once
#include "SDL2/SDL.h"

enum MouseButtons {
	LEFT = SDL_BUTTON_LEFT,
	MIDDLE = SDL_BUTTON_MIDDLE,
	RIGHT = SDL_BUTTON_RIGHT
};
class Input {
public: 
	Input();
	
	// detect the input of the user
	void ProcessInput();
	
	// detect if a specific key is down based on an SDL Scancode
	// Hint* SDL_SCANCODE_KEY (eg. SDL_SCANCODE_W)
	bool IsKeyDown(SDL_Scancode Key);

	// detect if the mouse button inputted is being pressed down
	bool IsMouseButtonDown(MouseButtons Button);

	// change the visibility of the cursor
	void ShowCursor(bool bVisibility);

private:
	// update the mouse button states with the current state of the mouse
	void SetMouseButtonStates(Uint8 ButtonIndex, bool CurrentState);

	// handle the variables that rely on mouse motion
	void OnMouseMove(SDL_MouseMotionEvent& MEvent);

	// handle the variables that rely on the scroll of the mouse
	void OnMouseScroll(SDL_MouseWheelEvent& MEvent);

public:
	// mouse position
	int MouseX, MouseY;

	// how far the mouse has moved since the last check/tick
	int MouseXDelta, MouseYDelta;

	// how far the player has scrolled since the last frame
	float ScrollDelta;

private:
	// keyboard current states
	const Uint8* KeyboardState;

	// mouse button current states
	bool MouseButtonStates[10] = { false };
};
