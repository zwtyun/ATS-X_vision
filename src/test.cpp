#include <iostream>
#include "graph.h"

using namespace cv;
using namespace std;


void unevenLightCompensate(Mat &image, int blockSize)
{
    if (image.channels() == 3) cvtColor(image, image, 7);
    double average = mean(image)[0];
    int rows_new = ceil(double(image.rows) / double(blockSize));
    int cols_new = ceil(double(image.cols) / double(blockSize));
    Mat blockImage;
    blockImage = Mat::zeros(rows_new, cols_new, CV_32FC1);
    for (int i = 0; i < rows_new; i++)
    {
        for (int j = 0; j < cols_new; j++)
        {
            int rowmin = i*blockSize;
            int rowmax = (i + 1)*blockSize;
            if (rowmax > image.rows) rowmax = image.rows;
            int colmin = j*blockSize;
            int colmax = (j + 1)*blockSize;
            if (colmax > image.cols) colmax = image.cols;
            Mat imageROI = image(Range(rowmin, rowmax), Range(colmin, colmax));
            double temaver = mean(imageROI)[0];
            blockImage.at<float>(i, j) = temaver;
        }
    }
    blockImage = blockImage - average;
    Mat blockImage2;
    resize(blockImage, blockImage2, image.size(), (0, 0), (0, 0), INTER_CUBIC);
    Mat image2;
    image.convertTo(image2, CV_32FC1);
    Mat dst = image2 - blockImage2;
    dst.convertTo(image, CV_8UC1);
}

int main()
{

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
        unevenLightCompensate(img, 32);
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

    /*
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
*/
}
