#include "ofMain.h"
#include "ofxGui.h"

class App : public ofBaseApp{
 public:
   ofShader sh;
   ofFbo fbo,tex;
   ofEasyCam cam;
   ofImage img;
   ofxPanel gui;
   ofParameter<float> pix;
   ofParameter<ofVec3f> cols;
   ofParameter<ofVec3f> cols2;
   ofParameter<bool> siTex;
   int w,h;

   void setup(){
      w=ofGetScreenWidth();
      h=ofGetScreenHeight();

      gui.setup();
      gui.add(pix.set("pixelSize",0.25f,0.f,50.f));
      gui.add(cols.set("cols",ofVec3f(0.6f),ofVec3f(0.2f),ofVec3f(1.f)));
      gui.add(cols2.set("cols2",ofVec3f(0.2f),ofVec3f(0.f),ofVec3f(1.f)));
      gui.add(siTex.set("texture",false));

      ofDisableArbTex();
      sh.load("vertex.glsl","fragment.glsl");
      fbo.allocate(w,h);
      tex.allocate(w,h);
      img.loadImage("img.jpg");
   }

   void update(){
      fbo.begin();
      ofClear(0,0,0,0);
      float movementSpeed = .1;
      float cloudSize = ofGetWidth() / 2;
      float maxBoxSize = 100;
      float spacing = 1;
      int boxCount = 100;
      
      cam.begin();
      ofPushStyle();
      ofRotateX(ofSignedNoise(ofGetElapsedTimef(), 0, 0));
      for(int i = 0; i < boxCount; i++) {
         ofPushMatrix();
         
         float t = (ofGetElapsedTimef() + i * spacing) * movementSpeed;
         ofVec3f pos(
                     ofSignedNoise(t, 0, 0),
                     ofSignedNoise(0, t, 0),
                     ofSignedNoise(0, 0, t));
         
         float boxSize = maxBoxSize * ofNoise(pos.x, pos.y, pos.z);
         
         pos *= cloudSize;
         ofTranslate(pos);
         ofRotateX(pos.x);
         ofRotateY(pos.y);
         ofRotateZ(pos.z);
         
         ofFill();
         ofSetColor(255);
         ofDrawBox(boxSize);
         
         ofNoFill();
         ofSetColor(ofColor::fromHsb(sinf(t) * 128 + 128, 255, 255));
         ofDrawBox(boxSize * 1.1f);
         
         ofPopMatrix();
         }
      ofPopStyle();
      cam.end();
      fbo.end();

      tex.begin();
      ofClear(0,0,0,0);
      fbo.draw(0,0);
      tex.end();
   }

   void draw(){

      ofBackground(255,0,0);

      sh.begin();
      sh.setUniformTexture("tInput",fbo.getTextureReference(),0);
      sh.setUniformTexture("texture",tex.getTextureReference(),1);
      sh.setUniform2f("resolution",w,h);
      sh.setUniform3f("cols",cols->x,cols->y,cols->z);
      sh.setUniform3f("cols2",cols2->x,cols2->y,cols2->z);
      sh.setUniform1f("pixelSize",pix);
      sh.setUniform1i("active",siTex);
           fbo.draw(0,0);
      sh.end();
      tex.draw(0,0,w/4,h/4);

      gui.draw();
   }
};

int main(){
	ofSetupOpenGL(1024,768, OF_WINDOW);
	ofRunApp( new App());
}
