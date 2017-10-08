#include <stdio.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <algorithm>
#include <vector>
#include <math.h>
using namespace cv;

#define times(l_i_mI_te, block) for(int i=0; i <l_i_mI_te; i++){block;}
struct Ponto{
	int x,y;
	Ponto(int X, int Y){
		x=X; y=Y;
	}

};

Mat sub(Mat img1, Mat img2){
	Mat img_res = img1.clone();
	for (int y=0; y < img1.rows; y++){
    	for (int x=0; x < img1.cols; x++) {
			uchar intensity_1 = img1.at<uchar>(y, x);
			uchar intensity_2 = img2.at<uchar>(y, x);
			uchar value = (int)abs(intensity_1 - intensity_2);
			//uchar green = (uchar)intensity.val[1];
			//uchar red = (uchar)intensity.val[2];
			//uchar red = 0;
			img_res.at<uchar>(y,x) = value;
		}
	}
	return img_res;
}

Mat threshold(Mat img, int lim, int dim=0){
	Mat img_res = img.clone();
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
    		double sum =0;
    		int count= 0;
			for (int j=-dim; j < dim+1; j++){
    			for (int i=-dim; i < dim+1; i++) {
    				if(x+j>=0 && x+j<(img.cols) && y+i>=0 && y+i<(img.rows)){	
						uchar intensity = img.at<uchar>(y+i, x+j);
						sum+=(uchar)intensity;
						count++;
    				}
				}
			}
			uchar value_m = sum/(double)count, value=img.at<uchar>(y,x);
			if(value_m <= lim){
				value = 0;
			}
			else{
				value = 255;
			}
			//uchar green = (uchar)intensity.val[1];
			//uchar red = (uchar)intensity.val[2];
			//uchar red = 0;
			img_res.at<uchar>(y,x) = value;
		}
	}
	return img_res;
}

void normalize(Mat &img, int lim){
	for(int y=0; y< img.rows; ++y){
		for(int x=0; x<img.cols; ++x){
			uchar intensity = img.at<uchar>(y, x);
			uchar value = (uchar)intensity;
			if(value <= lim) value = 255;
			img.at<uchar>(y, x) = value;
		}
	}
}

void negative(Mat &img){
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
			uchar intensity = img.at<uchar>(y, x);
			uchar value = 255-(uchar)intensity;
			//uchar green = (uchar)intensity.val[1];
			//uchar red = (uchar)intensity.val[2];
			//uchar red = 0;
			img.at<uchar>(y,x) = value;
		}
	}
}
Mat moda(Mat img, int dim = 1){
	Mat img_res = img.clone();
	std::vector<int> intesidades; 
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
    		intesidades.clear();
			for (int j=-dim; j < 1+dim; j++){
    			for (int i=-dim; i < 1+dim; i++) {
    				if(x+j>=0 && x+j<(img.cols) && y+i>=0 && y+i<(img.rows)){	
						uchar intensity = img.at<uchar>(y+i, x+j);
						intesidades.push_back((uchar)intensity);
    				}
				}
			}
			std::sort(intesidades.begin(), intesidades.begin()+intesidades.size());
			uchar value = intesidades[intesidades.size()/2];
			img_res.at<uchar>(y,x) = value;
		}
	}
	return img_res;
}

int hyp(int a, int b){
	return ceil(pow(pow((double)a,2.0)+pow((double)b,2.0),0.5));
}

Mat erosao(Mat img, int dim = 1){
	Mat img_res = img.clone();
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
			uchar min = 255; 
			for (int j=-dim; j < 1+dim; j++){
    			for (int i=-dim; i < 1+dim; i++) {
    				if(x+j>=0 && x+j<(img.cols) && y+i>=0 && y+i<(img.rows)){	
						uchar intensity = img.at<uchar>(y+i, x+j);
						if((uchar)intensity < min && hyp(j,i)<dim)min = (uchar)intensity;
    				}
				}
			}
			img_res.at<uchar>(y,x) = min;
		}
	}
	return img_res;
}

