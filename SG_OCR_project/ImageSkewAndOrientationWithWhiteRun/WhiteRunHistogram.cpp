#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<iostream>
#include<fstream>
#include<map>
#include"WhiteRunDetection.h"

#define _USE_MATH_DEFINES
#include<math.h>


using namespace std; 
using namespace cv; 


vector<vector<float>> classeA;
vector<vector<float>> classeB;

vector<float> horizontalWhiteRun;
vector<float> verticalWhiteRun;

vector<Point> verticalWhiteRunPos;
vector<Point> horizontalWhiteRunPos;
vector<vector<Point>> cluster; 
vector<vector<Point>> mostReleventClusters; 
vector<vector<float>> clusterLength;

vector<vector<float>> verticalWhiteRunHisto;
vector<vector<float>> horizontalWhiteRunHisto;
vector<float> straightLine;

Mat drawing; 

int vertMaximizeVariance;
int horiMaximizeVariance;
int interLineSpacingVert;
int interLineSpacingHori;
float sumPIxI;
float angle;
float meanAngle = 0; 
bool rotationInv = false; 

void computeVerticalWhiteRun(Mat src); 
void computeHorizontalWhiteRun(Mat src);

void computeVerticalWhiteRunSavePos(Mat src, int length);
void computeHorizontalWhiteRunSavePos(Mat src, int length);

vector<vector<float>> computeNumberOfWhiteRun(vector<float> whiteRun);

void computeClusterLength(vector<vector<Point>>& clust);
void straightLineEstimation();
void buildCluster(vector<Point> whiteRun, int length);
//void buildCluster2(vector<Point> whiteRunPos, int length); 


int euclidianDistance(Point p1, Point p2);
bool sortingFunc(const vector<float>& vec1, const vector<float>& vec2); 
bool sortingFuncLast(const vector<float>& vec1, const vector<float>& vec2);
bool sortingFuncInvFirst(const vector<float>& vec1, const vector<float>& vec2); 
bool sortingFuncInv(const vector<float>& vec1, const vector<float>& vec2); 
void choseMostReleventCluster(vector<vector<float>> clusterLength, vector<vector<Point>> mergeClusters); 
bool rotationDirection(vector<vector<Point>> mergeClusters); 
float computeAngle(Mat img); 
vector<vector<float>> computeNormalizedDensity(vector<vector<float>> whiteRunHisto, int nbWhiteRunTot); 
vector<vector<float>> computeZerothOrderCumulative(vector<vector<float>> normalizedDensity); 
float computeSumPIxI(vector<vector<float>> normalizedDensity); 
int detectSecondPeak(vector<vector<float>> zerothOrderCumulative,vector<vector<float>> normalizedDensity);
float detectSecondPeak2(vector<vector<float>> WhiteRunHisto);
void  constructClass(vector<vector<float>> whiteRunHisto); 

int foundInterlineSpacing(vector<vector<float>> whiteRunHisto, int lengthMaximizingVariance); 
float foundMedian(); 

