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

//@brief OCR����ͼ��
struct OcrImg
{
    unsigned char* pData;			//ͼ������
    int width;						//���
    int height;						//�߶�
    int channel;					//ͨ����
};

typedef void* OcrHandle;			//OCRģ��handle

//@brief OCRģ�����ò���
struct OcrParam
{
    int padding;					//ͼ����Χ��0��������
    int maxSideLen;					//����ͼ��resize�����߳�����ֵ��Χ1��ԭʼͼ�����߳�
    float boxScoreThresh;			//�ı����÷���ֵ����ֵ��Χ0��1.0f
    float boxThresh;				//�ָ�ͼ��ֵ��������ֵ����ֵ��Χ0��1.0f
    float unClipRatio;				//�������������
    int doAngle;					//ʹ�ýǶȼ�⣬0��ʾ��ʹ�ã�����ֵ��ʾʹ��
    int mostAngle;					//              0��ʾ��ʹ�ã�����ֵ��ʾʹ��
    float textBlockThresh;			//�ַ���ʶ����ֵ����ֵ��Χ0��1.0f
};

#endif //__OCR_STRUCT_H__
