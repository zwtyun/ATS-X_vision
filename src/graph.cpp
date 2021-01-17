#include "graph.h"

using namespace std;
using namespace cv;

const double PI =3.1415926;         //圆周率
const double min_rate=0.9;      //最小面积比率，可调整
const float max_dis_2=5.0;            //最大同心圆的圆心的距离平方，可调整
const int min_circle_num=2;         //最小同心圆数量

inline float distance_2(Point2f p1,Point2f p2)      //返回两点之间距离的平方
{
    return (p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y);
}

graph::graph()
{
    //ctor
}

graph::~graph()
{
    //dtor
}

bool graph_A::detect_A(Mat img)
{
    if(img.empty())
    {
        cout<<"image reading error!!!"<<endl;
    }

    //边缘检测
    //cvtColor(img, img, CV_RGB2GRAY);
    //namedWindow("y", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    //imshow("gray", img);

    threshold(img, img, 40,255,THRESH_BINARY);  //260帧率相机 40    30帧率相机 0
    namedWindow("threshold", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    imshow("threshold", img);

    Canny(img,canny,80,160,3,false);
    namedWindow("canny1", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    imshow("canny1", canny);

    dilate(canny,canny,kernel);
    namedWindow("dilate", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    imshow("dilate", canny);

    //erode(canny,canny,kernel);
    //namedWindow("erode", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    //imshow("erode", canny);

    GaussianBlur(canny, canny, Size(5, 5), 1, 1);
    namedWindow("blur", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    imshow("blur", canny);

    //轮廓发现
    vector<vector<Point>> contours;
    vector<Vec4i> hierarchy;
    findContours(canny,contours,hierarchy,RETR_LIST, CHAIN_APPROX_SIMPLE, Point());
    //drawContours(img, contours, -1, Scalar(255, 0, 0), 8);
    //namedWindow("findcontours", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
    //imshow("findcontours", img);


    //轮廓筛选
    vector<Point2f> center;
    vector<float> radius;
    center.reserve(contours.size());
    radius.reserve(contours.size());

    auto rate=0.0,max_radius=0.0;
    auto max_t=0;
    for(auto t=0;t<contours.size();t++)
    {
        minEnclosingCircle(contours[t],center[t],radius[t]);
        rate=contourArea(contours[t])/(PI*radius[t]*radius[t]);
        if (rate<min_rate)      //排除非圆轮廓
        {
            radius[t]=0.0;          //被排除的轮廓半径标记为0
            continue;
        }
        if (radius[t]>max_radius)       //计算最大圆
        {
            max_radius=radius[t];
            max_t=t;
        }
        circle(img, center[t], radius[t], Scalar(0, 0, 255), 1);
        namedWindow("circle", CV_WINDOW_NORMAL);//CV_WINDOW_NORMAL就是0
        imshow("circle", img);

        cout << "The " << t + 1 << " contours: x = " << center[t].x << ", y = " << center[t].y << ", r = " << radius[t] << endl;
    }
    //寻找同心圆
    auto num=1;
    auto sum_x=0.0,sum_y=0.0;
    //cout<<contours.size()<<", "<<radius[0]<<endl;
   for(auto t = 0; t < contours.size(); t++)
    {
        if (radius[t] > 0.0 && distance_2(center[max_t],center[t])<=max_dis_2)      //圆心与最大圆圆心的距离小于允许最大值，判定为同心圆
        {
            sum_x+=center[t].x;
            sum_y+=center[t].y;
            num++;
            //cout<<"x = "<<center[t].x<<", y = "<<center[t].y<<endl;
        }
    }

    cout << num << endl;

    if (num>min_circle_num)      //检测到的同心圆数目大于3，检测到目标
    {
        position.x=sum_x / num;
        position.y=sum_y / num;
        //cout<<num<<"x = "<<position.x<<"y = "<<position.y<<endl;
        return true;
    }
    else
    {
        return false;
    }
}
