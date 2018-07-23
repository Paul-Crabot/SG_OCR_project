/*#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
*/

#include<opencv2\core\core.hpp>
#include<opencv2\highgui\highgui.hpp>
#include<opencv2\imgproc\imgproc.hpp>
#include<iostream>
#include<fstream>
#include<map>
#include<algorithm> 


#define _USE_MATH_DEFINES
#include<math.h>


using namespace std;
using namespace cv;


#pragma once
class WhiteRunDetection
{

private:
	Mat src; 
	Mat base_img; 
	vector<float> horizontalWhiteRun;
	vector<float> verticalWhiteRun;

	vector<vector<float>> verticalWhiteRunHisto;
	vector<vector<float>> horizontalWhiteRunHisto;
	vector<vector<float>> classeA;
	vector<vector<float>> classeB;

	vector<Point> verticalWhiteRunPos;
	vector<Point> horizontalWhiteRunPos;


	vector<vector<Point>> cluster;
	vector<vector<float>> clusterLength;
	vector<vector<Point>> mostReleventClusters;


	vector<float> straightLine;
	vector<float> angles;


	float interVert, popVert;
	float interHori, popHori;
	float interLineSpacingVert;
	float interLineSpacingHori;
	int run; 
	float angle; 
	float median; 
	bool rotationInv = false;


public:
	WhiteRunDetection(Mat img);
	~WhiteRunDetection();

	void computeWhiteRuns(); 

	void computeVerticalWhiteRun();
	void computeHorizontalWhiteRun();


	vector<vector<float>> computeNumberOfWhiteRun(vector<float> whiteRun);

	void founInterlineSpacing(); 
	void choseVertOrHori(); 


	float detectSecondPeak(vector<vector<float>> WhiteRunHisto);
	
	void computeVerticalWhiteRunSavePos(int length);
	void computeHorizontalWhiteRunSavePos(int length);

	void buildCluster(vector<Point> whiteRun, int length);
	bool rotationDirection(vector<vector<Point>> mergeClusters);
	void choseMostReleventCluster();

	void computeClusterLength();
	void straightLineEstimation();


	float foundMedian();
	float foundMedianVectFloat(vector<vector<float>> vect);
	int euclidianDistance(Point p1, Point p2);
	static bool sortingFunc(const vector<float>& vec1, const vector<float>& vec2);
	static bool sortingFuncInv(const vector<float>& vec1, const vector<float>& vec2);
	vector<float> getHorizontalWhiteRun();
	vector<float> getVerticalWhiteRun();

	vector<vector<float>>& getVerticalWhiteRunHisto();
	vector<vector<float>>& getHorizontalWhiteRunHisto();
	
	void constructClass(vector<vector<float>> whiteRunHisto);

	vector<Point> getVerticalWhiteRunPos();
	vector<Point> getHorizontalWhiteRunPos();


	vector<vector<Point>>& getCluster();
	vector<vector<float>>& getClusterLength();
	vector<vector<Point>>& getMostReleventClusters();
	void correctSkew(float angle, Mat& src);
	void eraseBorder(Mat& src, float angle, Mat temp);
	void clearAll(); 

	vector<float>& getStraightLine();

	Mat& getSrc();
	Mat& getBase_img();
	float getPopVert();
	float getInterVert();
	float getPopHori();
	float getInterHori();
	float getInterLineSpacingVert(); 
	float getInterLineSpacingHori(); 
	vector<float>& getAngles();
	float getAngle();
	float getMedian(); 
	int getRun();
	bool getRotationInv();

	void setInterLineSpacingVert(float value);
	void setInterLineSpacingHori(float value);
	void setRotationInv(bool value);
	void setPopVert(float value);
	void setInterVert(float value);
	void setPopHori(float value);
	void setInterHori(float value);
	void setMedian(float value); 
	void setAngle(float value);
	void setAngles(float value); 
	void incrementRun(); 

	void setVerticalWhiteRunHisto(vector<vector<float>> vect);
	void setHorizontalWhiteRunHisto(vector<vector<float>> vect);


	vector<vector<float>>& getClasseA();
	vector<vector<float>>& getClasseB();


};

