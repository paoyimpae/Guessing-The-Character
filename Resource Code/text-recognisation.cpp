#include <stdio.h>
#include <leptonica/allheaders.h>
#include <tesseract/capi.h>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
// #include <unistd.h>
#include <iostream>

using namespace std;

void check(const char *errstr) {
	fputs(errstr, stderr);
	exit(1);
}

int main(int argc, char *argv[]) {
	TessBaseAPI *handle;
	PIX *img;
	char *text;

	handle = TessBaseAPICreate();
	if(TessBaseAPIInit3(handle, NULL, "eng") != 0)
	    check("Error initialising tesseract\n");
    
    //Open Camera
    cv::VideoCapture cap(0);
    int count = 0;
    while(1){
        cv::Mat frame;
        cap >> frame;

        //convert to grayscale
        // cv::Mat gray;
        // cv::cvtColor(frame, gray, CV_RGB2GRAY);
        cv::imwrite("gray.png", frame); //Convert Mat to png
        if(count == 100){
            //check image
            if((img = pixRead("gray.png")) == NULL)
		        check("Error Reading image\n");
        
	        TessBaseAPISetImage2(handle, img);

            //check recognize
        	if(TessBaseAPIRecognize(handle, NULL) != 0)
	        	check("Error in Tesseract recognition\n");

            //check text
	        if((text = TessBaseAPIGetUTF8Text(handle)) == NULL)
		        check("Error Getting text\n");

    	    fputs(text, stdout); //output text

            //Ask user for check result
            char ans;
            cout << "Is it correct? (y/n): ";
            cin >> ans;
            if((ans == 'Y')||(ans == 'y')){
                // cout << "Yeah! I'm win! =D" << endl;
                break;
            }
            count = 0;
        }
        
        imshow("frame", frame); //show camera
        
        count++;
        char c = (char)cv::waitKey(1);
        if( c == 27 )
            break;
        // usleep(10); //delay
    }
    TessDeleteText(text);
	TessBaseAPIEnd(handle);
	TessBaseAPIDelete(handle);
	// pixDestroy(&img);
    cv::destroyAllWindows();
	return 0;
}