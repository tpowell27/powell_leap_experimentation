##Leap Hands
This project explores the uses of the Leap motion controller. As the hands move farther away they change color until they eventually inplode. As one raises their hands up and down spikes are add to either the fingertips or the knuckles.

The code for the different shapes is a simple if statement that covers all locations.
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
            

The for the color change an if statement is sructured around the z position of the hands.

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
        