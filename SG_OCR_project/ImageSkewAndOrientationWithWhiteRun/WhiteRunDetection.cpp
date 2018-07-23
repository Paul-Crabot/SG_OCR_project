#include "WhiteRunDetection.h"



WhiteRunDetection::WhiteRunDetection(Mat img)
{
	img.copyTo(src);
	img.copyTo(base_img); 
	run = 1; 
}


WhiteRunDetection::~WhiteRunDetection()
{
}


void WhiteRunDetection::computeWhiteRuns() {

	computeVerticalWhiteRun();
	computeHorizontalWhiteRun();


	setVerticalWhiteRunHisto(computeNumberOfWhiteRun(getVerticalWhiteRun()));
	setHorizontalWhiteRunHisto(computeNumberOfWhiteRun(getHorizontalWhiteRun()));

}

void WhiteRunDetection::computeVerticalWhiteRun() {

	
	Point start;
	Point end;
	bool compute = false;
	bool save = false;
	int count = 0;
	for (int i = 0; i < src.cols; i++) {


		for (int j = 1; j < src.rows; j++) {

			if (src.at<uchar>(j - 1, i) == 0 && src.at<uchar>(j, i) == 255) {
				start.x = i;
				start.y = j;
				compute = true;
			}
			if (src.at<uchar>(j, i) == 0 && count != 0) {
				end.x = i;
				end.y = j - 1;
				compute = false;
				save = true;
			}
			if (compute)count++;
			if (save) {
				if (euclidianDistance(start, end) != 0) verticalWhiteRun.push_back(euclidianDistance(start, end));
				count = 0;
				save = false;
				compute = false;
			}
		}
		count = 0;
		save = false;
		compute = false;

	}


}


void  WhiteRunDetection::computeHorizontalWhiteRun() {

	Point start;
	Point end;
	bool compute = false;
	bool save = false;
	int count = 0;

	for (int i = 0; i < src.rows; i++) {

		for (int j = 1; j < src.cols; j++) {

			if (src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i, j) == 255) {
				start.x = j;
				start.y = i;
				compute = true;
			}
			if (src.at<uchar>(i, j) == 0 && count != 0) {
				end.x = j - 1;
				end.y = i;
				compute = false;
				save = true;
			}
			if (compute)count++;
			if (save) {
				if (euclidianDistance(start, end) != 0) horizontalWhiteRun.push_back(euclidianDistance(start, end)); 
				count = 0;
				save = false;
				compute = false;
			}
		}
		count = 0;
		save = false;
		compute = false;

	}

}

void WhiteRunDetection::founInterlineSpacing() {

	constructClass(verticalWhiteRunHisto);


	interVert = (classeA.at(0).at(1) > classeB.at(0).at(1)) ? classeA.at(0).at(1) : classeB.at(0).at(1);
	popVert = (classeA.at(0).at(1) > classeB.at(0).at(1)) ? classeA.at(0).at(0) : classeB.at(0).at(0);

	constructClass(horizontalWhiteRunHisto);

	interHori = (classeA.at(0).at(1) > classeB.at(0).at(1)) ? classeA.at(0).at(1) : classeB.at(0).at(1);
	popHori = (classeA.at(0).at(1) > classeB.at(0).at(1)) ? classeA.at(0).at(0) : classeB.at(0).at(0);

}

void  WhiteRunDetection::choseVertOrHori() {
	

	if (popVert >= popHori) {
		interLineSpacingVert = interVert;
		cout << "interline spacing : " << interVert << endl;//pour la présentation
		computeVerticalWhiteRunSavePos(interLineSpacingVert);
		buildCluster(verticalWhiteRunPos, interLineSpacingVert);
	}
	else {
		interLineSpacingHori = interHori;
		computeHorizontalWhiteRunSavePos(interLineSpacingHori);
		buildCluster(horizontalWhiteRunPos, interLineSpacingHori);
	}

}