/*
int main(int argc, char *argv[]) {


	
	Mat img = imread("./image/text_2.jpg", CV_LOAD_IMAGE_UNCHANGED);	
	drawing = imread("./image/text_2.jpg", CV_LOAD_IMAGE_UNCHANGED);
	cvtColor(drawing, drawing, CV_GRAY2BGR);
	
	/*
	WhiteRunDetection w; 
	w.computeVerticalWhiteRun(img); 
	w.setVerticalWhiteRunHisto(w.computeNumberOfWhiteRun(w.getVerticalWhiteRun()));
	w.setInterLineSpacingVert(w.detectSecondPeak(w.getVerticalWhiteRunHisto())); 
	w.computeVerticalWhiteRunSavePos(img, w.getInterLineSpacingVert());
	w.buildCluster(w.getVerticalWhiteRunPos(), w.getInterLineSpacingVert());
	//affichage des clusters
	cout << endl << "clusters" << endl;
	vector<vector<Point>>::iterator it10;
	vector<Point>::iterator it10_bis;
	for (it10 = w.getCluster().begin(); it10 != w.getCluster().end(); it10++) {

		for (it10_bis = it10->begin(); it10_bis != it10->end(); it10_bis++) {
			cout << *it10_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	sort(w.getCluster().begin(), w.getCluster().end(), [](const vector<Point> &a, const vector<Point> &b) {return a.size() > b.size(); });

	cout << endl << "clusters" << endl;
	vector<vector<Point>>::iterator it11;
	vector<Point>::iterator it11_bis;
	for (it11 = w.getCluster().begin(); it11 != w.getCluster().end(); it11++) {

		for (it11_bis = it11->begin(); it11_bis != it11->end(); it11_bis++) {
			cout << *it11_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	w.computeClusterLength(w.getCluster()); 


	cout << endl << "clusters length" << endl;
	vector<vector<float>>::iterator it12;
	vector<float>::iterator it12_bis;
	for (it12 = w.getClusterLength().begin(); it12 != w.getClusterLength().end(); it12++) {

		for (it12_bis = it12->begin(); it12_bis != it12->end(); it12_bis++) {
			cout << *it12_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	sort(w.getClusterLength().begin(), w.getClusterLength().end(), sortingFunc);

	cout << endl << "clusters length" << endl;
	vector<vector<float>>::iterator it13;
	vector<float>::iterator it13_bis;
	for (it13 = w.getClusterLength().begin(); it13 != w.getClusterLength().end(); it13++) {

		for (it13_bis = it13->begin(); it13_bis != it13->end(); it13_bis++) {
			cout << *it13_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	w.setRotationInv(w.rotationDirection(w.getCluster())); 
	
	w.choseMostReleventCluster(w.getClusterLength(),w.getCluster());


	vector<vector<Point>>::iterator it14;
	vector<Point>::iterator it14_bis;
	for (it14 = w.getMostReleventClusters().begin(); it14 != w.getMostReleventClusters().end(); it14++) {

		for (it14_bis = it14->begin(); it14_bis != it14->end(); it14_bis++) {
			cout << *it14_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	w.straightLineEstimation(); 
	vector<float>::iterator it;
	for (it = w.getStraightLine().begin(); it != w.getStraightLine().end(); it++) {

		cout << *it << endl;

	}
	float median = w.foundMedian();
	cout << median << endl;
	cout << "found angle : " << atan(median) * 180 / M_PI;
	cin.get(); 
	
	
	//chargement des white run  verticaux et horizontaux
	//computeVerticalWhiteRun(img); 
	computeHorizontalWhiteRun(img); 
	 
	
	//calcul du nombre d'occurence par length 
	//verticalWhiteRunHisto = computeNumberOfWhiteRun(verticalWhiteRun); 
	horizontalWhiteRunHisto = computeNumberOfWhiteRun(horizontalWhiteRun);


	
	//rangement des vector<vector<float>> par ordre décroissant afin de trouvé la plus grande population
	//sort(verticalWhiteRunHisto.begin(), verticalWhiteRunHisto.end(), sortingFuncInv);
	//sort(horizontalWhiteRunHisto.begin(), horizontalWhiteRunHisto.end(), sortingFunc);

	
	/* 
	//affichage des histogramme vertical et horizontal (nombre d'occurence par length)
	cout << "vertical whiteRun histo" << endl;
	vector<vector<float>>::iterator it3;
	vector<float>::iterator it3_bis;
	for (it3 = verticalWhiteRunHisto.begin(); it3 != verticalWhiteRunHisto.end(); it3++) {
		for (it3_bis = it3->begin(); it3_bis != it3->end(); it3_bis++) {
			cout << *it3_bis << " ";
		}
		
		cout << endl;
	}
	cout << endl << endl;
	

	constructClass(horizontalWhiteRunHisto); 

	interLineSpacingHori = classeA.at(0).at(1) > classeB.at(0).at(1) ? classeA.at(0).at(1) : classeB.at(0).at(1);


	/*
	cout << "horizontal whiteRun histo" << endl;
	vector<vector<float>>::iterator it4;
	vector<float>::iterator it4_bis;
	for (it4 = horizontalWhiteRunHisto.begin(); it4 != horizontalWhiteRunHisto.end(); it4++) {
		for (it4_bis = it4->begin(); it4_bis != it4->end(); it4_bis++) {
			cout << *it4_bis << " ";
		}
		cout << endl;
	}
	*/
	

	/*
	//Detection du second peak dans chaque histogramme pour sélectionner le plus révélateur et trouver l'interline spacing 
	normalizedDensity = computeNormalizedDensity(verticalWhiteRunHisto, verticalWhiteRun.size());
	
	float sum = 0;
	cout << verticalWhiteRun.size() << endl << "normalized sensity" << endl;
	vector<vector<float>>::iterator it_norm;
	for (it_norm = normalizedDensity.begin(); it_norm != normalizedDensity.end(); it_norm++) {
		cout << it_norm->at(0) << " " << it_norm->at(1) << endl;
		sum += it_norm->at(1);
	}
	cout << sum << endl;
	cin.get();
	
	
	
	zerothOrderCumulative = computeZerothOrderCumulative(normalizedDensity); 
	sumPIxI = computeSumPIxI(normalizedDensity); 

	float sum = 0; 
	cout << verticalWhiteRun.size() << endl << "normalized sensity" << endl;
	vector<vector<float>>::iterator it_norm;
	for (it_norm = normalizedDensity.begin(); it_norm != normalizedDensity.end(); it_norm++) {
		cout << it_norm->at(0) << " " << it_norm->at(1) << endl;
		sum += it_norm->at(1); 
	}
	cout << sum << endl;
	cin.get(); 
	cout << endl << "zeroth order cumulative moment" << endl;
	vector<vector<float>>::iterator it_zeroth;
	for (it_zeroth = zerothOrderCumulative.begin(); it_zeroth != zerothOrderCumulative.end(); it_zeroth++) {
		cout << it_zeroth->at(0) << " " << it_zeroth->at(1) << endl;
	}
	cout << endl;
	

	vertMaximizeVariance = detectSecondPeak(zerothOrderCumulative, normalizedDensity); 
	cout << "length of the white run that maximize the interclasse variance : " << vertMaximizeVariance << endl;
	interLineSpacingVert = foundInterlineSpacing(verticalWhiteRunHisto, vertMaximizeVariance);
	cout << "interline spacing : " << interLineSpacingVert << endl;
	cin.get(); 
	
	normalizedDensity = computeNormalizedDensity(horizontalWhiteRunHisto, horizontalWhiteRun.size());
	zerothOrderCumulative = computeZerothOrderCumulative(normalizedDensity);
	sumPIxI = computeSumPIxI(normalizedDensity);

	horiMaximizeVariance = detectSecondPeak(zerothOrderCumulative, normalizedDensity);
	cout << "length of the white run that maximize the interclasse variance : " << horiMaximizeVariance << endl;
	interLineSpacingHori = foundInterlineSpacing(horizontalWhiteRunHisto, horiMaximizeVariance);
	cout << "interline spacing : " << interLineSpacingHori << endl;
	cin.get(); 
	
	


	//chargement des positions de tout les white run de length verticalWhiteRunHisto[0][1] (biggest population) 
	computeVerticalWhiteRunSavePos(img, interLineSpacingVert);
	//computeHorizontalWhiteRunSavePos(img, interLineSpacingHori);

	
	/*
	//construction des clusters, vector<vector<Point>> où chaque cluster contient les white run qui sont proche 
	if (interLineSpacingVert >= interLineSpacingHori) {
		cout << "vertical is more relevent !" << endl; 
		
	}
	else {
		cout << "horizontal is more relevent !" << endl;
		buildCluster(horizontalWhiteRunPos, interLineSpacingHori);
	}
	
	 
	buildCluster(horizontalWhiteRunPos, interLineSpacingHori);
	
	//rangement des cluster par size pour trouver le cluster contenant le plus de points
	sort(cluster.begin(), cluster.end(), [](const vector<Point> &a, const vector<Point> &b) {return a.size() > b.size(); }); 

	//calcul des longueur de chaque cluster après merge et classement du plus grand au plus petit
	computeClusterLength(cluster); 
	sort(clusterLength.begin(), clusterLength.end(), sortingFunc);
	
	
	//affichage des length de chaque cluster 
	cout << "cluster length" << endl; 
	vector<vector<float>>::iterator it8;
	vector<float>::iterator it8_bis;
	for (it8 = clusterLength.begin(); it8 != clusterLength.end(); it8++) {

		for (it8_bis = it8->begin(); it8_bis != it8->end(); it8_bis++) {
			cout << *it8_bis << " ";
		}
		cout << endl;
	}
	cout << endl << endl; 
	


	//affichage des clusters
	cout << endl << "clusters" << endl;
	cout << cluster.size() << endl;
	vector<vector<Point>>::iterator it10;
	vector<Point>::iterator it10_bis;
	for (it10 = cluster.begin(); it10 != cluster.end(); it10++) {

		for (it10_bis = it10->begin(); it10_bis != it10->end(); it10_bis++) {
			cout << *it10_bis << " ";
		}
		cout << endl;
	}
	cout << endl;

	
	 
	
	//regarde si il y a une majorité de cluster à angles négatif ou positif pour que choseMostReleventCluster sélectionne les bons 
	rotationInv = rotationDirection(cluster); 
	
	//choix des 10 plus grands cluster (euclidianDistance) dont l'angle donné est supérieur à 0 si rotationInv = false et inversement 
	choseMostReleventCluster(clusterLength, cluster); 
	vector<vector<Point>>::iterator it9;
	vector<Point>::iterator it9_bis;
	for (it9 = mostReleventClusters.begin(); it9 != mostReleventClusters.end(); it9++) {
		for (it9_bis = it9->begin(); it9_bis != it9->end(); it9_bis++) {
			cout << *it9_bis << " ";
		}
		angle = atan2(it9->at(it9->size() - 1).y - it9->at(0).y, it9->at(it9->size() - 1).x - it9->at(0).x) * 180 / M_PI; 
		cout << angle << endl;
		//meanAngle += angle; 
	}
	//meanAngle /= mostReleventClusters.size(); 
	//cout << "document skew angle found : " << meanAngle << endl; 

	
	
	straightLineEstimation(); 
	vector<float>::iterator it; 
	for (it = straightLine.begin(); it != straightLine.end(); it++){

		cout << *it << endl;

	}
	float median = foundMedian();
	cout << median << endl; 
	cout << "found angle : "  << atan(median) * 180 / M_PI; 

	//draw relevent clusters
	int B = 255;
	int R = 0; 
	vector<vector<Point>>::iterator it_draw; 
	for (it_draw = mostReleventClusters.begin(); it_draw != mostReleventClusters.end(); it_draw++) {
		for (int i = 0; i < it_draw->size() - 1; i++) {
			if(i < it_draw->size() - 2)line(drawing, it_draw->at(i), it_draw->at(i + 1), Scalar(B,0,R), 2);
			else break; 
		}
		if (B == 255 && R == 0) {
			B = 0;
			R = 255;
		}
		else {
			B = 255;
			R = 0;
		}
	}
	

	
	
	//show result and write image res
	namedWindow("test pb div zero", CV_WINDOW_NORMAL);
	imshow("test pb div zero", drawing);
	imwrite("testDivZero.jpg", drawing);
	waitKey(); 

	cin.get(); 
	
	
	return 0; 
	
}
*/


