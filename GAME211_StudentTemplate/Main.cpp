#include <iostream>
#include "GameManager.h"

int main(int argc, char* args[]) { /// Standard C-style entry point, you need to use it

	GameManager *ptr = new GameManager();
	bool status  = ptr->OnCreate();
	if (status == true) {
		ptr->Run();
	} else if (status == false) {
		std::cerr << "Fatal error occured. Cannot start this program" << std::endl;
	}
	ptr->OnDestroy();
	delete ptr;
	return 0;

}