void  WhiteRunDetection::computeVerticalWhiteRunSavePos(int length) {

	Point start;
	Point end;
	bool compute = false;
	bool save = false;
	int count = 0;

	Mat drawing; //pour la présentation
	src.copyTo(drawing); //pour la présentation
	cvtColor(drawing, drawing, CV_GRAY2BGR); //pour la présentation

	for (int i = 0; i < src.cols; i++) {


		for (int j = 1; j < src.rows; j++) {

			if (src.at<uchar>(j - 1, i) == 0 && src.at<uchar>(j, i) == 255) {
				start.x = i;
				start.y = j;
				compute = true;
			}
			if (src.at<uchar>(j, i) == 0 && count != 0) {
				end.x = i;
				end.y = j - 1;
				compute = false;
				save = true;
			}
			if (compute)count++;
			if (save) {
				if (euclidianDistance(start, end) == length) {
					verticalWhiteRunPos.push_back(start);
					line(drawing, start, end, Scalar(0, 255, 0), 2); //pour la présentation
				}
				count = 0;
				save = false;
				compute = false;

			}


		}
		count = 0;
		save = false;
		compute = false;



	}

	imwrite("./ImageRes/-45/drawInterlineSpacing"+to_string(run)+".jpg",drawing); //pour la présentation


}

void WhiteRunDetection::computeHorizontalWhiteRunSavePos(int length) {

	Point start;
	Point end;
	bool compute = false;
	bool save = false;
	int count = 0;

	for (int i = 0; i < src.rows; i++) {


		for (int j = 1; j < src.cols; j++) {

			if (src.at<uchar>(i, j - 1) == 0 && src.at<uchar>(i, j) == 255) {
				start.x = j;
				start.y = i;
				compute = true;
			}
			if (src.at<uchar>(i, j) == 0 && count != 0) {
				end.x = j - 1;
				end.y = i;
				compute = false;
				save = true;
			}
			if (compute)count++;
			if (save) {
				if (euclidianDistance(start, end) == length) {
					horizontalWhiteRunPos.push_back(start);
				}
				count = 0;
				save = false;
				compute = false;
			}


		}
		count = 0;
		save = false;
		compute = false;



	}

}




int WhiteRunDetection::euclidianDistance(Point p1, Point p2) {

	return  sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2));

}

vector<vector<float>> WhiteRunDetection::computeNumberOfWhiteRun(vector<float> whiteRun) {


	sort(whiteRun.begin(), whiteRun.end());


	vector<float> temp;
	vector<vector<float>> res;
	bool computed = false;



	for (size_t i = 0; i < whiteRun.size() - 1;) {

		temp.push_back(whiteRun[i]);
		temp.push_back(0);


		while (temp[0] == whiteRun[i]) {
			temp[1]++;
			if (i == whiteRun.size() - 1)break;
			else i++;
		}
		res.push_back(temp);
		temp.clear();



	}

	return res;

}



bool WhiteRunDetection::sortingFunc(const vector<float>& vec1, const vector<float>& vec2) {
	for (size_t i = 0; i < vec1.size() && i < vec2.size(); i++) {
		if (vec1[i] > vec2[i]) {
			return true;
		}
		else if (vec1[i] < vec2[i]) {
			return false;
		}
	}
	return false;
}


bool WhiteRunDetection::sortingFuncInv(const vector<float>& vec1, const vector<float>& vec2) {
	for (size_t i = 0; i < vec1.size() && i < vec2.size(); i++) {
		if (vec1[i] > vec2[i]) {
			return false;
		}
		else if (vec1[i] < vec2[i]) {
			return true;
		}
	}
	return true;
}



