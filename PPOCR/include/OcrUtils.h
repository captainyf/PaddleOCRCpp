#ifndef __OCR_UTILS_H__
#define __OCR_UTILS_H__

#include <opencv2/core.hpp>
#include "OcrStruct.h"

#include <sys/stat.h>
/*#define __ENABLE_CONSOLE__ true
#define Logger(format, ...) {\
  if(__ENABLE_CONSOLE__) printf(format,##__VA_ARGS__); \
}*/
template<class T>
inline T clamp(T x, T min, T max) {
    if (x > max)
        return max;
    if (x < min)
        return min;
    return x;
}

double getCurrentTime();

inline bool isFileExists(const std::string &name) {
    struct stat buffer;
    return (stat(name.c_str(), &buffer) == 0);
}

std::wstring strToWstr(std::string str);

ScaleParam getScaleParam(cv::Mat &src, const float scale);

ScaleParam getScaleParam(cv::Mat &src, const int targetSize);

std::vector<cv::Point2f> getBox(const cv::RotatedRect &rect);

int getThickness(cv::Mat &boxImg);

void drawTextBox(cv::Mat &boxImg, cv::RotatedRect &rect, int thickness);

void drawTextBox(cv::Mat &boxImg, const std::vector<cv::Point> &box, int thickness);

void drawTextBoxes(cv::Mat &boxImg, std::vector<TextBox> &textBoxes, int thickness);

cv::Mat matRotateClockWise180(cv::Mat src);

cv::Mat matRotateClockWise90(cv::Mat src);

cv::Mat getRotateCropImage(const cv::Mat &src, std::vector<cv::Point> box);

cv::Mat adjustTargetImg(cv::Mat &src, int dstWidth, int dstHeight);

std::vector<cv::Point2f> getMinBoxes(const cv::RotatedRect &boxRect, float &maxSideLen);

float boxScoreFast(const std::vector<cv::Point2f> &boxes, const cv::Mat &pred);

cv::RotatedRect unClip(std::vector<cv::Point2f> box, float unClipRatio);

std::vector<int> getAngleIndexes(std::vector<Angle> &angles);

void saveImg(cv::Mat &img, const char *imgPath);

std::string getSrcImgFilePath(const char *path, const char *imgName);

std::string getResultTxtFilePath(const char *path, const char *imgName);

std::string getResultImgFilePath(const char *path, const char *imgName);

std::string getDebugImgFilePath(const char *path, const char *imgName, int i, const char *tag);

void printGpuInfo();

#endif //__OCR_UTILS_H__
