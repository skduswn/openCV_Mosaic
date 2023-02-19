//#include "opencv2/opencv.hpp"
//#include <iostream>
//
//using namespace cv;
//using namespace cv::ml;
//using namespace std;
//
//Ptr<SVM> train_hog_svm(const HOGDescriptor& hog);
//void detect_face();
//
//vector<Rect> faces;
//Mat img;
//int I;
//
//
//
//
//int main() 
//{//release mode
//#if _DEBUG
//	cout << "svmdigit.exe should be built as Relase mode!" << endl;
//	return 0;
//#endif
//
//	HOGDescriptor hog(Size(100, 100), Size(10, 10), Size(5, 5), Size(5, 5), 9);
//	Ptr<SVM> svm = train_hog_svm(hog);
//
//	if (svm.empty()) {
//		cerr << "Training failed!" << endl;
//		return -1;
//	}
//
//	detect_face();
//
//	imshow("img", img);
//	waitKey(0);
//
//
//	while (true) {
//		int c = waitKey();
//
//		if (c == 27) {
//			break;
//		}
//		else if (c == ' ') {
//			cout << "enter k or n or p : " << endl;
//
//			char C;
//			cin >> C;
//
//			for (int i = 0; i < faces.size(); i++) {
//				Point str(faces[i].x, faces[i].y);
//				Point end(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
//
//				Mat test = img(Rect(str, end));
//
//				Mat img_resize;
//				resize(test, img_resize, Size(100, 100), 0, 0, INTER_AREA);
//
//				vector<float> desc;
//				hog.compute(img_resize, desc);
//
//				Mat desc_mat(desc);
//				int res = cvRound(svm->predict(desc_mat.t()));
//				
//
//				if (C == 75 || C == 107) { //시현
//					if (res == 1) {
//						cout << res << endl;
//						I = i;
//					}
//				}
//				if (C == 78 || C == 110) { //연주
//					if (res == 0) {
//						cout << res << endl;
//						I = i;
//					}
//				}
//				if (C == 80 || C == 112) { //이진
//					if (res == 2) {
//						cout << res << endl;
//						I = i;
//					}
//				}
//
//			/*	if (res == 0) {
//					cout << res << endl;
//					imshow("연주", test);
//					waitKey(0);
//				}
//				else if (res == 1) {
//					cout << res << endl;
//					imshow("시현", test);
//					waitKey(0);
//				}
//				else if (res == 2) {
//					cout << res << endl;
//					imshow("이진", test);
//					waitKey(0);
//				}*/
//			}
//		}
//	}
//	return 0;
//}
//
//
//
//
//Ptr<SVM> train_hog_svm(const HOGDescriptor& hog) 
//{
//
//	Mat train = imread("face.png", IMREAD_GRAYSCALE);
//	if (train.empty()) {
//		cerr << "Image load failed!" << endl;
//		return 0;
//	}
//	
//	
//	Mat train_hog, labels;
//
//	for (int j = 0; j < 15; j++) {
//		for (int i = 0; i < 10; i++) {
//			Mat roi = train(Rect(i * 100, j * 100, 100, 100));
//
//			vector<float> desc;
//			hog.compute(roi, desc);
//
//			Mat desc_mat(desc);
//			train_hog.push_back(desc_mat.t());
//			labels.push_back(j / 3);
//		}
//	}
//
//	Ptr<SVM> svm = SVM::create();
//	svm->setType(SVM::Types::C_SVC);
//	svm->setKernel(SVM::KernelTypes::RBF);
//	svm->setC(62.5);
//	svm->setGamma(0.00015);
//	svm->train(train_hog, ROW_SAMPLE, labels);
//
//	//svm->trainAuto(train_hog, ROW_SAMPLE, labels);
//	//cout << "C : " << svm->getC() << endl;
//	//cout << "Gamma :" << svm->getGamma() << endl;
//
//	return svm;
//
//}
//
//
//
//void detect_face()
//{
//	img = imread("img1.png", IMREAD_GRAYSCALE);
//	
//	if (img.empty()) {
//		cerr << "Image load failed!" << endl;
//	}
//
//	CascadeClassifier classifier("haarcascade_frontalface_default.xml");
//
//	if (classifier.empty()) {
//		cerr << "XML load failed!" << endl;
//		return;
//	}
//
//	classifier.detectMultiScale(img, faces);
//
//
//	for (int i = 0; i < faces.size(); i++) {
//		Point str(faces[i].x, faces[i].y);
//		Point end(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
//
//		rectangle(img, str, end, Scalar(0, 255, 0), 1);
//	}
//
//}