void WhiteRunDetection::buildCluster(vector<Point> whiteRunPos, int length) {

	int i = 0;
	vector<Point> vecTemp;

	vector<Point>::iterator it = whiteRunPos.begin();
	vector<Point>::iterator it2 = whiteRunPos.begin() + 1;


	for (it; it != whiteRunPos.end();) {

		vecTemp.push_back(*it);

		for (it2; it2 != whiteRunPos.end();) {

			if (abs(it2->x - vecTemp.back().x) < length  && abs(it2->y - vecTemp.back().y) < length) {
				vecTemp.push_back(*it2);
				whiteRunPos.erase(it2);
				if (whiteRunPos.size() > 1 + i)it2 = whiteRunPos.begin() + 1 + i;
				else break;

			}
			else {
				i++;
				it2++;
			}

		}

		if (atan2(vecTemp.at(vecTemp.size() - 1).y - vecTemp.at(0).y, vecTemp.at(vecTemp.size() - 1).x - vecTemp.at(0).x) * 180 / M_PI != 0)cluster.push_back(vecTemp);
		whiteRunPos.erase(it);
		vecTemp.clear();
		if (whiteRunPos.size() > 1) {
			it = whiteRunPos.begin();
			it2 = whiteRunPos.begin() + 1;
		}
		else break;
		i = 0;


	}

}

void WhiteRunDetection::computeClusterLength() {

	int i = 0;
	int length = 0;
	int clusterNumber = 0;
	vector<float> temp;
	vector<vector<Point>>::iterator it;


	sort(cluster.begin(), cluster.end(), [](const vector<Point> &a, const vector<Point> &b) {return a.size() > b.size(); });


	for (it = cluster.begin(); it != cluster.end(); clusterNumber++) {

		length = euclidianDistance(it->at(0), it->at(it->size() - 1));
		if (length != 0) {
			temp.push_back(clusterNumber);
			temp.push_back(length);
			clusterLength.push_back(temp);
			temp.clear();
			i++;
			it++;
		}
		else {
			cluster.erase(it);
			if (cluster.size() > i)it = cluster.begin() + i;
			else break;
		}


	}

}

void WhiteRunDetection::straightLineEstimation() {

	int sumXY = 0;
	int sumSquareX = 0;
	float meanX = 0;
	float meanY = 0;
	float k = 0;
	int i = 0; 
	vector<float> temp;
	vector<vector<Point>>::iterator it;
	vector<Point>::iterator it_bis;
	for (it = mostReleventClusters.begin(); it != mostReleventClusters.end(); it++) {


		for (it_bis = it->begin(); it_bis != it->end(); it_bis++) {

			sumSquareX += pow(it_bis->x, 2);
			sumXY += it_bis->x * it_bis->y;
			meanX += it_bis->x;
			meanY += it_bis->y;

		}
		meanX /= it->size();
		meanY /= it->size();

		k = (sumXY - (it->size()*meanX*meanY)) / (sumSquareX - (it->size() * pow(meanX, 2)));
		straightLine.push_back(k);


		sumXY = 0;
		sumSquareX = 0;
		meanX = 0;
		meanY = 0;
		k = 0;
		temp.clear();

		i++;
	}
	

}

float WhiteRunDetection::foundMedian() {
	float median;

	sort(straightLine.begin(), straightLine.end());


	if (straightLine.size() % 2 == 0) {
		median = straightLine.at((straightLine.size() / 2) - 1) + straightLine.at(straightLine.size() / 2);
		median /= 2;
	}
	else {
		median = straightLine.at((straightLine.size() + 1) / 2 - 1);
	}

	return median;
}

float WhiteRunDetection::foundMedianVectFloat(vector<vector<float>> vect) {

	float median = 0;
	float temp; 

	vector<vector<float>>::iterator it = vect.begin();
	for (it; it != vect.end(); it++) {
		temp = it->at(0); 
		it->at(0) = it->at(1); 
		it->at(1) = temp; 
	}
	
	sort(vect.begin(), vect.end(), sortingFuncInv);

	
	if (vect.size() % 2 == 0) {
		median = vect.at((vect.size() / 2) - 1).at(0) + vect.at(vect.size() / 2).at(0);
		median /= 2;
	}
	else {
		median = vect.at((vect.size() + 1) / 2 - 1).at(0);
	}
	
	return median;



}




bool WhiteRunDetection::rotationDirection(vector<vector<Point>> clusters) {

	int countNegativeAngle = 0;
	int countPositiveAngle = 0;

	vector<vector<Point>>::iterator it = clusters.begin();
	for (it; it != clusters.end(); it++) {
		if (atan2(it->at(it->size() - 1).y - it->at(0).y, it->at(it->size() - 1).x - it->at(0).x) * 180 / M_PI > 0)countPositiveAngle++;
		else countNegativeAngle++;
	}

	if (countNegativeAngle > countPositiveAngle) return true;
	else return false;

}

