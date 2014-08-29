// detector.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <stdio.h>
#include <iostream>

#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"

using namespace std;

int main (int argc, char *argv[])
{
	printf("running\n");
	const char *imgfile = "test.jpg";
	cv::Mat img = cv::imread(imgfile, CV_LOAD_IMAGE_COLOR);
	if(img.empty()){
		printf("load failed\n");
		return -1;
	}
//	std::string unitbanFileName = "../github/detector/unitban.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
//	std::string unitretsuFileName = "../github/detector/unitretsu.xml";
	std::string numFileName = "../github/detector/num1.xml";
	std::string unitFileName = "../github/detector/num3.xml";
	cout << numFileName << endl;
	cv::CascadeClassifier numCascade;
	cv::CascadeClassifier unitCascade;
	if(numCascade.load(numFileName)) printf("num loaded\n");
	if(unitCascade.load(unitFileName)) printf("unit loaded\n");
	printf("1\n");

	std::vector<cv::Rect> numbers;
	std::vector<cv::Rect> units;
	unsigned int i;
	printf("load finished\n");


	/*検出*/
	numCascade.detectMultiScale(img, numbers,1.1,2,CV_HAAR_SCALE_IMAGE);
	unitCascade.detectMultiScale(img, numbers, 1.1, 2, CV_HAAR_SCALE_IMAGE);

	printf("numbers: %d\n",numbers.size());
	printf("units: %d\n",units.size());

	int parents[1000];
	for (unsigned int j=0;j<numbers.size();j++) {
		parents[j]=-1;
	}
	parents[numbers.size()]=numbers.size();

	for (i=0;i<units.size();i++)
	{
		//深さ優先で木を生成
		int node=numbers.size();
		CvRect root_rect = units.at(i);

		while (-1) {
			int origNode = node;
			CvRect parent_rect;
			if (node == numbers.size()) {
				parent_rect = units.at(i);
			} else {
				parent_rect = numbers.at(node);
			}

			for (unsigned int j=0;j<numbers.size();j++)
			{
				if (parents[j]==-1) {
					CvRect sub_rect = numbers.at(j);
					if (root_rect.height*1.5>sub_rect.height && sub_rect.height*1.5>root_rect.height && ((parent_rect.x-sub_rect.x)*(parent_rect.x-sub_rect.x-sub_rect.width)<=0 || (parent_rect.x-sub_rect.x)*(parent_rect.x+parent_rect.width-sub_rect.x)<=0) && ((parent_rect.y-sub_rect.y)*(parent_rect.y-sub_rect.y-sub_rect.height)<=0 || (parent_rect.y-sub_rect.y)*(parent_rect.y+parent_rect.height-sub_rect.y)<=0))
					{
						parents[j] = node;
						node = j;
						break;
					}
				}
			}
			if (node == origNode) {
				if (node == numbers.size()) {
					break;
				}
				node = parents[node];
			}
		}
	}

	cv::Mat img_clone = img.clone();
	
/*	for (i=0;i<numbers->total;i++) {
		if (parents[i]!=-1) {
			//モザイク処理
			CvRect rect = *(CvRect *)cvGetSeqElem(numbers, i);
			cv::Mat roi = img(rect);
			cv::Mat roi2 = img_clone(rect);
//			cv::medianBlur(roi2,roi,rect.width/4*2+1);
			cv::GaussianBlur(roi2, roi, cv::Size(rect.width/4*2+1,rect.height/4*2+1), rect.width/4, rect.height/4);
		}
	}/**/

		/*領域描画*/
	for (i=0;i<numbers.size(); i++)
	{
		cv::Rect number_rect = numbers.at(i);
		cv::rectangle(img, number_rect, CV_RGB(255,0,0),5);
	}
	for (i=0;i<units.size(); i++)
	{
		cv::Rect unit_rect = units.at(i);
		cv::rectangle(img, unit_rect, CV_RGB(0,0,255),5);
	}
	int mag = img.rows/500;
	cv::Mat img2(img.rows/mag, img.cols/mag, CV_8UC1);
	cv::resize(img, img2, img2.size(), 0, 0, cv::INTER_LINEAR);
	printf("mag: %d\n",mag);

	cv::namedWindow("test", CV_WINDOW_AUTOSIZE);
	cv::imshow("test", img2);
	cv::waitKey(0);
	cv::destroyWindow("test");
	
	return 0;
}
