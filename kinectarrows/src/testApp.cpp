#include "testApp.h"


//--------------------------------------------------------------
void testApp::setup() {
    // open an outgoing connection to HOST:PORT
	sender.setup( HOST, PORT );
    
	kinect.init();
	kinect.setVerbose(true);
	kinect.open();

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
    
    avgAngles = 0;
    rightAngles = -12;
    leftAngles = 12;
    toleranceAngles = 0.6;
    
    lines = 0;
    storage = cvCreateMemStorage(0);

	ofSetFrameRate(60);

	// zero the tilt on startup
	//angle = 0;
	//kinect.setCameraTiltAngle(angle);

    
    kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
    
    bLearnBakground = false;
    bDoBgSub = true;
    
    blackRectPoints = new CvPoint[20];
    for (int i = 0; i < 20; i++) blackRectPoints[i] = cvPoint(-1,-1);
}

//--------------------------------------------------------------
void testApp::update() {
    
    
	ofBackground(100, 100, 100);

	kinect.update();

	// there is a new frame and we are connected
	if(kinect.isFrameNew()) {

		// LOAD IMAGE: load grayscale depth image from the kinect source
		grayImage.setFromPixels(kinect.getDepthPixels(), kinect.width, kinect.height);
        colorImgLine.setFromPixels(kinect.getPixels(), kinect.width, kinect.height);
        //////////////////////////////
        
        //BACKGROUND SUBTRACTION//////
        if (bLearnBakground == true && bDoBgSub == true){  
            memoryGrayImage = grayImage;
            bLearnBakground = false;  
        }  
        if (bDoBgSub == true){
            // take the abs value of the difference between background and incoming and then threshold:  
            grayImageWithoutBackground.absDiff(memoryGrayImage, grayImage);  
        } else {
            grayImageWithoutBackground = grayImage;
        }
        //////////////////////////////
        
        
        //THRESHOLD IMAGE/////////////
		// we do two thresholds - one for the far plane and one for the near plane
		// we then do a cvAnd to get the pixels which are a union of the two thresholds
			grayThreshNear = grayImageWithoutBackground;
			grayThreshFar = grayImageWithoutBackground;
			grayThreshNear.threshold(nearThreshold, true);
			grayThreshFar.threshold(farThreshold);
			cvAnd(grayThreshNear.getCvImage(), grayThreshFar.getCvImage(), grayImageWithoutBackground.getCvImage(), NULL);
		// update the cv images
		grayImage.flagImageChanged();
        //////////////////////////////
        
		//REMOVE ANNOYING OBJECTS BY HAND/////
        //draw black squares
        grayImageWithoutBackgroundAndBlackRects = grayImageWithoutBackground;
        for( int i = 0; blackRectPoints[i+1].x != - 1 && i < 19 ; i+=2 )
        {
            cvRectangle( grayImageWithoutBackgroundAndBlackRects.getCvImage(), blackRectPoints[i], blackRectPoints[i+1], CV_RGB(0,0,0), CV_FILLED);
        }
        //////////////////////////////
  
        //DETECT LINES WITH HOUGH TRANSFORM
        cvCanny( grayImageWithoutBackgroundAndBlackRects.getCvImage(), cannyImage.getCvImage(), 70, 200 );
        cvCvtColor( cannyImage.getCvImage(), colorImgLine.getCvImage(), CV_GRAY2BGR );
        lines = cvHoughLines2(  cannyImage.getCvImage(), storage, CV_HOUGH_STANDARD, 1, CV_PI/180, lineThreshold);
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
        //////////////////////////////
        
        //DETECT AVERAGE DIRECTION
        smoothAngles = 0.7 * smoothAngles   + 0.3* totalAngles;
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
        //////////////////////////////
	}
}