void WhiteRunDetection::choseMostReleventCluster() {

	
	
	float temp; 
	
	computeClusterLength(); 
	

	vector<vector<float>>::iterator it = clusterLength.begin();
	for (it; it != clusterLength.end(); it++) {
		temp = it->at(0);
		it->at(0) = it->at(1);
		it->at(1) = temp;
	}

	sort(clusterLength.begin(), clusterLength.end(), sortingFunc);
	

	vector<vector<float>>::iterator it9 = clusterLength.begin();
	while (mostReleventClusters.size() < round(((cluster.size() * 10) / 100)) && it9 != clusterLength.end()) {
		if (rotationInv) {

			if (atan2(cluster.at(it9->at(1)).at(cluster.at(it9->at(1)).size() - 1).y - cluster.at(it9->at(1)).at(0).y, cluster.at(it9->at(1)).at(cluster.at(it9->at(1)).size() - 1).x - cluster.at(it9->at(1)).at(0).x) * 180 / M_PI < 0) {
				mostReleventClusters.push_back(cluster.at(it9->at(1)));
			}
			it9++;

		}
		else {

			if (atan2(cluster.at(it9->at(1)).at(cluster.at(it9->at(1)).size() - 1).y - cluster.at(it9->at(1)).at(0).y, cluster.at(it9->at(1)).at(cluster.at(it9->at(1)).size() - 1).x - cluster.at(it9->at(1)).at(0).x) * 180 / M_PI > 0) {
				mostReleventClusters.push_back(cluster.at(it9->at(1)));
			}
			it9++;

		}
	}

}


float WhiteRunDetection::detectSecondPeak(vector<vector<float>> whiteRunHisto) {

	float temp;
	float res;


	vector<vector<float>>::iterator it = whiteRunHisto.begin();
	for (it; it != whiteRunHisto.end(); it++) {
		temp = it->at(0);
		it->at(0) = it->at(1);
		it->at(1) = temp;
	}
	sort(whiteRunHisto.begin(), whiteRunHisto.end(), sortingFunc);


	res = (whiteRunHisto.at(0).at(1) > whiteRunHisto.at(1).at(1)) ? whiteRunHisto.at(0).at(1) : whiteRunHisto.at(1).at(1);


	return res;
}






