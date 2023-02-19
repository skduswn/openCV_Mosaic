#include "opencv2/opencv.hpp"
#include <iostream>

using namespace cv;
using namespace std;
using namespace cv::ml;

Mat img;
Point ptOld;
vector<Rect> faces;
int Pos;
int cnt;
Point ptr[2];
int I;

void detect_face();   //얼굴 검출
void on_mouse(int event, int x, int y, int flags, void*);
void drawMosaic(vector<Rect> faces); //모자이크
void on_AllMosaic(int pos, void* userdata); //트랙바
Ptr<SVM> train_hog_svm(const HOGDescriptor& hog); //얼굴 인식
void findPerson();


int main(int argc, char* argv[])
{
   
    namedWindow("img");

    detect_face();
    createTrackbar("Allmode", "img", 0, 2, on_AllMosaic, (void*)&img);

    setMouseCallback("img", on_mouse);

    imshow("img", img);
    waitKey();

    return 0;
}




void detect_face()
{
    img = imread("im4.jpg", IMREAD_COLOR);
	if (img.empty()) {
		cerr << "Image load failed!" << endl;
	}

    CascadeClassifier classifier("haarcascade_frontalface_default.xml");

    if (classifier.empty()) {
        cerr << "XML load failed!" << endl;
        return;
    }

    classifier.detectMultiScale(img, faces);


    for (int i = 0; i < faces.size(); i++) {
        Point str(faces[i].x, faces[i].y);
        Point end(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

        rectangle(img, str, end, Scalar(0, 255, 0), 1);
    }

}



void drawMosaic(vector<Rect> faces) {

    Mat img_mosaic;


    for (int i = 0; i < faces.size(); i++) {
        Point str(faces[i].x + 1, faces[i].y + 1);
        Point end(faces[i].x + faces[i].width, faces[i].y + faces[i].height);
        img_mosaic = img(Rect(str, end));

        if (Pos == 2) {  //전체 모자이크
            Mat img_temp;
            resize(img_mosaic, img_temp, Size(img_mosaic.rows * 0.1, img_mosaic.cols * 0.1));
            resize(img_temp, img_mosaic, Size(img_mosaic.rows, img_mosaic.cols));
        }
        if (Pos == 1) { //나빼고 다
            if (i == I)
                continue;
            Mat img_temp;
            resize(img_mosaic, img_temp, Size(img_mosaic.rows * 0.1, img_mosaic.cols * 0.1));
            resize(img_temp, img_mosaic, Size(img_mosaic.rows, img_mosaic.cols));
            //findPerson();
        }

        if (Pos == 0) { //선택 모자이크
            if (ptOld.x > str.x && ptOld.x < end.x) {
                if (ptOld.y > str.y && ptOld.y < end.y) {
                    Mat img_temp;
                    resize(img_mosaic, img_temp, Size(img_mosaic.rows * 0.15, img_mosaic.cols * 0.15));
                    resize(img_temp, img_mosaic, Size(img_mosaic.rows, img_mosaic.cols));
                }
            }

        }

    }

    imshow("img", img);

    waitKey(0);
    destroyAllWindows();
}




void on_mouse(int event, int x, int y, int flags, void*)
{
    if (event == EVENT_LBUTTONDOWN) {
        ptOld = Point(x, y);
        cout << "LBUTTONDOWN: " << x << ", " << y << endl;
        if (Pos == 0)
            drawMosaic(faces);
    }
        
    if (event == EVENT_RBUTTONDOWN) {
        if (cnt < 2) {
            ptr[cnt] = Point(x, y);
            cout << "RBUTTONDOWN: " << x << ", " << y << endl;
            cnt++;
        }
        if (cnt == 2) {
            rectangle(img, ptr[0], ptr[1], Scalar(0, 0, 255), 1);
            Mat img2 = img(Rect(ptr[0], ptr[1]));
            medianBlur(img2, img2, 7);

            cnt = 0;
            imshow("img", img);
        }
    }

}



void on_AllMosaic(int pos, void* userdata) {
    if (pos == 0) {
        Pos = pos;
        detect_face();
        imshow("img", img);
        waitKey(0);
    }
    if (pos == 1) {
        Pos = pos;
        detect_face();
        imshow("img", img);
        waitKey(0);

        findPerson();              
    }
    if (pos == 2) {
        Pos = pos;
        drawMosaic(faces);
    }



}



Ptr<SVM> train_hog_svm(const HOGDescriptor& hog)
{

    Mat train = imread("face.png", IMREAD_GRAYSCALE);
    if (train.empty()) {
        cerr << "Image load failed!" << endl;
        return 0;
    }


    Mat train_hog, labels;

    for (int j = 0; j < 15; j++) {
        for (int i = 0; i < 10; i++) {
            Mat roi = train(Rect(i * 100, j * 100, 100, 100));

            vector<float> desc;
            hog.compute(roi, desc);

            Mat desc_mat(desc);
            train_hog.push_back(desc_mat.t());
            labels.push_back(j / 3);
        }
    }

    Ptr<SVM> svm = SVM::create();
    svm->setType(SVM::Types::C_SVC);
    svm->setKernel(SVM::KernelTypes::RBF);
    svm->setC(62.5);
    svm->setGamma(0.00015);
    svm->train(train_hog, ROW_SAMPLE, labels);

    //svm->trainAuto(train_hog, ROW_SAMPLE, labels);
    //cout << "C : " << svm->getC() << endl;
    //cout << "Gamma :" << svm->getGamma() << endl;

    return svm;

}




void findPerson() {

    //release mode
#if _DEBUG
    cout << "svmdigit.exe should be built as Relase mode!" << endl;
    return 0;
#endif

    HOGDescriptor hog(Size(100, 100), Size(10, 10), Size(5, 5), Size(5, 5), 9);
    Ptr<SVM> svm = train_hog_svm(hog);

    if (svm.empty()) {
        cerr << "Training failed!" << endl;
    }

    while (true) {
        int c = waitKey();

        if (c == 27) {
            break;
        }
        else if (c == ' ') {
            cout << "enter k or n or p : " << endl;

            char C;
            cin >> C;

            for (int i = 0; i < faces.size(); i++) {
                Point str(faces[i].x, faces[i].y);
                Point end(faces[i].x + faces[i].width, faces[i].y + faces[i].height);

                Mat test = img(Rect(str, end));

                Mat img_resize;
                resize(test, img_resize, Size(100, 100), 0, 0, INTER_AREA);

                vector<float> desc;
                hog.compute(img_resize, desc);

                Mat desc_mat(desc);
                int res = cvRound(svm->predict(desc_mat.t()));
                int cnt1=0, cnt2=0, cnt3 =0;


                if (C == 75 || C == 107) { //시현
                    if (res == 1) {
                        cnt1++;
                        if (cnt1 == 1) {
                            cout << res << endl;
                            I = i;
                            cnt1 = 0;

                            drawMosaic(faces);
                        }
                          
                        
                    }
                }
                else if (C == 78 || C == 110) { //연주
                    if (res == 0) {
                        cnt2++;
                        if (cnt2 == 1) {
                            cout << res << endl;
                            I = i;
                            cnt2 = 0;

                            drawMosaic(faces);
                        }
                       
                    }
                }
                else if (C == 80 || C == 112) { //이진
                    if (res == 2) {
                        cnt3++;
                        if (cnt3 == 1) {
                            cout << res << endl;
                            I = i;
                            cnt3 = 0;

                            drawMosaic(faces);
                        }
                        
                    }
                }



				//if (res == 0) {
				//	cout << res << endl;
				//	imshow("연주", test);
				//	waitKey(0);
				//}
				//else if (res == 1) {
				//	cout << res << endl;
				//	imshow("시현", test);
				//	waitKey(0);
				//}
				//else if (res == 2) {
				//	cout << res << endl;
				//	imshow("이진", test);
				//	waitKey(0);
				//}

            }
        }
    }
}

