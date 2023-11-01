#include<iostream>
#include<stdio.h>
#include<vector>
#include<io.h>
#include<direct.h>
#include<windows.h>
#include"opencv2/opencv.hpp"

//算法需要包含的头文件
#include"OcrliteCApi.h"

#define _SUBDIR		0x10   //Subdirectory

void getFiles(std::string path, std::vector<std::string>& files)
{
	//文件句柄
	long long hFile = 0;	//win10用户必须用long long，win7可使用long类型
	//文件信息
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//如果是目录，迭代之
			//如果不是，加入列表
			if ((fileinfo.attrib & _SUBDIR))
			{
				if (strcmp(fileinfo.name, ".") != 0 && strcmp(fileinfo.name, "..") != 0)
					getFiles(p.assign(path).append("\\").append(fileinfo.name), files);
			}
			else
				files.push_back(p.assign(path).append("\\").append(fileinfo.name));
		} while (_findnext(hFile, &fileinfo) == 0);
		_findclose(hFile);
	}
}

std::string Utf8ToGbk(const char* src_str)
{
	int len = MultiByteToWideChar(CP_UTF8, 0, src_str, -1, NULL, 0);
	wchar_t* wszGBK = new wchar_t[len + 1];
	memset(wszGBK, 0, len * 2 + 2);
	MultiByteToWideChar(CP_UTF8, 0, src_str, -1, wszGBK, len);
	len = WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, NULL, 0, NULL, NULL);
	char* szGBK = new char[len + 1];
	memset(szGBK, 0, len + 1);
	WideCharToMultiByte(CP_ACP, 0, wszGBK, -1, szGBK, len, NULL, NULL);
	std::string strTemp(szGBK);
	if (wszGBK) delete[] wszGBK;
	if (szGBK) delete[] szGBK;
	return strTemp;
}

int main(int argc, char** argv)
{
	//1.获取SDK版本号
	int bufLen = 20;
	char* buf = (char*)malloc(bufLen);
	int errorCodeVersion = 0;
	OCR_GetVersion(buf, &bufLen, &errorCodeVersion);
	if (errorCodeVersion != OCR_OK)
	{
		std::cout << "获取版本号失败" << std::endl;
		return 0;
	}
	std::cout << "version number: " << buf << std::endl;
	free(buf);

	//2.OCR模型初始化
	std::string modelFolderPath = "../PPOCR/models/";
	std::string detModelPath = modelFolderPath + "ch_PP-OCRv3_det";
	std::string clsModelPath = modelFolderPath + "cls-sim-op";
	std::string recModelPath = modelFolderPath + "ch_PP-OCRv3_rec";
	std::string keyFilePath = modelFolderPath + "paddleocr_keys.txt";
	int nThreads = 1;
	int gpuIndex = 0;
	int errorCodeInit = OCR_OK;

	OcrHandle handle = OCR_Create(detModelPath.c_str(), clsModelPath.c_str(), recModelPath.c_str(),
		keyFilePath.c_str(), nThreads, gpuIndex, &errorCodeInit);
	if (errorCodeInit != OCR_OK)
	{
		std::cout << "初始化失败" << std::endl;
		return 0;
	}
	else
		std::cout << "初始化成功" << std::endl;

	std::string imageFolderPath = "../images/";
	std::vector<std::string> files;
	getFiles(imageFolderPath, files);
	std::cout << "文件总数：" << files.size() << std::endl;
	for (int i = 0; i < files.size(); i++)
	{
		std::string imagePath = files[i];
		if ((imagePath.find(".jpg") == std::string::npos) && (imagePath.find(".png") == std::string::npos))
			continue;
		std::cout << imagePath << std::endl;
		cv::Mat img = cv::imread(imagePath);

		//3.OCR识别单张图像
		OcrImg ocrImg{ img.data, img.cols, img.rows, img.channels() };
		OcrParam ocrParam{ 0, 0, 0, 0, 0, 0, 0 };  //接口内部自动使用默认值
		int errorCodeRun = OCR_OK;
		double ts = (double)cv::getTickCount();
		OcrResult* pOcrResult = OCR_Run(handle, &ocrImg, &ocrParam, &errorCodeRun);
		double te = (double)cv::getTickCount();
		std::cout << "OCR识别耗时：" << (te - ts) / cv::getTickFrequency() * 1000 << "ms" << std::endl; //单位ms
		if (errorCodeRun != OCR_OK)
		{
			std::cout << "OCR识别推理失败" << std::endl;
			return 0;
		}
		else
			std::cout << "OCR识别推理成功" << std::endl;

		//打印识别结果
		std::cout << "OCR识别结果" << std::endl;
		for (int i = 0; i < pOcrResult->textBlocks.size(); i++)
		{
			std::cout << Utf8ToGbk(pOcrResult->textBlocks[i].text.c_str()) << std::endl;
		}

		cv::imshow("Image", img);
		cv::waitKey(0);

		//4.OCR单次识别结果释放
		int errorCodeRelease = OCR_OK;
		OCR_ReleaseResult(pOcrResult, &errorCodeRelease);
		if (errorCodeRelease != OCR_OK)
		{
			std::cout << "OCR单次结果释放失败" << std::endl;
			return 0;
		}
	}

	//5.OCR模型释放
	int errorCodeDestroy = OCR_OK;
	OCR_Destroy(handle, &errorCodeDestroy);
	if (errorCodeDestroy != OCR_OK)
	{
		std::cout << "OCR模型释放失败" << std::endl;
		return 0;
	}
}