Mat dilatacao(Mat img, int dim = 1){
	Mat img_res = img.clone();
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
			uchar max = 0; 
			for (int j=-dim; j < 1+dim; j++){
    			for (int i=-dim; i < 1+dim; i++) {
    				if(x+j>=0 && x+j<(img.cols) && y+i>=0 && y+i<(img.rows)){	
						uchar intensity = img.at<uchar>(y+i, x+j);
						if((uchar)intensity > max && hyp(j,i)<dim)max = (uchar)intensity;
    				}
				}
			}
			img_res.at<uchar>(y,x) = max;
		}
	}
	return img_res;
}


Mat media(Mat img){
	Mat img_res = img.clone();
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
    		double sum =0;
    		int count= 0;
			for (int j=-1; j < 2; j++){
    			for (int i=-1; i < 2; i++) {
    				if(x+j>=0 && x+j<(img.cols) && y+i>=0 && y+i<(img.rows)){	
						uchar intensity = img.at<uchar>(y+i, x+j);
						sum+=(uchar)intensity;
						count++;
    				}
				}
			}
			uchar value = sum/(double)count;
			img_res.at<uchar>(y,x) = value;
		}
	}
	return img_res;
}

void findCars(Mat img){
	int **img_ids=new int*[img.cols];
	times(img.rows,img_ids[i]=new int[img.rows]);
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
    		img_ids[x][y]=0;
		}
	}	
	int id=1;
	for (int y=0; y < img.rows; y++){
    	for (int x=0; x < img.cols; x++) {
    		if(img.at<uchar>(y,x)==255 && img_ids[x][y]==0){
    			std::vector<Ponto> pixels;
    			bool disponiveis=true;
    			int iter=-1;
    			while(disponiveis){
	    			disponiveis=true;
	    			if(iter>-1){
	    				x=pixels[iter].x;
	    				y=pixels[iter].y;
	    			}
	    			for (int j=-1; j < 2; j++){
	    			for (int i=-1; i < 2; i++) {
	    				bool novo=true;
	    				for(int e=0; e<pixels.size(); e++){
	    					if(pixels[e].x==x+j && pixels[e].y==y+i)
	    						novo=false;
	    				}
	    				if(img.at<uchar>(y+i,x+j)==255 && novo){
	    					pixels.push_back(Ponto(x+j,y+i));
	    				}
	    			}}	
	    			disponiveis=(iter<pixels.size());
	    			iter++;
    			}
				for(int e=0; e<pixels.size(); e++){
					img_ids[pixels[e].x][pixels[e].y]=id;
				}
    			y=x=-1;
    			id++;
    			printf("%d",id);
    		}
		}
	}
}

int main(int argc, char** argv )
{
    if ( argc != 3 )
    {
        printf("usage: DisplayImage.out <Image_Path>\n");
        return -1;
    }

    Mat image;
    Mat backg, orig, img_sub_moda, img_binary, img_tretada;
    //image = imread( argv[1], 1 );
    backg = imread(argv[1], IMREAD_GRAYSCALE);
    orig = imread(argv[2], IMREAD_GRAYSCALE);
    img_sub_moda = orig.clone();
    img_binary = orig.clone();
    img_tretada = orig.clone();
    if ( !backg.data || !orig.data )
    {
        printf("No image data \n");
        return -1;
    }

    orig=media(orig);
    backg=media(backg);
    img_sub_moda = sub(backg, orig);
    img_binary = threshold(img_sub_moda, 15,4);
   	img_tretada=img_binary.clone();
   	img_tretada = erosao(img_tretada, 2);
    img_tretada = dilatacao(img_tretada, 5);
    //Mat final_grey;
    //cvtColor(img_tretada, final_grey, cv::COLOR_BGR2GRAY);
     
    SimpleBlobDetector::Params params;
	params.minDistBetweenBlobs = 10.0f;
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
	printf("%d\n",(int)keypoints.size());
	Mat im_with_keypoints;
	drawKeypoints( img_tretada, keypoints, im_with_keypoints, Scalar(0,0,255), DrawMatchesFlags::DEFAULT );

	imshow("keypoints", im_with_keypoints );
    

	//namedWindow("Display Processed Image1", WINDOW_AUTOSIZE );
    //imshow("Display Processed Image1", img_sub_moda);    
	//namedWindow("Display Processed Image2", WINDOW_AUTOSIZE );
    //imshow("Display Processed Image2", img_binary);     
	//namedWindow("Display Processed Image3", WINDOW_AUTOSIZE );
    //imshow("Display Processed Image3", img_tretada); 

    waitKey(0);

    return 0;
}

