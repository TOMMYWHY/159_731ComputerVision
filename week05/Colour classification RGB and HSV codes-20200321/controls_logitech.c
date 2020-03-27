////////////supported logitech controls over opencv 1.1
///////cvSetCaptureProperty(capture,CV_CAP_PROP_POS_MSEC , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_POS_FRAMES , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_POS_AVI_RATIO , 0.4);//not supported
//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_WIDTH, 1200);// why size doesn't work??? It works on opencv1.1.0!!
//cvSetCaptureProperty(capture, CV_CAP_PROP_FRAME_HEIGHT, 1000);// why size doesn't work???It works on opencv1.1.0!!
///////cvSetCaptureProperty(capture,CV_CAP_PROP_FPS , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_FOURCC , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_FRAME_COUNT , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_FORMAT , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_MODE , 0.4);//not supported
//cvSetCaptureProperty(capture, CV_CAP_PROP_BRIGHTNESS, 0.55);//works
//cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST, 0.25);//works
//cvSetCaptureProperty(capture, CV_CAP_PROP_SATURATION, 0.2);//works
///////cvSetCaptureProperty(capture,CV_CAP_PROP_HUE , 0.4);//not supported
///////cvSetCaptureProperty(capture,CV_CAP_PROP_GAIN , 0.4);//not supported
///////cvSetCaptureProperty(capture, CV_CAP_PROP_CONVERT_RGB, 0.4);//not supported
CvFont font;
#define SCRIPT_WIDTH 250
#define SCRIPT_HEIGHT 150

#define INITIAL_BRIGHTNESS 167
#define INITIAL_CONTRAST 44
#define INITIAL_SATURATION 56


////Brightness controls
int brightness=INITIAL_BRIGHTNESS;
float setbrightness=INITIAL_BRIGHTNESS/255.0;
float curbrightness=INITIAL_BRIGHTNESS/255.0;
void on_trackbar_CV_CAP_PROP_BRIGHTNESS( int brightness)
{
setbrightness=brightness/255.0;
}

//cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST, 0.25);//works
int contrast=INITIAL_CONTRAST;
float setcontrast=INITIAL_CONTRAST/255.0;
float curcontrast=INITIAL_CONTRAST/255.0;
void on_trackbar_CV_CAP_PROP_CONTRAST( int contrast)
{
setcontrast=contrast/255.00;
}

//cvSetCaptureProperty(capture, CV_CAP_PROP_SATURATION, 0.2);//works
int saturation=INITIAL_SATURATION;
float setsaturation=INITIAL_SATURATION/255.0;
float cursaturation=INITIAL_SATURATION/255.0;
void on_trackbar_CV_CAP_PROP_SATURATION( int saturation)
{
setsaturation=saturation/255.00;
}


void startup_controls(CvCapture * capture, int cameranumber){
//IplImage * scripting;
char windowname[100];
strcat(windowname,"Controls");
sprintf(windowname,"%s%d\n",windowname,cameranumber);
printf("%s \n",windowname);
cvNamedWindow(windowname, 1 );

//brightness
cvCreateTrackbar( "Brightness", windowname, &brightness, 255, on_trackbar_CV_CAP_PROP_BRIGHTNESS);
cvSetTrackbarPos("Brightness",windowname,INITIAL_BRIGHTNESS);
cvSetCaptureProperty(capture, CV_CAP_PROP_BRIGHTNESS,setbrightness);

//contrast
cvCreateTrackbar( "Contrast", windowname, &contrast, 255, on_trackbar_CV_CAP_PROP_CONTRAST);
cvSetTrackbarPos("Contrast",windowname,INITIAL_SATURATION);
cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST,setcontrast);

//saturation
cvCreateTrackbar( "Saturation", windowname, &saturation, 255, on_trackbar_CV_CAP_PROP_SATURATION);
cvSetTrackbarPos("Saturation",windowname,INITIAL_SATURATION);
cvSetCaptureProperty(capture, CV_CAP_PROP_SATURATION,setsaturation);



printf("_FRAME_WIDTH %f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH));
printf("_FRAME_HEIGHT %f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT));
//write initial values on image
char parm1[100];char parm2[100];
IplImage* scripting=cvCreateImage( cvSize(SCRIPT_WIDTH,SCRIPT_HEIGHT),8,3);
cvInitFont( &font, CV_FONT_HERSHEY_COMPLEX, 0.5, 0.5, 0.0, 1, CV_AA);//CvFont* font,int fontface,double hscale, double vscale, double shear, int thick, int line type);
sprintf(parm1,"Width %d",cvRound(cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_WIDTH)));
sprintf(parm2,"Height %d",cvRound(cvGetCaptureProperty(capture,CV_CAP_PROP_FRAME_HEIGHT)));
cvPutText( scripting, parm1 , cvPoint(0,50), &font, CV_RGB(255,255,0));
cvPutText( scripting, parm2 , cvPoint(0,100), &font, CV_RGB(255,255,0));
cvShowImage( windowname, scripting);

}

void control_capture(CvCapture * capture){
if (setbrightness!=curbrightness){
		cvSetCaptureProperty(capture, CV_CAP_PROP_BRIGHTNESS,setbrightness);
		printf("BRIGHTNESS %f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_BRIGHTNESS));//10
		curbrightness=setbrightness;
	    }
if (setcontrast!=curcontrast){
		cvSetCaptureProperty(capture, CV_CAP_PROP_CONTRAST,setcontrast);
		printf("CONSTRAST %f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_CONTRAST));//10
		curcontrast=setcontrast;
	    }
if (setsaturation!=cursaturation){
		cvSetCaptureProperty(capture, CV_CAP_PROP_SATURATION,setsaturation);
		printf("SATURATION %f \n",cvGetCaptureProperty(capture,CV_CAP_PROP_SATURATION));//10
		cursaturation=setsaturation;
	    }
}
