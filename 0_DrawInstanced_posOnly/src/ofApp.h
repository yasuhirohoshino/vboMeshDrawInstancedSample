#pragma once

#include "ofMain.h"

#define NUM_OBJECTS 10000
#define SPHERE_RADIUS 8
#define SPHERE_RESOLUTION 8

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);
		
    ofShader vtfShader;
    ofVboMesh object;
    ofLight light1;
    ofEasyCam cam;
    
    ofVec3f pos[NUM_OBJECTS];
    int textureRes;
    ofTexture posTex;
};
