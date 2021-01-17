#ifndef GRAPH_H
#define GRAPH_H

#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include<vector>

using namespace cv;

class graph
{
    public:
        graph();
        virtual ~graph();

    protected:

protected:
        Point2f position;
};

class graph_A : public graph
{
private:
    Mat canny;
    Mat kernel = getStructuringElement(MORPH_RECT, Size(5, 5));
public:
    bool detect_A(Mat img);
};

#endif // GRAPH_H
