#include "Algebra.hpp"
#include "Engine.hpp"

int main (int argc, char **argv) 
{
	srand(time(NULL));

	Engine engine(Vector2D(1460, 940));
	engine.Init();
	engine.Run();

	return 0;
}
