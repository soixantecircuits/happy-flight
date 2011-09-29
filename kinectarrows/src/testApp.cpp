#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    // open an outgoing connection to HOST:PORT
	sender.setup( HOST, PORT );
    
	kinect.init();
	//kinect.init(true);  // shows infrared instead of RGB video image
	//kinect.init(false, false);  // disable infrared/rgb video iamge (faster fps)
	kinect.setVerbose(true);
	kinect.open();

	// start with the live kinect source
	kinectSource = &kinect;

	colorImg.allocate(kinect.width, kinect.height);
	grayImage.allocate(kinect.width, kinect.height);
    memoryGrayImage.allocate(kinect.width, kinect.height);
    grayImageWithoutBackground.allocate(kinect.width, kinect.height);
	grayThreshNear.allocate(kinect.width, kinect.height);
	grayThreshFar.allocate(kinect.width, kinect.height);
    colorImgLine.allocate(kinect.width, kinect.height);
    cannyImage.allocate(kinect.width, kinect.height);
    grayImageWithoutBackgroundAndBlackRects.allocate(kinect.width, kinect.height);

	nearThreshold = 254;
	farThreshold  = 30;
    lineThreshold = 32;
	bThreshWithOpenCV = true;
    speedBall = 20;
    avgAngles = 0;
    rightAngles = -12;
    leftAngles = 12;
    toleranceAngles = 0.6;
    
    lines = 0;
    storage = cvCreateMemStorage(0);

	ofSetFrameRate(60);

	bRecord = false;
	bPlayback = false;

	// zero the tilt on startup
	angle = 0;
	kinect.setCameraTiltAngle(angle);

	// start from the front
	pointCloudRotationY = 180;
	bDrawPointCloud = false;
    
    kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
    
    bLearnBakground = false;  
    threshold = 10;  
    
    blackRectPoints = new CvPoint[20];
    for (int i = 0; i < 20; i++) blackRectPoints[i] = cvPoint(-1,-1);
    int a =2;
    //cout << "kinect width,height" << kinect.width << ", " << kinect.height << endl;
}

//--------------------------------------------------------------
void testApp::update() {
    
    
	ofBackground(100, 100, 100);

	kinectSource->update();

	// there is a new frame and we are connected
	if(kinectSource->isFrameNew()) {
        

        
		// record ?
		if(bRecord && kinectRecorder.isOpened()) {
			kinectRecorder.newFrame(kinect.getRawDepthPixels(), kinect.getPixels());
		}

		// load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinectSource->getDepthPixels(), kinect.width, kinect.height);
        colorImgLine.setFromPixels(kinectSource->getPixels(), kinect.width, kinect.height);
        //cvCvtColor( colorImgLine.getCvImage(), grayImage.getCvImage(), CV_RGB2GRAY );
        
        if (bLearnBakground == true){  
            memoryGrayImage = grayImage;
            bLearnBakground = false;  
        }  
        
        // take the abs value of the difference between background and incoming and then threshold:  
        grayImageWithoutBackground.absDiff(memoryGrayImage, grayImage);  
        //grayImageWithoutBackground.threshold(nearThreshold);  
        
        
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
			grayThreshNear = grayImageWithoutBackground;
			grayThreshFar = grayImageWithoutBackground;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImageWithoutBackground.getCvImage(), NULL);


		// update the cv images
		grayImage.flagImageChanged();
        
		// find contours which are between the size of 20 pixels and 1/3 the w*h pixels.
    	// also, find holes is set to true so we will get interior contours as well....
    	//contourFinder.findContours(grayImage, 10, (kinect.width*kinect.height)/2, 20, false);
        
        /*
        int  cvHoughLines( CvArr* image, double rho,
                          double theta, int threshold,
                          float* lines, int linesNumber );
        */
        /*
         CvSeq* cvHoughLines2( CvArr* image, void* line_storage, int method,
         double rho, double theta, int threshold,
         double param1=0, double param2=0 );
         */
        
        //draw black squares
        grayImageWithoutBackgroundAndBlackRects = grayImageWithoutBackground;
        for( int i = 0; blackRectPoints[i+1].x != - 1 && i < 19 ; i+=2 )
        {
            //cout << "draw rect " << blackRectPoints[i].x << ", " << blackRectPoints[i].y << "to " << blackRectPoints[i+1].x << ", " << blackRectPoints[i+1].y << endl;
            cvRectangle( grayImageWithoutBackgroundAndBlackRects.getCvImage(), blackRectPoints[i], blackRectPoints[i+1], CV_RGB(0,0,0), CV_FILLED);
        }
  
        cvCanny( grayImageWithoutBackgroundAndBlackRects.getCvImage(), cannyImage.getCvImage(), 70, 200 );
        cvCvtColor( cannyImage.getCvImage(), colorImgLine.getCvImage(), CV_GRAY2BGR );
        lines = cvHoughLines2(  cannyImage.getCvImage(), storage, CV_HOUGH_STANDARD, 1, CV_PI/180, lineThreshold);
        //cvHoughLines( grayImage, 1, CV_PI/180, 100, lines, 100 );
        //cvHough
        totalAngles = 0;
        for( int i = 0; i < MIN(lines->total,100); i++ )
        {
            float* line = (float*)cvGetSeqElem(lines,i);
            float rho = line[0];
            float theta = line[1];
            CvPoint pt1, pt2;
            double a = cos(theta), b = sin(theta);
            double x0 = a*rho, y0 = b*rho;
            pt1.x = cvRound(x0 + 1000*(-b));
            pt1.y = cvRound(y0 + 1000*(a));
            pt2.x = cvRound(x0 - 1000*(-b));
            pt2.y = cvRound(y0 - 1000*(a));
            cvLine( colorImgLine.getCvImage(), pt1, pt2, CV_RGB(255,0,0), 3, 8 );
            totalAngles += (theta-CV_PI/2.);
        }
        smoothAngles = 0.7 * smoothAngles   + 0.3* totalAngles;
        
	}
}

