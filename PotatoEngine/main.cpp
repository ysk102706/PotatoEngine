#include <iostream>
#include "MainEngine.h" 

int main() { 
	Engine::MainEngine engine;

	if (!engine.Initialize()) {
		std::cout << "failed Engine Initialzie()\n";
		return 0;
	}

	engine.Run();
}