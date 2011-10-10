#include "testApp.h"
#include "ofAppGlutWindow.h"

int main() {
	ofAppGlutWindow window;
	ofSetupOpenGL(&window, 830, 768, OF_WINDOW);
	ofRunApp(new testApp());
}
