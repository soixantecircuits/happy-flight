#pragma once

#include "ofMain.h"
#include "ofxOpenCv.h"
#include "ofxKinect.h"
#include "ofxOsc.h"

#define HOST "192.168.1.101"
#define PORT 12345

class testApp : public ofBaseApp {
	public:

		void setup();
		void update();
		void draw();
		void exit();

		void keyPressed  (int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void windowResized(int w, int h);
		


        ofxOscSender        sender;
		ofxKinect 			kinect;
		

		ofxCvColorImage		colorImg;
        ofxCvColorImage		colorImgLine;
        ofxCvGrayscaleImage cannyImage;
		ofxCvGrayscaleImage grayImage;			// grayscale depth image
        ofxCvGrayscaleImage grayImageWithoutBackground;			// grayscale depth image
        ofxCvGrayscaleImage grayImageWithoutBackgroundAndBlackRects;
        ofxCvGrayscaleImage memoryGrayImage;			// grayscale depth image
    
        ofxCvGrayscaleImage grayThreshNear;		// the near thresholded image
		ofxCvGrayscaleImage grayThreshFar;		// the far thresholded image

		ofxCvContourFinder 	contourFinder;
        bool                bLearnBakground;  
		
		bool				bThreshWithOpenCV;
		bool				bDrawPointCloud;
        bool                bDoBgSub;

		int 				nearThreshold;
		int					farThreshold;
        int                 lineThreshold; 
        float               smoothAngles;
        float               toleranceAngles;
        float               totalAngles;
        float               avgAngles;
        float               leftAngles;
        float               rightAngles;
    
        CvSeq*              lines;
        CvMemStorage*       storage;
        CvPoint*            blackRectPoints;

		int					angle;
};