//--------------------------------------------------------------
void testApp::draw() {

	ofSetColor(255, 255, 255);

    // draw from the live kinect
    kinect.drawDepth(420, 10, 400, 300);
    kinect.draw(10, 10, 400, 300);
    //grayImage.draw(830, 10, 400, 300);
    //grayImageWithoutBackground.draw(10, 320, 400, 300);
    grayImageWithoutBackgroundAndBlackRects.draw(10, 320, 400, 300);
    colorImgLine.draw(420, 320, 400, 300);
    
    //draw right and left squares
    if ( (smoothAngles > avgAngles) && abs(smoothAngles - avgAngles) > toleranceAngles * abs(leftAngles - avgAngles)){
        ofRect(420, 640, 20, 20);
    }
    else if ( (smoothAngles < avgAngles) && abs(smoothAngles - avgAngles) > toleranceAngles * abs(avgAngles - rightAngles)){
        ofRect(820, 640, 20, 20);
    }
    
    //draw moving ball
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


	// draw instructions
	ofSetColor(255, 255, 255);
	stringstream reportStream;
	reportStream << "calibration is: " << ofToString(leftAngles, 2) << " / "
                                       << ofToString(avgAngles, 2) << " / "
                                       << ofToString(rightAngles, 2) << endl
                 << "value is:           "<< ofToString(smoothAngles, 2) << endl
                 << "to calibrate, press c for left, v for right, space bar for middle" << endl
                 << "press @ to do a background subtraction, & to cancel" << endl
                 << "set tolerance for saying right or left " << toleranceAngles << " (press: o p)" << endl
				 << "set line threshold " << lineThreshold << " (press: a z)" << endl
                 << "set near threshold " << nearThreshold << " (press: + -)" << endl
				 << "set far threshold "  << farThreshold << " (press: < >)"
                 << "click on image to draw a black rectangle, press f to clear all" << endl
                 << "press UP and DOWN to change the tilt angle: " << angle << " degrees" << endl;
	ofDrawBitmapString(reportStream.str(),20,652);
}


//--------------------------------------------------------------
void testApp::exit() {
	//kinect.setCameraTiltAngle(0); // zero the tilt on exit
	kinect.close();
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
        case'&':
            bDoBgSub = !bDoBgSub;
			break;
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
        case '-':
			nearThreshold --;
			if (nearThreshold < 0) nearThreshold = 0;
			break;
            
        case 'o':
			toleranceAngles -=0.05;
			if (toleranceAngles < 0) toleranceAngles = 0;
			break;
            
		case 'p':
			toleranceAngles += 0.05;
			if (toleranceAngles > 2) toleranceAngles = 2;
			break;
            
		case 'a':
			lineThreshold --;
			if (lineThreshold < 0) lineThreshold = 0;
			break;
            
		case 'z':
			lineThreshold ++;
			if (lineThreshold > 255) lineThreshold = 255;
			break;

		case 'w':
			kinect.enableDepthNearValueWhite(!kinect.isDepthNearValueWhite());
			break;
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
}

//--------------------------------------------------------------
void testApp::mouseDragged(int x, int y, int button)
{}

//--------------------------------------------------------------
void testApp::mousePressed(int x, int y, int button)
{
    if (x < 10) x = 10;
    if (x > 410) x = 410;
    if (y < 320) y = 320;
    if (y > 620) y = 620;
    int i;
    for (i = 0; blackRectPoints[i].x != -1; i++);
    if (i < 19){
        blackRectPoints[i] = cvPoint((x-10) * 640 / 400 ,(y-320) * 640 / 400 );
    }
}

//--------------------------------------------------------------
void testApp::mouseReleased(int x, int y, int button)
{
    if (x < 10) x = 10;
    if (x > 410) x = 410;
    if (y < 320) y = 320;
    if (y > 620) y = 620;
    int i;
    for (i = 0; blackRectPoints[i].x != -1; i++);
    if (i < 20){
        blackRectPoints[i] = cvPoint((x-10)* 640 / 400 ,(y-320)* 640 / 400 );
    }
}

//--------------------------------------------------------------
void testApp::windowResized(int w, int h)
{}

