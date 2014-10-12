#pragma once

#include "ofMain.h"

#define NUM_OBJECTS_SQRT 100

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
    
    ofVec3f pos[NUM_OBJECTS_SQRT][NUM_OBJECTS_SQRT];
    ofVec3f size[NUM_OBJECTS_SQRT][NUM_OBJECTS_SQRT];
    ofQuaternion rot[NUM_OBJECTS_SQRT][NUM_OBJECTS_SQRT];
    ofFloatColor color[NUM_OBJECTS_SQRT][NUM_OBJECTS_SQRT];
    float hue[NUM_OBJECTS_SQRT][NUM_OBJECTS_SQRT];
    int textureRes;
    ofTexture posTex,sizeTex,rotTex,colorTex;
    
    ofVec3f target;
    
    bool lookAtSphere;
};
