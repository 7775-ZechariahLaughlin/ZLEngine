#include "ZLEngine/Game.h"

int main(int argc, char** argv){

	Game::GetGameInstance().Start("ZLEngine | An OpenGL Engine", false, 1200, 720);

	Game::DestroyGameInstance();

	return 0;
}