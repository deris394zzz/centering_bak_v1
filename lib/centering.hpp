#ifndef CENTERING_HPP
#define CENTERING_HPP

#include <opencv2/core/types.hpp>

enum Color {BLUE, RED};

class bak
{
    public:
        Color color; 
        cv::Point posititon;
        cv::Rect rect;

        bak();
        bak(Color color, cv::Rect rect, int x, int y);
};


struct command {
    int x_cmd;
    int y_cmd;
    int drop_ball;

};

#endif