#include "ofApp.h"

/* Note on OS X, you must have this in the Run Script Build Phase of your project.
 where the first path ../../../addons/ofxLeapMotion2/ is the path to the ofxLeapMotion addon.
 
 cp -f ../../../addons/ofxLeapMotion2/libs/lib/osx/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/libLeap.dylib"; install_name_tool -change ./libLeap.dylib @executable_path/libLeap.dylib "$TARGET_BUILD_DIR/$PRODUCT_NAME.app/Contents/MacOS/$PRODUCT_NAME";
 
 If you don't have this you'll see an error in the console: dyld: Library not loaded: @loader_path/libLeap.dylib
 */

//--------------------------------------------------------------


void ofApp::setup(){
    
    ofSetFrameRate(60);
    ofSetVerticalSync(true);
	ofSetLogLevel(OF_LOG_VERBOSE);
    
	leap.open();
    
	cam.setOrientation(ofPoint(-20, 0, 0));
    
	glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    
}


//--------------------------------------------------------------
void ofApp::update(){
	fingersFound.clear();
	
	//here is a simple example of getting the hands and drawing each finger and joint
	//the leap data is delivered in a threaded callback - so it can be easier to work with this copied hand data
	
	//if instead you want to get the data as it comes in then you can inherit ofxLeapMotion and implement the onFrame method.
	//there you can work with the frame data directly.
    
    
    
    //Option 1: Use the simple ofxLeapMotionSimpleHand - this gives you quick access to fingers and palms.
    
    
    simpleHands = leap.getSimpleHands();
    
    if( leap.isFrameNew() && simpleHands.size() ){
        
        leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
		leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
        leap.setMappingZ(-150, 150, -200, 200);
        
        fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
        
        for(int i = 0; i < simpleHands.size(); i++){
            for (int f=0; f<5; f++) {
                int id = simpleHands[i].fingers[ fingerTypes[f] ].id;
                ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp; // metacarpal
                ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip; // proximal
                ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip; // distal
                ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip; // fingertip
                fingersFound.push_back(id);
            }
        }
    }
    
    
    
    //Option 2: Work with the leap data / sdk directly - gives you access to more properties than the simple approach
    //uncomment code below and comment the code above to use this approach. You can also inhereit ofxLeapMotion and get the data directly via the onFrame callback.
    
     vector <Hand> hands = leap.getLeapHands();
     if( leap.isFrameNew() && hands.size() ){
     
     //leap returns data in mm - lets set a mapping to our world space.
     //you can get back a mapped point by using ofxLeapMotion::getMappedofPoint with the Leap::Vector that tipPosition returns
     leap.setMappingX(-230, 230, -ofGetWidth()/2, ofGetWidth()/2);
     leap.setMappingY(90, 490, -ofGetHeight()/2, ofGetHeight()/2);
     leap.setMappingZ(-150, 150, -200, 200);
     
     fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
     
     for(int i = 0; i < hands.size(); i++){
     for(int j = 0; j < 5; j++){
     ofPoint pt;
     
     const Finger & finger = hands[i].fingers()[ fingerTypes[j] ];
     
     //here we convert the Leap point to an ofPoint - with mapping of coordinates
     //if you just want the raw point - use ofxLeapMotion::getofPoint
     
     pt = leap.getMappedofPoint( finger.tipPosition() );
     pt = leap.getMappedofPoint( finger.jointPosition(finger.JOINT_DIP) );
     
     fingersFound.push_back(finger.id());
     
     }
     }
     }
     
    
	//IMPORTANT! - tell ofxLeapMotion that the frame is no longer new.
	leap.markFrameAsOld();
}

