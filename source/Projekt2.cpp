#include <opencv2/opencv.hpp>
using namespace std;
using namespace cv;

void ObjectCounter(){
		int counter = 0; // Zähler
		int frameNumber = 0;
		bool isPressed= false; 
		bool s1 = false; 		
		bool s2 = false;  		
		bool s3 = false;
		int s1Frame=0; 
		int x1Anzahlw=0;		
		int x2Anzahlw=0;		
		int x3Anzahlw=0;
	    VideoCapture videoCapture;
        videoCapture.open(0); // Kamera anschalten
        int width = videoCapture.get(CV_CAP_PROP_FRAME_WIDTH);
        int height = videoCapture.get(CV_CAP_PROP_FRAME_HEIGHT);

		namedWindow("WebCam2");
        namedWindow("CompareFrame");
        namedWindow("Maske");
		Mat videoFrame;
        Mat compareFrame(height, width, CV_8UC3);
		Mat binaerMaske(height, width, CV_8UC1);
 
		cout << "Height: " << height << " Width: " << width << endl;
		cout << "Druecken Sie eine Taste um den Firstframe zu erstellen"<< endl; 

        while(true){
            if (false == videoCapture.read(videoFrame)){
                    break;
            }
            frameNumber++;
            int key = waitKey(8); 
			if (key!= -1){
                    videoFrame.copyTo(compareFrame);
						imshow("CompareFrame", compareFrame);
						isPressed = true; 
            }
			if(true==isPressed){
				for(int x = 0; x < (width); x++){ // WICHTIG SIND EIGENTLICh NUR ETWA 50 PX (x=550)
					for(int y = 0; y < height; y++){
						Vec3b pixelVideo = videoFrame.at<Vec3b>(y, x);
						Vec3b pixelFirstFrame = compareFrame.at<Vec3b>(y,x);
						int helligkeitVideo = (pixelVideo[0] + pixelVideo[1]+ pixelVideo[2])/3;
						int helligkeitFirstFrame = (pixelFirstFrame[0] + pixelFirstFrame[1] + pixelFirstFrame[2])/3;
						int binaerPixel;
						if (abs(helligkeitVideo - helligkeitFirstFrame) > 25){		//SCHWELLE BEI 20
							binaerPixel = 255;
						}
						else {
							binaerPixel = 0;
						}
						binaerMaske.at<uchar>(y,x) = binaerPixel;
					}
				} //Frame zu Ende
			}// If zu Ende															 
			//[s2,	, s1,    , s3       ]
			
			for (int k=0; k<height; k++){ //[weis>20, weis>20, schwarz<20] erst dann counter ++
				int x1 = binaerMaske.at<uchar>(k,500);		// erste Schranke   Ausgang [300px	,400px	 ,500px      ] <--Eingang
				int x2 = binaerMaske.at<uchar>(k,100);		// zweite Schranke 
				int x3 = binaerMaske.at<uchar>(k,101);		// dritte Schranke muss Schwarz (Obejkt hat verlassen)
						
				if (x1 == 255){
					x1Anzahlw++;  	
					if (x1Anzahlw > 20){
						s1 = true;
					}
				}

	
				if (x2 == 255){
					x2Anzahlw++;  	
					if (x2Anzahlw > 20  && s1== true){	
						s2 = true;
						s1 = false; 	
					}
				}

				if (x3 == 255){
					x3Anzahlw++;  	
					if (x2Anzahlw < 15  && s2== true){	
						s3 = true;
						s2= false; 
					}
				}
			}
			 
			cout<< "S1:" <<s1 << " S2:" << s2 << " S3:" << s3 << " Frame"<< frameNumber <<endl; 
			if (s3 == true){
				counter++; 
				cout  <<counter<< endl; 
				s3 = false; 
			}
			//s1 = false; s2 = false; s3 = false;
			s1Frame = 0; 
			x1Anzahlw = 0; x2Anzahlw = 0; x3Anzahlw = 0;
            imshow("WebCam2", videoFrame);
            imshow("Maske", binaerMaske);
 
        }// While zu Ende 
}


int main(){
	ObjectCounter(); 
}