//--------------------------------------------------------------
void testApp::draw() {

	ofSetColor(255, 255, 255);

	if(bDrawPointCloud) {
		ofPushMatrix();
		ofTranslate(420, 320);
		// we need a proper camera class
		drawPointCloud();
		ofPopMatrix();
	} else {
		if(!bPlayback) {
			// draw from the live kinect
			kinect.drawDepth(420, 10, 400, 300);
			kinect.draw(10, 10, 400, 300);
		} else {
			// draw from the player
			kinectPlayer.drawDepth(420, 10, 400, 300);
			kinectPlayer.draw(10, 10, 400, 300);
		}
        grayImage.draw(830, 10, 400, 300);
		grayImageWithoutBackground.draw(10, 320, 400, 300);
        grayImageWithoutBackgroundAndBlackRects.draw(420, 320, 400, 300);
        colorImgLine.draw(830, 320, 400, 300);
	}
    //send info
    if ( (smoothAngles > avgAngles) && abs(smoothAngles - avgAngles) > toleranceAngles * abs(leftAngles - avgAngles)){
        ofRect(420, 640, 20, 20);
        ofxOscMessage m;
        m.setAddress( "/keyboard/right" );
        m.addIntArg( 1 );
        sender.sendMessage( m );
    }
    else if ( (smoothAngles < avgAngles) && abs(smoothAngles - avgAngles) > toleranceAngles * abs(avgAngles - rightAngles)){
        ofRect(820, 640, 20, 20);
        ofxOscMessage m;
        m.setAddress( "/keyboard/left" );
        m.addIntArg( 1 );
        sender.sendMessage( m );
    }
    else {
        ofxOscMessage m;
        m.setAddress( "/keyboard/left" );
        m.addIntArg( 0 );
        sender.sendMessage( m );
        m.setAddress( "/keyboard/right" );
        m.addIntArg( 0 );
        sender.sendMessage( m );
    }
    
    //draw ball
    if (smoothAngles > avgAngles){
        float a = 200./(avgAngles - leftAngles);
        float b = -1 * a * avgAngles;
        ofCircle(620+(a*smoothAngles+b), 315, 30);

    }
    else{
        float a = -1 * 200./(avgAngles - rightAngles);
        float b = -1 * a * avgAngles;
        ofCircle(620+(a*smoothAngles+b), 315, 30);
    }
    
    //cout << (smoothAngles-avgAngles) << endl;
	// draw recording/playback indicators
	ofPushMatrix();
	ofTranslate(25, 25);
	ofFill();
	if(bRecord) {
		ofSetColor(255, 0, 0);
		ofCircle(0, 0, 10);
	}
	if(bPlayback) {
		ofSetColor(0, 255, 0);
		ofTriangle(-10, -10, -10, 10, 10, 0);
	}
	ofPopMatrix();


	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "calibration is: " << ofToString(leftAngles, 2) << " / "
								 << ofToString(avgAngles, 2) << " / "
								 << ofToString(rightAngles, 2) << endl
    << "value is:                 "<< ofToString(smoothAngles, 2) << endl
                    << "accel is: " << ofToString(kinect.getMksAccel().x, 2) << " / "
                    << ofToString(kinect.getMksAccel().y, 2) << " / "
                    << ofToString(kinect.getMksAccel().z, 2) << endl
				 << "press p to switch between images and point cloud, rotate the point cloud with the mouse" << endl
				 << "using opencv threshold = " << bThreshWithOpenCV <<" (press spacebar)" << endl
				 << "set line threshold " << lineThreshold << " (press: a z)" << endl
                << "set speed" << speedBall << " (press: q s)" << endl
                 << "set near threshold " << nearThreshold << " (press: + -)" << endl
				 << "set far threshold " << farThreshold << " (press: < >) num blobs found " << contourFinder.nBlobs
				 	<< ", fps: " << ofGetFrameRate() << endl
				 << "press c to close the connection and o to open it again, connection is: " << kinect.isConnected() << endl
				 << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl
				 << "press r to record and q to playback, record is: " << bRecord << ", playback is: " << bPlayback;
	ofDrawBitmapString(reportStream.str(),20,652);
}

