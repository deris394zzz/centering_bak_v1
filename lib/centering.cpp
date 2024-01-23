#include "centering.hpp"

using namespace std;
using namespace cv;

bak::bak(){
}

bak::bak(Color color, cv::Rect rect, int x, int y){
    this->color = color;
    this->rect = rect;
    this->posititon = cv::Point(x,y);
}
