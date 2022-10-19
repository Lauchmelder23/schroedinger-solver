#include <stdio.h>

#include "application.h"

int main(int argc, char** argv)
{
	Application app;
	if (init_application(&app, "Schrodinger Solver") != 0)
	{
		return 1;
	}

	return launch_application(&app);
}
