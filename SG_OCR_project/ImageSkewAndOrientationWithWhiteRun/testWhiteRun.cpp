#include "WhiteRunDetection.h"


 

int main(int argc, char* argv[]) {
	




	float temp = 0; 
	Mat img = imread("./image/text_sub0.jpg", CV_LOAD_IMAGE_UNCHANGED);
	Mat temp2;
	WhiteRunDetection w(img);

	do {

		threshold(w.getSrc(), w.getSrc(), 200, 255, THRESH_BINARY);
		
		
		w.computeWhiteRuns(); 


		w.founInterlineSpacing(); 

		
		w.choseVertOrHori(); 

		
		w.setRotationInv(w.rotationDirection(w.getCluster()));


		w.choseMostReleventCluster();


		w.straightLineEstimation();


		w.setMedian(w.foundMedian());
		
		w.setAngles(atan(w.getMedian()) * 180 / M_PI);
		
		w.setAngle(w.getAngles().back());
		
		w.correctSkew(w.getAngle(), w.getSrc());
		
		imwrite("./imageRes/-45/run"+to_string(w.getRun())+".jpg", w.getSrc()); 


		w.incrementRun();
		w.clearAll(); 

		
		if (w.getAngle() < 0) w.setAngle(abs(w.getAngle()));
	
	} while (w.getAngle() > 1);
 
	vector<float>::iterator it_temp = w.getAngles().begin(); 
	for (it_temp; it_temp != w.getAngles().end(); it_temp++) {
		temp += *it_temp;
		cout <<*it_temp << endl;  
	}
	cout << temp << endl; 
	w.setAngle(temp); 

	
	w.getBase_img().copyTo(temp2);


	w.correctSkew(w.getAngle(), w.getBase_img());
	w.eraseBorder(w.getBase_img(), temp, temp2);

	imwrite("./imageRes/-45/final.jpg", w.getBase_img());

	cin.get();
	

}

