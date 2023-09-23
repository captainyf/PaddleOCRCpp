#ifndef __OCR_STRUCT_H__
#define __OCR_STRUCT_H__

#include "opencv2/core.hpp"
#include <vector>

struct ScaleParam {
    int srcWidth;
    int srcHeight;
    int dstWidth;
    int dstHeight;
    float ratioWidth;
    float ratioHeight;
};

struct TextBox {
    std::vector<cv::Point> boxPoint;
    float score;
};

struct Angle {
    int index;
    float score;
    double time;
};

struct TextLine {
    std::string text;
    std::vector<float> charScores;
    double time;
};

struct TextBlock {
    std::vector<cv::Point> boxPoint;
    float boxScore;
    int angleIndex;
    float angleScore;
    double angleTime;
    std::string text;
    std::vector<float> charScores;
    double crnnTime;
    double blockTime;
};

struct OcrResult {
    double dbNetTime;
    std::vector<TextBlock> textBlocks;
    cv::Mat boxImg;
    double fullTime;
    std::string strRes;
};

//@brief OCR输入图像
struct OcrImg
{
    unsigned char* pData;			//图像数据
    int width;						//宽度
    int height;						//高度
    int channel;					//通道数
};

typedef void* OcrHandle;			//OCR模型handle

//@brief OCR模型配置参数
struct OcrParam
{
    int padding;					//图像周围补0的像素数
    int maxSideLen;					//输入图像resize的最大边长，数值范围1至原始图像最大边长
    float boxScoreThresh;			//文本检测得分阈值，数值范围0至1.0f
    float boxThresh;				//分割图二值化像素阈值，数值范围0至1.0f
    float unClipRatio;				//轮廓区域裁切率
    int doAngle;					//使用角度检测，0表示不使用，其他值表示使用
    int mostAngle;					//              0表示不使用，其他值表示使用
    float textBlockThresh;			//字符串识别阈值，数值范围0至1.0f
};

#endif //__OCR_STRUCT_H__