//--------------------------------------------------------------
void ofApp::draw(){
    ofBackgroundGradient(ofColor(90, 90, 90), ofColor(30, 30, 30),  OF_GRADIENT_BAR);
	
	ofSetColor(200);
	ofDrawBitmapString("ofxLeapMotion - Example App\nLeap Connected? " + ofToString(leap.isConnected()), 20, 20);
    
	cam.begin();
    
	ofPushMatrix();
    ofRotate(90, 0, 0, 1);
    ofSetColor(20);
    ofDrawGridPlane(800, 20, false);
	ofPopMatrix();
    
    //ofDrawPlane(0,0,-150,1000,1000) ;
    
    
    fingerType fingerTypes[] = {THUMB, INDEX, MIDDLE, RING, PINKY};
    
    for(int i = 0; i < simpleHands.size(); i++){
        bool isLeft        = simpleHands[i].isLeft;
        ofPoint handPos    = simpleHands[i].handPos;
        ofPoint handNormal = simpleHands[i].handNormal;
        
        ofSetColor(225);
        ofDrawSphere(handPos.x, handPos.y, handPos.z, 30);
        ofSetColor(0, 0, 0);
        ofDrawArrow(handPos, handPos + 100*handNormal);
        if(handPos.z <= -25) {
            ofSetColor(100,100,100) ;
        }
        if(handPos.z <= -50) {
            ofSetColor(0,0,255) ;
        }
        if(handPos.z <= -100) {
            ofSetColor(0,255,0) ;
        }
        if(handPos.z <= -150) {
            ofSetColor(255,0,0) ;
        }
        
        for (int f=0; f<5; f++) {
            ofPoint mcp = simpleHands[i].fingers[ fingerTypes[f] ].mcp;  // metacarpal
            ofPoint pip = simpleHands[i].fingers[ fingerTypes[f] ].pip;  // proximal
            ofPoint dip = simpleHands[i].fingers[ fingerTypes[f] ].dip;  // distal
            ofPoint tip = simpleHands[i].fingers[ fingerTypes[f] ].tip;  // fingertip
            
            if(handPos.z <=-300) {
                ofSetColor(ofRandom(50,255), ofRandom(0,255), ofRandom(100,155));
                ofRotate(180);
            }
            
            if(handPos.x >= 0) {
                
                if(handPos.y>=ofGetHeight()/4) {
                    ofDrawBox(mcp.x, mcp.y, mcp.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 80) );
                    ofDrawBox(pip.x, pip.y, pip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 60) );
                    ofDrawBox(dip.x, dip.y, dip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 50) );
                    ofDrawCone(tip.x, tip.y-30, tip.z, 10, 60);

                }
                
                else {
                    ofDrawBox(mcp.x, mcp.y, mcp.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 80) );
                    ofDrawBox(pip.x, pip.y, pip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 60) );
                    ofDrawBox(dip.x, dip.y, dip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 50) );
                    ofDrawBox(tip.x, tip.y, tip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 40) );
                }
            }
            
            if(handPos.x <= -1) {
                
                if(handPos.y>=ofGetHeight()/4) {
                    ofDrawSphere(mcp.x, mcp.y, mcp.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 80) );
                    ofDrawSphere(pip.x, pip.y, pip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 60) );
                    ofDrawSphere(dip.x, dip.y, dip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 50) );
                    ofDrawCone(tip.x, tip.y-30, tip.z, 10, 60);
                }
                
                
                else {
                    
                    ofDrawSphere(mcp.x, mcp.y, mcp.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 80) );
                    ofDrawSphere(pip.x, pip.y, pip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 60) );
                    ofDrawSphere(dip.x, dip.y, dip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 50) );
                    ofDrawSphere(tip.x, tip.y, tip.z, ofMap(handPos.y, 0, ofGetWindowHeight(), 30, 40) );
                }
            }
            
            ofSetLineWidth(5);
            ofLine(mcp.x, mcp.y, mcp.z, pip.x, pip.y, pip.z);
            ofLine(pip.x, pip.y, pip.z, dip.x, dip.y, dip.z);
            ofLine(dip.x, dip.y, dip.z, tip.x, tip.y, tip.z);
            
        }
    }
	cam.end();
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

//--------------------------------------------------------------
void ofApp::exit(){
    // let's close down Leap and kill the controller
    leap.close();
}