/*
float computeAngle(Mat img) {

	float angle; 

	//chargement des white run  verticaux et horizontaux
	computeVerticalWhiteRun(img);
	computeHorizontalWhiteRun(img);

	//calcul du nombre d'occurence par length 
	verticalWhiteRunHisto = computeNumberOfWhiteRun(verticalWhiteRun);
	horizontalWhiteRunHisto = computeNumberOfWhiteRun(horizontalWhiteRun);


	//rangement des vector<vector<float>> par ordre décroissant afin de trouvé la plus grande population
	sort(verticalWhiteRunHisto.begin(), verticalWhiteRunHisto.end(), sortingFunc);
	sort(horizontalWhiteRunHisto.begin(), horizontalWhiteRunHisto.end(), sortingFunc);


	//interclass variance to find the second peak on histogram
	detectSecondPeak(verticalWhiteRunHisto, verticalWhiteRun.size());
	sort(interClasseVariance.begin(), interClasseVariance.end(), sortingFunc);
	int lengthVert = interClasseVariance[0][1];
	interClasseVariance.clear();

	detectSecondPeak(horizontalWhiteRunHisto, horizontalWhiteRun.size());
	sort(interClasseVariance.begin(), interClasseVariance.end(), sortingFunc);
	int lengthHori = interClasseVariance[0][1];

	//chargement des positions de tout les white run de length verticalWhiteRunHisto[0][1] (biggest population) 
	computeVerticalWhiteRunSavePos(img, lengthVert);
	computeHorizontalWhiteRunSavePos(img, lengthHori);


	//construction des clusters, vector<vector<Point>> où chaque cluster contient les white run qui sont proche 
	if (lengthVert >= lengthHori) {
		buildCluster(verticalWhiteRunPos, lengthVert);
	}
	else {
		buildCluster(horizontalWhiteRunPos, lengthHori);
	}



	//rangement des cluster par size pour trouver le cluster contenant le plus de points
	sort(cluster.begin(), cluster.end(), [](const vector<Point> &a, const vector<Point> &b) {return a.size() > b.size(); });

	if (lengthVert > lengthHori)mergeNearestCluster(lengthVert);
	else mergeNearestCluster(lengthHori);

	//classement des cluster by size of vector<Point>
	sort(mergeCluster.begin(), mergeCluster.end(), [](const vector<Point> &a, const vector<Point> &b) {return a.size() > b.size(); });

	//calcul des longueur de chaque cluster après merge et classement du plus grand au plus petit
	computeClusterLength(mergeCluster);
	sort(clusterLength.begin(), clusterLength.end(), sortingFunc);


	rotationInv = rotationDirection(mergeCluster);

	//choix des 10 plus grands cluster (euclidianDistance) dont l'angle donné est supérieur à 0 si rotationInv = false et inversement 
	choseMostReleventCluster(clusterLength, mergeCluster);


	vector<vector<Point>>::iterator it9;
	for (it9 = mostReleventClusters.begin(); it9 != mostReleventClusters.end(); it9++) {
		meanAngle += atan2(it9->at(it9->size() - 1).y - it9->at(0).y, it9->at(it9->size() - 1).x - it9->at(0).x) * 180 / M_PI;;
	}
	meanAngle /= 10;
	return meanAngle; 
}
*/

