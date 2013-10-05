#include <iostream>
#include <cv.h>
#include <highgui.h>
#include <stdio.h>



using namespace std;

CvHaarClassifierCascade *cascade_f; //classifiers to detect eyes and face.
CvHaarClassifierCascade *cascade_e;
CvMemStorage			*storage;

void detectFace(IplImage *img, int p); // three methods to detect eyes, face and a draw aim image over my eyes.

IplImage *resized;
int x_prev = 0;
int y_prev = 0;
int primary = 1;


int main( int argc, char** argv )
{

	IplImage *img;
	CvCapture *capture;
	char *file1 = "classifiers/haarcascade_frontalface_alt.xml"; //classifiers in a .xml file
	char *file2 = "classifiers/haarcascade_eye.xml";
	int key = 1;
	capture = cvCaptureFromCAM(0); // capture from webcam, inbuilt or via USB.

	// load the face classifier 
	cascade_f = (CvHaarClassifierCascade*)cvLoad(file1, 0, 0, 0);

	// load the eye classifier 
	cascade_e = (CvHaarClassifierCascade*)cvLoad(file2, 0, 0, 0);

	// setup memory storage, needed by the object detector 
	storage = cvCreateMemStorage(0);

	// check if files loaded poperly and memory is created
	assert(cascade_f);
	assert(cascade_e);
	assert(storage);

	cvNamedWindow("w");

	// detect eyes and display image

	while( key != 'q' ) {  //while the q key is not pressed...

		img = cvQueryFrame( capture ); //get a frame from capture, which stores data from the webcam
		if( !img ) break; //if there is no image, stop

		detectFace(img, primary); //method to detect eyes, see below
		cvShowImage("w", img); //show modified image with fire on top of eyes, see below

		key = cvWaitKey(1); //wait for key press to continue, but if the parameter is positive, continue anyway. if q is pressed, program exits.
	}

	cvWaitKey(0); //wait for key press
	cvDestroyWindow("w"); //close windoes and release resoures
	cvReleaseImage(&img); //release memory used by the image

	return 0; 
}

void detectFace(IplImage *img, int p) //method to detect eyes.
{
	

	CvSeq *faces = cvHaarDetectObjects(  //using the face classifier, first detect the face.
		img, cascade_f, storage, 
		2.0, 3, 0, cvSize( 50,50 ) );

	//return if not found 
	if (faces->total == 0) return;



	// draw a rectangle on each face
	CvRect *r = (CvRect*)cvGetSeqElem(faces, 0);
	CvRect *g = (CvRect*)cvGetSeqElem(faces, 0);
	CvRect *e = (CvRect*)cvGetSeqElem(faces, 0);
	cvRectangle(img,
		cvPoint(e->x, e->y),
		cvPoint(e->x + e->width, e->y + e->height),
		CV_RGB(255, 0, 0), 1, 8, 0);

	if (p == 1){
		x_prev = e->x;
		y_prev = e->y;
		p == 0;
	}

	if(x_prev - e->x > 10){
		cout << "Face tilted left\n";
		x_prev = e->x;
	}
	if(e->x - x_prev > 10){
		cout << "Face tilted right\n";
		x_prev = e->x;
	}
	if(y_prev - e->y > 10){
		cout << "Face tilted down\n";
		y_prev = e->y;
	}
	if(e->y - y_prev > 10){
		cout << "Face tilted up\n";
		y_prev = e->y;
	}


	//reset the memory buffer for the eyes now.
	cvClearMemStorage(storage);

	cvResetImageROI(img); //reset the region of interest.


}



