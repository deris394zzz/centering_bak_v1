#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <iostream>
#include "lib/centering.hpp"
#include "rclcpp/rclcpp.hpp"
#include "msg_bank/msg/motion_command.hpp" 

using namespace std;
using namespace cv;


#define center_window_x 320
#define center_window_y 320

cv::Scalar redLow = Scalar(170, 140, 160);
cv::Scalar redHigh = Scalar(180, 255, 255);
cv::Scalar blueLow = Scalar(100, 150, 150);
cv::Scalar blueHigh = Scalar(140, 255, 255);


std::vector<bak> baks; //baks = bak tapi banyak xixiix


//ros2
class bakcentering : public rclcpp::Node {
public:
    msg_bank::msg::MotionCommand cmd;
    bakcentering() : Node("centering") {
        // Inisialisasi publisher
        publisher = this->create_publisher<msg_bank::msg::MotionCommand>("motion_command", 10);
        timer_ = this->create_wall_timer(5ms, std::bind(&bakcentering::timer_callback, this));
    }
private:
    void timer_callback()
  {
    cv::Mat frame, hsv, gray, maskHSV, resultHSV, imgThresholded, image;
    namedWindow("original");
    cv::VideoCapture cap(0);
    resizeWindow("original", 640, 640);

    if (!cap.isOpened()) {
        cout << "cannot open camera";
        return;
    }

    // Capture frame
    cap >> frame;

    // Check if the frame is empty
    if (frame.empty()) {
        cout << "captured frame is empty";
        return;
    }

    cv::Mat target;
    frame.copyTo(target);

    cv::Mat background;
    target.copyTo(background);

    cv::cvtColor(target, target, cv::COLOR_BGR2HSV);

    
    //find blue baks
    cv::Mat mask;
    cv::inRange(target, blueLow, blueHigh, mask);
    std::vector<std::vector<cv::Point> > contours;
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
 
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect boundRect = boundingRect(contours[i]);
        if (boundRect.area() > 350 && (boundRect.width < 100 || boundRect.height < 100)) {
            int x_mid = boundRect.x + boundRect.width / 2;
            int y_mid = boundRect.y + boundRect.height / 2;
            baks.emplace_back(BLUE, boundRect, x_mid, y_mid);
            rectangle(background, baks[i].rect.tl(), baks[i].rect.br(), CV_RGB(0, 0, 255), 2);
            int perbedaan_x = x_mid - center_window_x;
            int perbedaan_y = y_mid - center_window_y;
            int x_threshold = 25;
            int y_threshold = 25;
            if(perbedaan_x > x_threshold || perbedaan_y > y_threshold){
            this->cmd.x_cmd = perbedaan_x;
            this->cmd.y_cmd = perbedaan_y;
            }else{
            this->cmd.drop_ball = 1;
            
        }
        publisher->publish(cmd);
    }
    };

    //find red baks
    cv::inRange(target, redLow, redHigh, mask);
    cv::findContours(mask, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
 
    for (size_t i = 0; i < contours.size(); i++)
    {
        cv::Rect boundRect = boundingRect(contours[i]);
        if (boundRect.area() > 350 && (boundRect.width < 70 || boundRect.height < 70)) {
            int x_mid = boundRect.x + boundRect.width / 2;
            int y_mid = boundRect.y + boundRect.height / 2;
            baks.emplace_back(RED, boundRect, x_mid, y_mid);
            rectangle(background, baks[i].rect.tl(), baks[i].rect.br(), CV_RGB(255, 0, 0), 2);
            int perbedaan_x = x_mid - center_window_x;
            int perbedaan_y = y_mid - center_window_y;
            int x_threshold = 25;
            int y_threshold = 25;
            if(perbedaan_x > x_threshold || perbedaan_y > y_threshold){
            this->cmd.x_cmd = perbedaan_x;
            this->cmd.y_cmd = perbedaan_y;
            }else{
            this->cmd.drop_ball = 1;
            }
            publisher->publish(cmd);
        }   
    };

    imshow("contours", background);
    waitKey(25);
  };
  rclcpp::TimerBase::SharedPtr timer_;
  rclcpp::Publisher<msg_bank::msg::MotionCommand>::SharedPtr publisher;
};


int main(int argc, char *argv[])
{
  rclcpp::init(argc, argv);
  rclcpp::spin(std::make_shared<bakcentering>());
  rclcpp::shutdown();


return 0;
}