void  WhiteRunDetection::constructClass(vector<vector<float>> whiteRunHisto) {

	int cpt = 0; 
	float temp;
	float medianClasseA;
	float medianClasseB;
	vector<float> temp2; 
	vector<vector<float>> partClasseA;
	vector<vector<float>> partClasseB;

	classeA.clear(); 
	classeB.clear(); 

	vector<vector<float>>::iterator it = whiteRunHisto.begin();
	for (it; it != whiteRunHisto.end(); it++) {
		temp = it->at(0);
		it->at(0) = it->at(1);
		it->at(1) = temp;
	}
	sort(whiteRunHisto.begin(), whiteRunHisto.end(), sortingFunc);
	

	vector<vector<float>>::iterator it2 = whiteRunHisto.begin();
	temp2.push_back(it2->at(0));
	temp2.push_back(it2->at(1));
	classeA.push_back(temp2);
	temp2.clear();
	it2++; 
	for (it2; it2 != whiteRunHisto.end(); it2++) {
	
		
		if (it2->at(1) >= classeA.at(0).at(1) - 3 && it2->at(1) <= classeA.at(0).at(1) + 3) {
			
			temp2.push_back(it2->at(0));
			temp2.push_back(it2->at(1));
			classeA.push_back(temp2);

		}
		else{
			temp2.push_back(it2->at(0));
			temp2.push_back(it2->at(1));
			classeB.push_back(temp2);
		}
		temp2.clear(); 
	}
	
	if (classeA.size() > 50) {
		vector<vector<float>>::iterator first = classeA.begin();
		vector<vector<float>>::iterator last = classeA.begin() + 5; //5 fisrt elements;
		vector<vector<float>> partClasseA(first, last);
		medianClasseA = foundMedianVectFloat(partClasseA);
	}
	else medianClasseA = foundMedianVectFloat(classeA);


	if (classeB.size() > 50) {
		vector<vector<float>>::iterator first2 = classeB.begin();
		vector<vector<float>>::iterator last2 = classeB.begin() + 5; //5 fisrt elements;
		vector<vector<float>> partClasseB(first2, last2);
		medianClasseB = foundMedianVectFloat(partClasseB);
	}
	else medianClasseB = foundMedianVectFloat(classeB);
	 


	
	vector<vector<float>>::iterator it3 = classeA.begin(); 
	for (it3; it3 != classeA.end();) {
		
		if (abs(it3->at(1) - medianClasseB) < abs(it3->at(1) - medianClasseA)) {
			temp2.push_back(it3->at(0));
			temp2.push_back(it3->at(1));
			classeB.push_back(temp2); 
			classeA.erase(it3);  
			if(classeA.size() > cpt)it3 = classeA.begin() + cpt;
			else break; 
		}
		else {
			cpt++;
			it3++; 
		}
		 
		temp2.clear(); 
	}
	
	vector<vector<float>>::iterator it4 = classeB.begin();
	for (it4; it4 != classeB.end();) {

		if (abs(it4->at(1) - medianClasseA) < abs(it4->at(1) - medianClasseB)) {
			temp2.push_back(it4->at(0));
			temp2.push_back(it4->at(1));
			classeA.push_back(temp2);
			classeB.erase(it4);
			if (classeB.size() > cpt)it4 = classeB.begin() + cpt;
			else break; 
		}
		else {
			cpt++;
			it4++;
		}

		temp2.clear(); 
	}


	
}


void WhiteRunDetection::correctSkew(float angle, Mat& src) {

	
	Point2f center((src.cols - 1) / 2.0, (src.rows - 1) / 2.0);
	Mat rot = getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle, center not relevant
	Rect2f bbox = RotatedRect(Point2f(), src.size(), angle).boundingRect2f();

	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - src.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - src.rows / 2.0;

	warpAffine(src, src, rot, bbox.size());

}

void WhiteRunDetection::eraseBorder(Mat& src, float angle, Mat temp2) {

	
	float temp;
	vector<Vec4i> hierarchy;
	vector<vector<Point>> contours;

	Point2f center((temp2.cols - 1) / 2.0, (temp2.rows - 1) / 2.0);
	Mat rot = getRotationMatrix2D(center, angle, 1.0);
	// determine bounding rectangle, center not relevant
	Rect2f bbox = RotatedRect(Point2f(), temp2.size(), angle).boundingRect2f();

	// adjust transformation matrix
	rot.at<double>(0, 2) += bbox.width / 2.0 - temp2.cols / 2.0;
	rot.at<double>(1, 2) += bbox.height / 2.0 - temp2.rows / 2.0;

	Point2f center2;
	center2.x = center.x * rot.at<double>(0, 0) + center.y * rot.at<double>(0, 1) + rot.at<double>(0, 2);
	center2.y = center.x * rot.at<double>(1, 0) + center.y * rot.at<double>(1, 1) + rot.at<double>(1, 2);
	RotatedRect rect(center2, temp2.size(), -angle);

	
	Mat draw(src.size(), CV_8U);
	
	
	Point2f vertices[4];
	rect.points(vertices);
	
	for (int i = 0; i < 4; i++) {
		line(draw, vertices[i], vertices[(i + 1) % 4], 255 , 3, 8);
	}
	

	Mat src_copy = draw.clone(); 
	findContours(src_copy, contours, hierarchy, CV_RETR_TREE, CHAIN_APPROX_SIMPLE);

	
	Mat raw_dist(draw.size(), CV_32FC1);

	for (int j = 0; j < draw.rows; j++) {
		for (int i = 0; i < draw.cols; i++) {
			if (pointPolygonTest(contours[0], Point2f(i, j), true) <= 4)src.at<uchar>(j, i) = 255; 
			
		}
	}

}



