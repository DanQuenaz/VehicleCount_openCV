#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace cv;
using namespace std;

void sub(Mat img1, Mat img2, Mat &dst){
	dst=img1.clone();
	for (int y=0; y < img1.rows; y++){
    	for (int x=0; x < img1.cols; x++) {
			uchar intensity_1 = img1.at<uchar>(y, x);
			uchar intensity_2 = img2.at<uchar>(y, x);
			uchar value = (int)abs(intensity_1 - intensity_2);
			dst.at<uchar>(y,x) = value;
		}
	}
}

int main(int argc, char** argv )
{
    vector<String> fn; 
	String path = "images/*.jpeg";
	glob(path,fn,false);
    float y_top=0.23, y_bot=0.26;
    char count_txt[6], bt='1';
    bool modo=true;
    int erosion_size=2, dilation_size=5, count_car=0;
    Mat backg, orig, img_sub_moda, img_binary, img_tretada, img_aux;
   	Mat erode_element = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*erosion_size + 1, 2*erosion_size+1 ),
                                       Point( erosion_size, erosion_size ) );
   	Mat dilation_element = getStructuringElement( MORPH_ELLIPSE,
                                       Size( 2*dilation_size + 1, 2*dilation_size+1 ),
                                       Point( dilation_size, dilation_size ) );

    backg = imread("images/image-031.jpeg", IMREAD_GRAYSCALE);
    blur(backg,backg,Size(3,3));
	namedWindow("video", WINDOW_AUTOSIZE );
    for(int i=0; i<fn.size() and bt!='0' ;i++){
	    orig = imread(fn[i], IMREAD_GRAYSCALE);    
		blur(orig,orig,Size(3,3));
	    sub(backg, orig, img_sub_moda);
	    threshold(img_sub_moda,img_binary, 26, 255, THRESH_BINARY);
	   	img_tretada=img_binary.clone();
	  	erode(img_tretada, img_tretada, erode_element );
	    dilate(img_tretada, img_tretada, dilation_element);
	     
	    SimpleBlobDetector::Params params;
		params.minDistBetweenBlobs = 25.0f;
		params.filterByInertia = false;
		params.filterByConvexity = false;
		params.filterByColor = false;
		params.filterByCircularity = false;
		params.filterByArea = true;
		params.minArea = 600.0f;
		params.maxArea = (float)1e9;
		Ptr<SimpleBlobDetector> detector = SimpleBlobDetector::create(params);
		std::vector<KeyPoint> keypoints;
		detector->detect( img_tretada, keypoints);
		for(int j=0; j<keypoints.size(); j++){
			if(keypoints[j].pt.y>(int)((double)orig.rows*y_top) && 
				keypoints[j].pt.y<(int)((double)orig.rows*y_bot)){
				count_car++;
			}
			else if(keypoints[j].pt.y<(int)((double)orig.rows*y_top)){
				keypoints.erase(keypoints.begin()+j);
				j--;
			}
		}
		sprintf(count_txt,"%d", count_car);
		Mat im_with_keypoints;
		drawKeypoints((modo)?(orig):(img_tretada), keypoints, im_with_keypoints, Scalar(0,0,255), 4);
		drawKeypoints( im_with_keypoints, keypoints, im_with_keypoints, Scalar(0,255,255), 0);
		line(im_with_keypoints, Point(0.0f,(float)orig.rows*y_top), Point((float)orig.cols-1,(float)orig.rows*y_top), Scalar(0,255,0),2);
		line(im_with_keypoints, Point(0.0f,(float)orig.rows*y_bot), Point((float)orig.cols-1,(float)orig.rows*y_bot), Scalar(0,255,0),2);
		putText(im_with_keypoints, count_txt, Point(40.0f,30.0f), FONT_HERSHEY_PLAIN, 2.5, Scalar(40,60,230), 2);
		imshow("video", im_with_keypoints );
	    bt = waitKey(200);
	    if(bt=='r'){i=0;count_car=0;}
	    if(bt=='t'){modo=!modo;}
	    if(bt=='p'){
	    	bt='1';
		    while(bt!='p'){
		    	bt = waitKey(200);
		    }
		}
	}
	

    return 0;
}

