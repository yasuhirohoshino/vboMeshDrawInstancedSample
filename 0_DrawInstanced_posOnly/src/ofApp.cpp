#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
    cam.setupPerspective(true,60,0.1,2000);
    vtfShader.load("shaders/vtf.vert","shaders/render.frag");
    
    light1.setPointLight();
    light1.setAmbientColor(ofFloatColor(0.3,0.3,0.3,1.0));
    light1.setDiffuseColor(ofFloatColor(0.6,0.6,0.6,1.0));
    light1.setSpecularColor(ofFloatColor(1.0,1.0,1.0,1.0));
    light1.setPosition(400, -400, 400);
    
    ofSpherePrimitive _sp;
    _sp.setRadius(SPHERE_RADIUS);
    _sp.setResolution(SPHERE_RESOLUTION);
    object = _sp.getMesh();
    
    for(int i=0;i<NUM_OBJECTS;i++){
        float s,t,r;
        s = ofRandom(0,2*PI);
        t = ofRandom(0,PI);
        r = ofRandom(150,400);
        pos[i].set(r*sin(s)*cos(t),r*sin(s)*sin(t),r*cos(s));
    }
    textureRes = ofNextPow2(sqrt(NUM_OBJECTS));
    float posArray[textureRes*textureRes*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            posArray[i*3 + 0] = pos[i].x;
            posArray[i*3 + 1] = pos[i].y;
            posArray[i*3 + 2] = pos[i].z;
        }
    }
    posTex.allocate(textureRes, textureRes, GL_RGB32F);
    posTex.loadData(posArray, textureRes, textureRes, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::update(){
    for(int i=0;i<NUM_OBJECTS;i++){
        pos[i]+=ofVec3f(ofRandom(-1,1),ofRandom(-1,1),ofRandom(-1,1));
    }
    float posArray[textureRes*textureRes*3];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            posArray[i*3 + 0] = pos[i].x;
            posArray[i*3 + 1] = pos[i].y;
            posArray[i*3 + 2] = pos[i].z;
        }
    }
    posTex.loadData(posArray, textureRes, textureRes, GL_RGB);
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(255), ofColor(212));
    ofEnableDepthTest();
    ofEnableLighting();
    cam.begin();
    light1.enable();
    vtfShader.begin();
    vtfShader.setUniform4f("COMMON_Ambient",0.1,0.1,0.1,1.0);
    vtfShader.setUniform4f("COMMON_Diffuse",0.5,0.5,0.5,1.0);
    vtfShader.setUniform4f("COMMON_Specular",0.5,0.5,0.5,1.0);
    vtfShader.setUniform1f("COMMON_Shininess",10.0);
    vtfShader.setUniform1i("ADS_NumLights",ofGetLightingEnabled());
    vtfShader.setUniformTexture("posTex", posTex,0);
    vtfShader.setUniform1f("iCount", textureRes);
    object.drawInstanced(OF_MESH_FILL, NUM_OBJECTS);
    vtfShader.end();
    light1.disable();
    cam.end();
    ofDisableLighting();
    ofDisableDepthTest();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
