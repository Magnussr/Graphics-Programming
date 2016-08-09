/*including the WindowManager library so i can use what's in WindowManager */
#include "../WindowManager.h"

/*including memory so i can use utilities to manage dynamic memory
Need it to use Shared_ptr that lwets me create a pointer that i can share*/
#include <memory>

#ifdef _WIN32
#include <Windows.h>
#endif

/* Simple program that starts our Window manager */
int main(int argc, char *argv[]) {
	std::shared_ptr<WindowManager> window;
	window.reset(new WindowManager());
	window->init();
	window->play();
	window.reset();
	/*must have return 0 because we use int main */
	return 0;
}
