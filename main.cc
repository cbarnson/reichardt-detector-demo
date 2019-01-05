#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iomanip>
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/tracking.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <vector>
using namespace cv;

double THRESH = 0.05;  // [0-1] lower = more noise
const Scalar green = Scalar(0, 255, 0);
const Scalar red = Scalar(0, 0, 255);

void Reichardt(int row, int col, const Mat &grayPrev, const Mat &grayCurr,
               Mat &outputMat);
double crossCorrelation(uchar A1, uchar B1, uchar A2, uchar B2);

// if an integer command argument is provided, and the integer is between 0 and
// 100, THRESH will be changed to match argv[1] / 100.0
int main(int argc, char *argv[]) {
  if (argc == 2) {
    double newThresh = std::atof(argv[1]);
    while (newThresh > 100 || newThresh < 0) {
      std::cout
          << "Invalid threshold, please enter a value between 0 and 100: ";
      std::string str;
      std::cin >> str;
      newThresh = std::stod(str);
    }
    THRESH = newThresh / 100.0;
    std::cout << "Threshold set to " << THRESH << std::endl;
    std::cout << "==========================" << std::endl;
  }

  std::cout << "Press 'm' to toggle motion tracking" << std::endl;
  std::cout << "Press 'ESC' to exit" << std::endl;

  Mat frame, gray1, gray2;
  // flag to enable/disable motion tracking --- press 'm' to toggle
  bool motion = true;

  // webcam
  VideoCapture cap(0);
  if (!cap.isOpened()) {
    std::cout << "error opening camera\n";
    return -1;
  }
  // read in first frame
  cap.read(frame);
  cvtColor(frame, gray1, COLOR_BGR2GRAY);

  // clone 3 channel BGR image to hori and vert
  // set constants for frame size
  const int ROWS = frame.rows;
  const int COLS = frame.cols;

  // setup windows
  namedWindow("frame", WINDOW_AUTOSIZE);
  moveWindow("frame", 0, 0);

  // MAIN LOOP
  for (;;) {
    // read in frame
    cap.read(frame);

    // grayscale, store in gray2
    cvtColor(frame, gray2, COLOR_BGR2GRAY);

    if (motion) {
      for (int i = 1; i < ROWS - 1; i++) {
        for (int j = 1; j < COLS - 1; j++) {
          Reichardt(i, j, gray1, gray2, frame);
        }
      }
    }
    imshow("frame", frame);
    gray2.copyTo(gray1);

    switch (waitKey(10)) {
      case 27:  // 'ESC'
        return 0;
      case 109:  // 'm'
        motion = !motion;
        break;
    }
  }
  // END OF MAIN LOOP

  return 0;
}

void Reichardt(int i, int j, const Mat &grayPrev, const Mat &grayCurr,
               Mat &outputMat) {
  double directions[4];

  // get intensities, cast to double and normalize
  uchar f1_o = grayPrev.at<uchar>(i, j);
  uchar f1_h = grayPrev.at<uchar>(i, j + 1);
  uchar f1_v = grayPrev.at<uchar>(i + 1, j);
  uchar f1_d1 = grayPrev.at<uchar>(i + 1, j + 1);
  uchar f1_d2 = grayPrev.at<uchar>(i - 1, j + 1);

  uchar f2_o = grayCurr.at<uchar>(i, j);
  uchar f2_h = grayCurr.at<uchar>(i, j + 1);
  uchar f2_v = grayCurr.at<uchar>(i + 1, j);
  uchar f2_d1 = grayCurr.at<uchar>(i + 1, j + 1);
  uchar f2_d2 = grayCurr.at<uchar>(i - 1, j + 1);

  directions[0] = crossCorrelation(f1_o, f1_h, f2_o, f2_h);
  directions[1] = crossCorrelation(f1_o, f1_v, f2_o, f2_v);
  directions[2] = crossCorrelation(f1_o, f1_d1, f2_o, f2_d1);  // down-right
  directions[3] = crossCorrelation(f1_o, f1_d2, f2_o, f2_d2);  // up-right

  // we only color in pixel (i, j) if there is movement
  // to avoid overriding in a non-intuitive way, consider the max of the 4
  // directions only
  auto it = std::max_element(
      directions, directions + 4,
      [](double a, double b) { return std::fabs(a) < std::fabs(b); });

  if (*it > THRESH) {
    cv::circle(outputMat, cv::Point2f(j, i), 1, green, -1);
  } else if (*it < -THRESH) {
    cv::circle(outputMat, cv::Point2f(j, i), 1, red, -1);
  }
}

double crossCorrelation(uchar A1, uchar B1, uchar A2, uchar B2) {
  double a1 = (double)A1 / 255.0;
  double b1 = (double)B1 / 255.0;
  double a2 = (double)A2 / 255.0;
  double b2 = (double)B2 / 255.0;

  double product1 = a1 * b2;
  double product2 = a2 * b1;
  return product1 - product2;
}
