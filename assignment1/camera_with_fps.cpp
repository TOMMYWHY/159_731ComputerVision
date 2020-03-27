#include <stdio.h>
#include <chrono>
#include <ctime>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

using namespace cv;
using namespace std;
using namespace chrono;

/*********************************************************************************************
 * compile with:
 * g++ -std=c++11 camera_with_fps.cpp -o camera_with_fps `pkg-config --cflags --libs opencv`
*********************************************************************************************/

Mat frame;//, image;
int main(int argc, char** argv)
{
  VideoCapture cap;
  cap.open(0);
  if (!cap.isOpened())
     {
         cout << "Failed to open camera" << endl;
         return 0;
     }
   cout << "Opened camera" << endl;
   namedWindow("WebCam", 1);
   cap.set(CV_CAP_PROP_FRAME_WIDTH, 640);
//   cap.set(CV_CAP_PROP_FRAME_WIDTH, 960);
//   cap.set(CV_CAP_PROP_FRAME_WIDTH, 1600);
   cap.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
//   cap.set(CV_CAP_PROP_FRAME_HEIGHT, 720);
//   cap.set(CV_CAP_PROP_FRAME_HEIGHT, 1200);
   cap >> frame;
   printf("frame size %d %d \n",frame.rows, frame.cols);
   int key=0;

   double fps=0.0;
   while (1){
     system_clock::time_point start = system_clock::now();
     //for(int a=0;a<10;a++){
       cap >> frame;
       if( frame.empty() )
       break;

       char printit[100];
       sprintf(printit,"%2.1f",fps);
       putText(frame, printit, cvPoint(10,30), FONT_HERSHEY_PLAIN, 2, cvScalar(255,255,255), 2, 8);
       imshow("WebCam", frame);
       key=waitKey(1);
       if(key==113 || key==27) return 0;//either esc or 'q'
       
     //}
     system_clock::time_point end = system_clock::now();
     double seconds = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
     //fps = 1000000*10.0/seconds;
     fps = 1000000/seconds;
     cout << "frames " << fps << " seconds " << seconds << endl;
   }
}
