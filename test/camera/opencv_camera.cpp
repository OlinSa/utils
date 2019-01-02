#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

using namespace cv;
using namespace std;

int main(int argc, char *argv[])
{
    VideoCapture capture(0);
    if(!capture.isOpened()) {
        cout<<"open camera failed"<<endl;
        return -1;
    }
    Mat frame;
    while(capture.isOpened()) {
        capture >> frame;
        if(frame.empty())
            break;
        imshow("capture", frame);
        char key = static_cast<char>(cv::waitKey(10));
        if(key == 27) //esc
            break;
    }
    capture.release();
    destroyAllWindows();
    return 0;

}