void testApp::drawPointCloud() {
	ofScale(400, 400, 400);
	int w = 640;
	int h = 480;
	ofRotateY(pointCloudRotationY);
	glBegin(GL_POINTS);
	int step = 2;
	for(int y = 0; y < h; y += step) {
		for(int x = 0; x < w; x += step) {
			ofVec3f cur = kinect.getWorldCoordinateFor(x, y);
			ofColor color = kinect.getCalibratedColorAt(x,y);
			glColor3ub((unsigned char)color.r,(unsigned char)color.g,(unsigned char)color.b);
			glVertex3f(cur.x, cur.y, cur.z);
		}
	}
	glEnd();
}

//--------------------------------------------------------------
void testApp::exit() {
	kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
	kinectPlayer.close();
	kinectRecorder.close();
}

//--------------------------------------------------------------
void testApp::keyPressed (int key) {
	switch (key) {
        case 'f':
            for (int i = 0; i < 20; i++) blackRectPoints[i] = cvPoint(-1,-1);
            break;
            
		case ' ':
            avgAngles = smoothAngles;
            break;
        case 'c':
            leftAngles = smoothAngles;
            break;
        case 'v':
            rightAngles = smoothAngles;
            break;    
        case'@':
            bLearnBakground = true;  
			break;
        /*
		case'p':
			bDrawPointCloud = !bDrawPointCloud;
			break;
         */
		case '>':
		case '.':
			farThreshold ++;
			if (farThreshold > 255) farThreshold = 255;
			break;

		case '<':
		case ',':
			farThreshold --;
			if (farThreshold < 0) farThreshold = 0;
			break;

		case '+':
		case '=':
			nearThreshold ++;
			if (nearThreshold > 255) nearThreshold = 255;
			break;
        case 'o':
			toleranceAngles -=0.05;
			if (lineThreshold < 0) lineThreshold = 0;
			break;
            
		case 'p':
			lineThreshold += 0.05;
			if (lineThreshold > 2) lineThreshold = 2;
			break;
            
		case 'a':
			lineThreshold --;
			if (lineThreshold < 0) lineThreshold = 0;
			break;
            
		case 'z':
			lineThreshold ++;
			if (lineThreshold > 255) lineThreshold = 255;
			break;
            
        case 'q':
			speedBall --;
			if (speedBall < 0) speedBall = 0;
			break;
            
		case 's':
			speedBall ++;
			if (speedBall > 255) speedBall = 255;
			break;
            
		case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;

		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
        /*
		case 'o':
			kinect.setCameraTiltAngle(angle);	// go back to prev tilt
			kinect.open();
			break;
         */
/*
		case 'r':
			bRecord = !bRecord;
			if(bRecord) {
				startRecording();
			} else {
				stopRecording();
			}
			break;
		case 'q':
			bPlayback = !bPlayback;
			if(bPlayback) {
				startPlayback();
			} else {
				stopPlayback();
			}
			break;
*/
		case OF_KEY_UP:
			angle++;
			if(angle>30) angle=30;
			kinect.setCameraTiltAngle(angle);
			break;

		case OF_KEY_DOWN:
			angle--;
			if(angle<-30) angle=-30;
			kinect.setCameraTiltAngle(angle);
			break;
	}
}

//--------------------------------------------------------------
void testApp::mouseMoved(int x, int y) {
	pointCloudRotationY = x;
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    //if ( x > 10 && x < 410 && y > 320 && y <620)
    if (x < 10) x = 10;
    if (x > 410) x = 410;
    if (y < 320) y = 320;
    if (y > 620) y = 620;
        int i;
        for (i = 0; blackRectPoints[i].x != -1; i++);
        if (i < 19){
            blackRectPoints[i] = cvPoint((x-10) * 640 / 400 ,(y-320) * 640 / 400 );
            //cout << "first point " << blackRectPoints[i].x << ", " << blackRectPoints[i].y << endl;
        }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if ( x > 10 && x < 410 && y > 320 && y <620){
        int i;
        for (i = 0; blackRectPoints[i].x != -1; i++);
        if (i < 20){
            blackRectPoints[i] = cvPoint((x-10)* 640 / 400 ,(y-320)* 640 / 400 );
            //cout << "last point " << blackRectPoints[i].x << ", " << blackRectPoints[i].y << endl;
        }
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

//--------------------------------------------------------------
void testApp::startRecording() {

	// stop playback if running
	stopPlayback();

	kinectRecorder.init(ofToDataPath("recording.dat"));
	bRecord = true;
}

//--------------------------------------------------------------
void testApp::stopRecording() {
	kinectRecorder.close();
	bRecord = false;
}

//--------------------------------------------------------------
void testApp::startPlayback() {

	stopRecording();
	kinect.close();

	// set record file and source
	kinectPlayer.setup(ofToDataPath("recording.dat"), true);
	kinectPlayer.loop();
	kinectSource = &kinectPlayer;
	bPlayback = true;
}

//--------------------------------------------------------------
void testApp::stopPlayback() {
	kinectPlayer.close();
	kinect.open();
	kinectSource = &kinect;
	bPlayback = false;
}
