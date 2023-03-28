#include "ZLEngine/Input.h"
#include "ZLEngine/Game.h"
#include <iostream>

using namespace std;

Input::Input()
{
	// assign the current keys by default
	KeyboardState = SDL_GetKeyboardState(NULL);
	MouseX = MouseY = 0;
	MouseXDelta = MouseYDelta = 0;
	ScrollDelta = 0;
}

void Input::ProcessInput()
{
	// reset the mouse position deltas each frame
	MouseXDelta = MouseYDelta = 0;
	// reset the mouse scroll delta each frame
	ScrollDelta = 0;

	//TODO: Handle inputs
	SDL_Event PollEvent;

	// this is waiting for inputs to be pressed
	while (SDL_PollEvent(&PollEvent)) {
		// checking what input was pressed
		switch (PollEvent.type) {
		case SDL_MOUSEBUTTONDOWN:
			SetMouseButtonStates(PollEvent.button.button, true);
			break;
		case SDL_MOUSEBUTTONUP:
			SetMouseButtonStates(PollEvent.button.button, false);
			break;
		case SDL_MOUSEMOTION:
			OnMouseMove(PollEvent.motion);
			break;
		case SDL_MOUSEWHEEL:
			OnMouseScroll(PollEvent.wheel);
			break;
			// update all the keys on the keyboard with up or down states
		case SDL_KEYDOWN:
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_KEYUP:
			KeyboardState = SDL_GetKeyboardState(NULL);
			break;
		case SDL_QUIT: // on close button pressed
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

bool Input::IsMouseButtonDown(MouseButtons Button)
{
	return MouseButtonStates[Button];
}

void Input::ShowCursor(bool bVisibility)
{
	static SDL_bool bIsVisible = SDL_FALSE;

	// only run if the visibility of the cursor is opposite to what the input is asking for
	// ie. if we turn off the cursor but it's already off we don't need to turn it off
	// if the cursor is off and we ask for it to be on
	if (bVisibility && bIsVisible == SDL_TRUE) {
		bIsVisible = SDL_FALSE;
		SDL_SetRelativeMouseMode(bIsVisible);
	}
	// if the cursor is on and we ask for it to be off
	else if (!bVisibility && bIsVisible == SDL_FALSE) {
		bIsVisible = SDL_TRUE;
		SDL_SetRelativeMouseMode(bIsVisible);
	}
}

void Input::SetMouseButtonStates(Uint8 ButtonIndex, bool CurrentState)
{
	MouseButtonStates[ButtonIndex] = CurrentState;
}

void Input::OnMouseMove(SDL_MouseMotionEvent& MEvent)
{
	// the mouse position
	MouseX = MEvent.x;
	MouseY = MEvent.y;

	// movement of the mouse based on the last position
	// the relative movemenmt
	MouseXDelta += MEvent.xrel;
	MouseYDelta += MEvent.yrel;
}

void Input::OnMouseScroll(SDL_MouseWheelEvent& MEvent)
{
	// increase the delta relative to how much was scrolled
	ScrollDelta += MEvent.preciseY;
}