void WhiteRunDetection::clearAll() {
	horizontalWhiteRun.clear();
	verticalWhiteRun.clear();
	verticalWhiteRunHisto.clear();
	horizontalWhiteRunHisto.clear();
	classeA.clear();
	classeB.clear();
	verticalWhiteRunPos.clear();
	horizontalWhiteRunPos.clear();
	cluster.clear();
	clusterLength.clear();
	mostReleventClusters.clear();
	straightLine.clear();
	 
}



Mat& WhiteRunDetection::getSrc() {
	return src; 
}

Mat& WhiteRunDetection::getBase_img() {
	return base_img;
}

vector<float> WhiteRunDetection::getHorizontalWhiteRun() {
	return horizontalWhiteRun; 
}

vector<float> WhiteRunDetection::getVerticalWhiteRun() {
	return verticalWhiteRun; 
}

vector<vector<float>>& WhiteRunDetection::getVerticalWhiteRunHisto(){
	return verticalWhiteRunHisto; 
}

vector<vector<float>>& WhiteRunDetection::getHorizontalWhiteRunHisto() {
	return horizontalWhiteRunHisto; 
}

vector<Point> WhiteRunDetection::getVerticalWhiteRunPos() {

	return verticalWhiteRunPos; 
}

vector<Point> WhiteRunDetection::getHorizontalWhiteRunPos() {

	return horizontalWhiteRunPos; 
}



vector<vector<Point>>& WhiteRunDetection::getCluster() {
	return cluster; 
}

vector<vector<float>>& WhiteRunDetection::getClusterLength() {
	return clusterLength; 
}

vector<vector<Point>>& WhiteRunDetection::getMostReleventClusters() {
	return mostReleventClusters; 
}



vector<float>& WhiteRunDetection::getStraightLine() {
	return straightLine; 
}

vector<float>& WhiteRunDetection::getAngles() {
	return angles; 
}

float WhiteRunDetection::getMedian() {
	return median;
}

int WhiteRunDetection::getRun() {
	return run;
}

bool WhiteRunDetection::getRotationInv() {
	return rotationInv;
}

float WhiteRunDetection::getInterLineSpacingVert() {
	return interLineSpacingVert; 
}


float WhiteRunDetection::getInterLineSpacingHori() {
	return interLineSpacingHori; 
}


vector<vector<float>>& WhiteRunDetection::getClasseA() {
	return classeA; 
}


vector<vector<float>>& WhiteRunDetection::getClasseB() {
	return classeB;
}


float WhiteRunDetection::getPopVert() {
	return popVert;
}

float WhiteRunDetection::getInterVert() {
	return interVert;
}

float WhiteRunDetection::getPopHori() {
	return popHori;
}

float WhiteRunDetection::getInterHori() {
	return interHori;
}

float WhiteRunDetection::getAngle() {
	return angle; 
}

void WhiteRunDetection::setMedian(float value) {
	median = value;
}

void WhiteRunDetection::incrementRun() {
	run++; 
}

void WhiteRunDetection::setAngle(float value) {
	angle = value; 
}

void WhiteRunDetection::setAngles(float value) {
	angles.push_back(value); 
}

void WhiteRunDetection::setPopVert(float value) {
	popVert = value; 
}

void WhiteRunDetection::setInterVert(float value) {
	interVert = value; 
}

void WhiteRunDetection::setPopHori(float value) {
	popHori = value; 
}

void WhiteRunDetection::setInterHori(float value) {
	interHori = value; 
}


void WhiteRunDetection::setInterLineSpacingVert(float value) {
	interLineSpacingVert = value; 
}

void WhiteRunDetection::setInterLineSpacingHori(float value) {
	interLineSpacingHori = value;
}

void  WhiteRunDetection::setRotationInv(bool value) {
	rotationInv = value;
}

void  WhiteRunDetection::setVerticalWhiteRunHisto(vector<vector<float>> vect) {
	verticalWhiteRunHisto = vect;
}

void  WhiteRunDetection::setHorizontalWhiteRunHisto(vector<vector<float>> vect) {
	horizontalWhiteRunHisto = vect;
}
