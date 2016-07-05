#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( )
{
    ofGLWindowSettings settings;
    
    settings.setGLVersion(4,1);
    settings.width  = 600;
    settings.height = 400;
    
    ofCreateWindow(settings);
    
    ofRunApp(new ofApp());
}
