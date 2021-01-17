#include <iostream>
#include "graph.h"

using namespace cv;
using namespace std;

int main()
{
    /*
    Mat img;
    double exposure = 0;
    double brightness = 0;        //亮度
    double fps = 0;

    VideoCapture cap(2);
    cap.set(CV_CAP_PROP_EXPOSURE, 0);
    cap.set(CV_CAP_PROP_BRIGHTNESS, 0);
    //cap.set(CV_CAP_PROP_FPS, 60);//帧率 帧/秒

    graph_A detect_A;
    while(1)
    {
        cap>>img;
       imshow("test",img);
       fps = cap.get(CV_CAP_PROP_FPS);
    cout << "相机帧率：" << fps  <<endl;

        if(detect_A.detect_A(img))
    {

    cout<<"detected!"<<std::endl;
    cout<<"###############"<<std::endl;

    }
    waitKey(1);
    }
     */
    graph_A detect_A;
    Mat img=imread("/home/zwt/CLionProjects/ATS-X_vision/1.jpg");
    namedWindow("test", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    imshow("test", img);
    if(detect_A.detect_A(img))
    {

    std::cout<<"detected!"<<std::endl;
    }
    waitKey(0);
    return 0;

}