void  computeVerticalWhiteRun(Mat src) {


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
				if(euclidianDistance(start, end) != 0)verticalWhiteRun.push_back(euclidianDistance(start, end));
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


void  computeHorizontalWhiteRun(Mat src) {

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
				if (euclidianDistance(start, end) != 0)horizontalWhiteRun.push_back(euclidianDistance(start, end));
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




void  computeVerticalWhiteRunSavePos(Mat src, int length) {
 
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
				if (euclidianDistance(start, end) == length) {
					verticalWhiteRunPos.push_back(start);
					line(drawing, start, end, Scalar(0,255,0)); 
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

void computeHorizontalWhiteRunSavePos(Mat src, int length) {

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
					line(drawing, start, end, Scalar(255, 0, 0));
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




int euclidianDistance(Point p1, Point p2) {

	return  sqrt(pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2)); 

}

vector<vector<float>> computeNumberOfWhiteRun(vector<float> whiteRun) {


	sort(whiteRun.begin(), whiteRun.end());
	 

	vector<float> temp; 
	vector<vector<float>> res; 
	bool computed = false; 
	
	

	for (size_t i = 0;i < whiteRun.size()-1;) {

		temp.push_back(whiteRun[i]);
		temp.push_back(0);
		
		
		while (temp[0] == whiteRun[i]) {
			temp[1]++;
			if(i == whiteRun.size() - 1)break;
			else i++; 
		}
		//temp.push_back(abs(temp.at(0)*temp.at(1))); 
		res.push_back(temp);
		temp.clear();



	}
	
	return res; 

}



bool sortingFuncLast(const vector<float>& vec1, const vector<float>& vec2) {
	
	if (vec1[vec1.size() - 1] < vec2[vec2.size() - 1]) {
		return true;
	}
	else if (vec1[vec1.size() - 1] < vec2[vec2.size() - 1]) {
		return false;
	}

	return false;
}

bool sortingFuncInvFirst(const vector<float>& vec1, const vector<float>& vec2) {

	if (vec1[0] < vec2[0]) {
		return false;
	}
	else if (vec1[0] < vec2[0]) {
		return true;
	}

	return false;
}
bool sortingFunc(const vector<float>& vec1, const vector<float>& vec2) {
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



bool sortingFuncInv(const vector<float>& vec1, const vector<float>& vec2) {
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



/*
void buildCluster2(vector<Point> whiteRunPos, int length) {
	

	int i = 0;
	int k = 0;
	int j = 0;
	bool firstRound = true;
	bool found = true;
	vector<Point> temp;
	vector<vector<Point>> temp2;

	vector<Point>::iterator it = whiteRunPos.begin(); 
	vector<Point>::iterator it2;
	vector<vector<Point>>::iterator it3;
	vector<Point>::iterator it4;



	for (it; it != whiteRunPos.end(); it++) {

		if(firstRound) temp.push_back(*it);
		it2 = whiteRunPos.begin() + 1;

		while(found){
			 
			if (!firstRound) {
				temp.push_back(temp2.at(temp2.size() -1).back());
			}
			 
			for (it2; it2 != whiteRunPos.end();) {

				if (it->x - it2->x < length && it->y - it2->y < length) {

					temp.push_back(*it2);
					whiteRunPos.erase(it2); 
					if (whiteRunPos.size() > 1 + i) {
						it2 = whiteRunPos.begin() + 1 + i; 
					}
					else break; 
					k++; 
				}
				else {
					i++; 
					it2++; 
				}

			}
			if (k >= 1) {
				found = true;
				firstRound = false;
			}
			else {
				found = false;
			}
			temp2.push_back(temp); 
			temp.clear();
			whiteRunPos.erase(it);
			k = 0;
			i = 0; 
			it2 = whiteRunPos.begin(); 
		}
		firstRound = true; 
		found = true;
		for (it3 = temp2.begin(); it3 != temp2.end(); it3++) {

			for (it4 = it3->begin()+1; it4 != it3->end(); it4++) {
				cluster.at(j).push_back(*it4);
			}

		}
		//cluster.push_back(temp); 
		//temp.clear(); 
		if (whiteRunPos.size() >  1) {
			it = whiteRunPos.begin();
		}
		else break; 
		j++; 
		
	}

}
*/


void buildCluster(vector<Point> whiteRunPos, int length) {

	int i = 0;
	vector<Point> vecTemp;

	vector<Point>::iterator it = whiteRunPos.begin();
	vector<Point>::iterator it2 = whiteRunPos.begin() + 1;


	for (it; it != whiteRunPos.end();) {

		vecTemp.push_back(*it);

		for (it2; it2 != whiteRunPos.end();) {

			if (abs(it2->x - vecTemp.back().x) < length  && abs(it2->y - vecTemp.back().y) < length ) {
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
		cluster.push_back(vecTemp);
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

void computeClusterLength(vector<vector<Point>>& clust) {

	int i = 0; 
	int length = 0; 
	int clusterNumber = 0; 
	vector<float> temp; 
	vector<vector<Point>>::iterator it;
	
	for (it = clust.begin(); it != clust.end();  clusterNumber++) {

		length = euclidianDistance(it->at(0), it->at(it->size() -1));
		if (length != 0) {
			temp.push_back(clusterNumber);
			temp.push_back(length);
			clusterLength.push_back(temp);
			temp.clear();
			i++; 
			it++; 
		}
		else {
			clust.erase(it);
			if (clust.size() > i)it = clust.begin()+i; 
			else break; 
		}
		
		
	}

}

void straightLineEstimation() {

	int sumXY = 0; 
	int sumSquareX = 0; 
	float meanX = 0;
	float meanY = 0;
	float k = 0; 

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
	}


}

float foundMedian() {
	float median; 

	sort(straightLine.begin(), straightLine.end()); 


	if (straightLine.size() % 2 == 0) {
		median = straightLine.at((straightLine.size() / 2) - 1) + straightLine.at(straightLine.size() / 2);
		median /= 2;
	}
	else {
		median = straightLine.at((straightLine.size() + 1) / 2); 
	}

	return median; 
}

float foundMedianVectFloat(vector<vector<float>> vect) {

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



void  constructClass(vector<vector<float>> whiteRunHisto) {

	int cpt = 0;
	float temp;
	float medianClasseA;
	float medianClasseB;
	vector<float> temp2;
	vector<vector<float>> partClasseA;
	vector<vector<float>> partClasseB;



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

		//if ( it2->at(1) >= classeA.at(0).at(1) - 3 && it2->at(1)  <= classeA.at(0).at(1) + 3) {
		if (it2->at(1) >= classeA.at(0).at(1) - 3 && it2->at(1) <= classeA.at(0).at(1) + 3) {

			temp2.push_back(it2->at(0));
			temp2.push_back(it2->at(1));
			classeA.push_back(temp2);

		}
		else {
			temp2.push_back(it2->at(0));
			temp2.push_back(it2->at(1));
			classeB.push_back(temp2);
		}
		temp2.clear();
	}

	if (classeA.size() > 50) {
		vector<vector<float>>::iterator first = classeA.begin();
		vector<vector<float>>::iterator last = classeA.begin() + ((classeA.size() * 10) / 100);
		vector<vector<float>> partClasseA(first, last);
		medianClasseA = foundMedianVectFloat(partClasseA);
	}
	else medianClasseA = foundMedianVectFloat(classeA);


	if (classeB.size() > 50) {
		vector<vector<float>>::iterator first2 = classeB.begin();
		vector<vector<float>>::iterator last2 = classeB.begin() + ((classeB.size() * 10) / 100);
		vector<vector<float>> partClasseB(first2, last2);
		medianClasseB = foundMedianVectFloat(partClasseB);
	}
	else medianClasseB = foundMedianVectFloat(classeB);




	vector<vector<float>>::iterator it3 = classeA.begin();
	for (it3; it3 != classeA.end(); it3++) {

		if (abs(it3->at(1) - medianClasseB) < abs(it3->at(1) - medianClasseA)) {
			temp2.push_back(it3->at(0));
			temp2.push_back(it3->at(1));
			classeB.push_back(temp2);
			classeA.erase(it3);
			if (classeA.size() > cpt)it3 = classeA.begin() + cpt;
		}
		else cpt++;

		temp2.clear();
	}

	vector<vector<float>>::iterator it4 = classeB.begin();
	for (it4; it4 != classeB.end(); it4++) {

		if (abs(it4->at(1) - medianClasseA) < abs(it4->at(1) - medianClasseB)) {
			temp2.push_back(it4->at(0));
			temp2.push_back(it4->at(1));
			classeA.push_back(temp2);
			classeB.erase(it4);
			if (classeB.size() > cpt)it4 = classeB.begin() + cpt;
		}
		else cpt++;

		temp2.clear();
	}



}

vector<vector<float>> computeNormalizedDensity(vector<vector<float>> whiteRunHisto, int nbWhiteRunTot) {

	float temp;
	vector<vector<float>> normalizedDensity; 
	vector<float> temp2; 

	/*
	vector<vector<float>>::iterator it = whiteRunHisto.begin(); 
	for (it; it != whiteRunHisto.end(); it++) {
		temp = it->at(0); 
		it->at(0) = it->at(1); 
		it->at(1) = temp; 
	}
	sort(whiteRunHisto.begin(), whiteRunHisto.end(), sortingFuncInv); 
	*/

	
	vector<vector<float>>::iterator it2 = whiteRunHisto.begin();
	for (it2; it2 != whiteRunHisto.end(); it2++) {
		temp2.push_back(it2->at(0)); 
		temp2.push_back(it2->at(1) / nbWhiteRunTot); 
		normalizedDensity.push_back(temp2); 
		temp2.clear(); 
	}

	return normalizedDensity; 

}

vector<vector<float>> computeZerothOrderCumulative(vector<vector<float>> normalizedDensity) {

	vector<float> temp; 
	vector<vector<float>> zerothOrderCumulative; 

	vector<vector<float>>::iterator it;
	vector<vector<float>>::iterator it2; 
	for (it = normalizedDensity.begin(); it != normalizedDensity.end(); it++) {
		temp.push_back(it->at(0));
		temp.push_back(it->at(1));
		it2 = normalizedDensity.begin();
		while (it2->at(0) < it->at(0)) {
			temp.at(1) += it2->at(1); 
			if(it2 != normalizedDensity.end() - 1)it2++;
			else break; 
		}
		zerothOrderCumulative.push_back(temp);
		temp.clear(); 
	}

	return zerothOrderCumulative; 
}

float computeSumPIxI(vector<vector<float>> normalizedDensity) {

	float sum = 0; 

	vector<vector<float>>::iterator it = normalizedDensity.begin(); 
	for (it; it != normalizedDensity.end(); it++) {
		sum += it->at(0) * it->at(1); 
	}

	return sum; 
}

int detectSecondPeak(vector<vector<float>> zerothOrderCumulative, vector<vector<float>> normalizedDensity) {
	
	vector<vector<float>> varianceinter;  
	vector<float> temp;
	float temp2 = 0; 
	float sumPIxIuntilK = 0;  
	

	vector<vector<float>>::iterator it = zerothOrderCumulative.begin(); 
	vector<vector<float>>::iterator it2;
	for (it; it != zerothOrderCumulative.end(); it++) {

		
		it2 = normalizedDensity.begin();
		while (it2->at(0) <= it->at(0)) {

			sumPIxIuntilK += it2->at(0) * it2->at(1);
			if(it2 != normalizedDensity.end() - 1)it2++;
			else break; 
		
		} 
		cout << endl << "sumPIxIuntilK " << sumPIxIuntilK << " " << it->at(0) << endl; 


		temp2 = pow(((it->at(1) * sumPIxI) - sumPIxIuntilK),2) / (it->at(1) * (1 - it->at(1))); 


		temp.push_back(temp2); 
		temp.push_back(it->at(0)); 
		varianceinter.push_back(temp);
		temp.clear(); 
		sumPIxIuntilK = 0; 
		
	 

	}

	

	sort(varianceinter.begin(), varianceinter.end(), sortingFunc);
	
	vector<vector<float>>::iterator it3 = varianceinter.begin();
	for (it3; it3 != varianceinter.end(); it3++) {
		cout << it3->at(0) << " " << it3->at(1) << endl;
	}
	cout << endl;
	
	return varianceinter.at(0).at(1); 

}

int foundInterlineSpacing(vector<vector<float>> whiteRunHisto, int lengthMaximizingVariance) {

	vector<float> temp; 
	float temp2 = 0; 

	 

	vector<vector<float>>::iterator it_bis;
	vector<vector<float>>::iterator it2 = whiteRunHisto.begin();

	vector<vector<float>>::iterator it3 = whiteRunHisto.begin();
	for (it3; it3 != whiteRunHisto.end(); it3++) {
		temp2 = it3->at(0);
		it3->at(0) = it3->at(1);
		it3->at(1) = temp2;
	}
	sort(whiteRunHisto.begin(), whiteRunHisto.end(), sortingFuncInv);

	
	vector<vector<float>>::iterator it = whiteRunHisto.begin();
	while (it->at(0) != lengthMaximizingVariance) {
		if(it != whiteRunHisto.end() -1) it++;
		else break; 
	}
	it_bis = it+1;
	temp.push_back(it_bis->at(0));
	temp.push_back(it_bis->at(1));

	it_bis++; 
	for (it_bis; it_bis != whiteRunHisto.end(); it_bis++) {
		if (it_bis->at(1) > temp.at(1)) {
			temp.clear(); 
			temp.push_back(it_bis->at(0));
			temp.push_back(it_bis->at(1));
		}
	}

	return temp.at(0); 
}



bool rotationDirection(vector<vector<Point>> clusters) {

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

void choseMostReleventCluster(vector<vector<float>> clusterLength, vector<vector<Point>> clusters) {

	vector<vector<float>>::iterator it9 = clusterLength.begin(); 
	while (mostReleventClusters.size() < round(((clusters.size() * 10) / 100)) && it9 != clusterLength.end()) {
		if (rotationInv) {
			
			if (atan2(clusters.at(it9->at(1)).at(clusters.at(it9->at(1)).size() - 1).y - clusters.at(it9->at(1)).at(0).y, clusters.at(it9->at(1)).at(clusters.at(it9->at(1)).size() - 1).x - clusters.at(it9->at(1)).at(0).x) * 180 / M_PI < 0) {
				mostReleventClusters.push_back(clusters.at(it9->at(1)));
			}
			it9++;

		}
		else{

			if (atan2(clusters.at(it9->at(1)).at(clusters.at(it9->at(1)).size() - 1).y - clusters.at(it9->at(1)).at(0).y, clusters.at(it9->at(1)).at(clusters.at(it9->at(1)).size() - 1).x -clusters.at(it9->at(1)).at(0).x) * 180 / M_PI > 0) {
				mostReleventClusters.push_back(clusters.at(it9->at(1)));
			}
			it9++;  

		}
	}

}


float detectSecondPeak2(vector<vector<float>> whiteRunHisto) {

	float temp; 
	float res; 


	vector<vector<float>>::iterator it = whiteRunHisto.begin();
	for (it; it != whiteRunHisto.end(); it++) {
		temp = it->at(0); 
		it->at(0) = it->at(1); 
		it->at(1) = temp; 
	}
	sort(whiteRunHisto.begin(), whiteRunHisto.end(),sortingFunc);

	
	res = (whiteRunHisto.at(0).at(1) > whiteRunHisto.at(1).at(1)) ? whiteRunHisto.at(0).at(1) : whiteRunHisto.at(1).at(1); 


	return res;
}