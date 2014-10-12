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
    light1.setPosition(500, -500, 500);
    
    ofBoxPrimitive b;
    b.set(1, 1, 1, 1, 1, 1);
    object = b.getMesh();
    
    textureRes = NUM_OBJECTS_SQRT;
    target.set(0,0,0);
    lookAtSphere=false;
    
    for(int i=0;i<textureRes;i++){
        for(int j=0;j<textureRes;j++){
            size[i][j].set(10,ofRandom(10,150),10);
            float x = ofMap(i,0,textureRes,-500,500);
            float y = 100-size[i][j].y/2;
            float z = ofMap(j,0,textureRes,-500,500);
            pos[i][j].set(x,y,z);
            hue[i][j] = ofRandom(0.3,0.6);
            color[i][j].setHsb(hue[i][j], 0.8, 1);
        }
    }
    float posArray[textureRes*textureRes*3];
    float sizeArray[textureRes*textureRes*3];
    float rotArray[textureRes*textureRes*4];
    float colorArray[textureRes*textureRes*4];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            posArray[i*3 + 0] = pos[x][y].x;
            posArray[i*3 + 1] = pos[x][y].y;
            posArray[i*3 + 2] = pos[x][y].z;
            sizeArray[i*3 + 0] = size[x][y].x;
            sizeArray[i*3 + 1] = size[x][y].y;
            sizeArray[i*3 + 2] = size[x][y].z;
            rotArray[i*4 + 0] = 0;
            rotArray[i*4 + 1] = 0;
            rotArray[i*4 + 2] = 0;
            rotArray[i*4 + 3] = 1;
            colorArray[i*4 + 0] = color[x][y].r;
            colorArray[i*4 + 1] = color[x][y].g;
            colorArray[i*4 + 2] = color[x][y].b;
            colorArray[i*4 + 3] = 1;
        }
    }
    posTex.allocate(textureRes, textureRes, GL_RGB32F);
    posTex.loadData(posArray, textureRes, textureRes, GL_RGB);
    sizeTex.allocate(textureRes, textureRes, GL_RGB32F);
    sizeTex.loadData(sizeArray, textureRes, textureRes, GL_RGB);
    rotTex.allocate(textureRes, textureRes, GL_RGBA32F);
    rotTex.loadData(rotArray, textureRes, textureRes, GL_RGBA);
    colorTex.allocate(textureRes, textureRes, GL_RGBA32F);
    colorTex.loadData(colorArray, textureRes, textureRes, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update(){
    ofSetWindowTitle(ofToString(ofGetFrameRate()));
    float time = ofGetElapsedTimef();
    if(lookAtSphere)target.set(500*ofSignedNoise(1,0,0,time*0.2),-50+50*ofSignedNoise(1,0,0,time*0.2),500*ofSignedNoise(0,0,1,time*0.2));
    for(int i=0;i<textureRes;i++){
        for(int j=0;j<textureRes;j++){
            size[i][j].set(10,10+70+70*ofSignedNoise(i,j,0,time*0.5),10);
            pos[i][j].set(ofMap(i,0,textureRes,-500,500),
                          100-size[i][j].y/2,
                          ofMap(j,0,textureRes,-500,500));
            hue[i][j] += 0.001;
            hue[i][j] = fmodf(hue[i][j], 1.0);
            color[i][j].setHsb(hue[i][j], 0.8, 1);
            if(lookAtSphere){
                ofQuaternion _q;
                _q.makeRotate((target-pos[i][j]).getNormalized(), ofVec3f(0,-1,0));
                rot[i][j] = _q;
            }else{
                rot[i][j].set(0, 0, 0, 1);
            }
        }
    }
    float posArray[textureRes*textureRes*3];
    float sizeArray[textureRes*textureRes*3];
    float rotArray[textureRes*textureRes*4];
    float colorArray[textureRes*textureRes*4];
    for (int x = 0; x < textureRes; x++){
        for (int y = 0; y < textureRes; y++){
            int i = textureRes * y + x;
            posArray[i*3 + 0] = pos[x][y].x;
            posArray[i*3 + 1] = pos[x][y].y;
            posArray[i*3 + 2] = pos[x][y].z;
            sizeArray[i*3 + 0] = size[x][y].x;
            sizeArray[i*3 + 1] = size[x][y].y;
            sizeArray[i*3 + 2] = size[x][y].z;
            rotArray[i*4 + 0] = rot[x][y].x();
            rotArray[i*4 + 1] = rot[x][y].y();
            rotArray[i*4 + 2] = rot[x][y].z();
            rotArray[i*4 + 3] = rot[x][y].w();
            colorArray[i*4 + 0] = color[x][y].r;
            colorArray[i*4 + 1] = color[x][y].g;
            colorArray[i*4 + 2] = color[x][y].b;
            colorArray[i*4 + 3] = 1.0;
        }
    }
    posTex.loadData(posArray, textureRes, textureRes, GL_RGB);
    sizeTex.loadData(sizeArray, textureRes, textureRes, GL_RGB);
    rotTex.loadData(rotArray, textureRes, textureRes, GL_RGBA);
    colorTex.loadData(colorArray, textureRes, textureRes, GL_RGBA);
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
    vtfShader.setUniform1f("COMMON_Shininess",100.0);
    vtfShader.setUniform1i("ADS_NumLights",ofGetLightingEnabled());
    vtfShader.setUniformTexture("posTex", posTex,0);
    vtfShader.setUniformTexture("sizeTex", sizeTex,1);
    vtfShader.setUniformTexture("rotTex", rotTex,2);
    vtfShader.setUniformTexture("colorTex", colorTex,3);
    vtfShader.setUniform1f("iCount", textureRes);
    object.drawInstanced(OF_MESH_FILL, textureRes*textureRes);
    vtfShader.end();
    if(lookAtSphere)ofDrawSphere(target, 50);
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
    lookAtSphere = !lookAtSphere;
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
