#include<iostream>
#include<stdio.h>
#include<vector>
#include<io.h>
#include<direct.h>
#include<windows.h>
#include"opencv2/opencv.hpp"

//�㷨��Ҫ������ͷ�ļ�
#include"OcrliteCApi.h"

#define _SUBDIR		0x10   //Subdirectory

void getFiles(std::string path, std::vector<std::string>& files)
{
	//�ļ����
	long long hFile = 0;	//win10�û�������long long��win7��ʹ��long����
	//�ļ���Ϣ
	struct _finddata_t fileinfo;
	std::string p;
	if ((hFile = _findfirst(p.assign(path).append("\\*").c_str(), &fileinfo)) != -1)
	{
		do
		{
			//�����Ŀ¼������֮
			//������ǣ������б�
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
	//1.��ȡSDK�汾��
	int bufLen = 20;
	char* buf = (char*)malloc(bufLen);
	int errorCodeVersion = 0;
	OCR_GetVersion(buf, &bufLen, &errorCodeVersion);
	if (errorCodeVersion != OCR_OK)
	{
		std::cout << "��ȡ�汾��ʧ��" << std::endl;
		return 0;
	}
	std::cout << "version number: " << buf << std::endl;
	free(buf);

	//2.OCRģ�ͳ�ʼ��
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
		std::cout << "��ʼ��ʧ��" << std::endl;
		return 0;
	}
	else
		std::cout << "��ʼ���ɹ�" << std::endl;

	std::string imageFolderPath = "../images/";
	std::vector<std::string> files;
	getFiles(imageFolderPath, files);
	std::cout << "�ļ�������" << files.size() << std::endl;
	for (int i = 0; i < files.size(); i++)
	{
		std::string imagePath = files[i];
		if ((imagePath.find(".jpg") == std::string::npos) && (imagePath.find(".png") == std::string::npos))
			continue;
		std::cout << imagePath << std::endl;
		cv::Mat img = cv::imread(imagePath);

		//3.OCRʶ����ͼ��
		OcrImg ocrImg{ img.data, img.cols, img.rows, img.channels() };
		OcrParam ocrParam{ 0, 0, 0, 0, 0, 0, 0 };  //�ӿ��ڲ��Զ�ʹ��Ĭ��ֵ
		int errorCodeRun = OCR_OK;
		double ts = (double)cv::getTickCount();
		OcrResult* pOcrResult = OCR_Run(handle, &ocrImg, &ocrParam, &errorCodeRun);
		double te = (double)cv::getTickCount();
		std::cout << "OCRʶ���ʱ��" << (te - ts) / cv::getTickFrequency() * 1000 << "ms" << std::endl; //��λms
		if (errorCodeRun != OCR_OK)
		{
			std::cout << "OCRʶ������ʧ��" << std::endl;
			return 0;
		}
		else
			std::cout << "OCRʶ������ɹ�" << std::endl;

		//��ӡʶ����
		std::cout << "OCRʶ����" << std::endl;
		for (int i = 0; i < pOcrResult->textBlocks.size(); i++)
		{
			std::cout << Utf8ToGbk(pOcrResult->textBlocks[i].text.c_str()) << std::endl;
		}

		cv::imshow("Image", img);
		cv::waitKey(0);

		//4.OCR����ʶ�����ͷ�
		int errorCodeRelease = OCR_OK;
		OCR_ReleaseResult(pOcrResult, &errorCodeRelease);
		if (errorCodeRelease != OCR_OK)
		{
			std::cout << "OCR���ν���ͷ�ʧ��" << std::endl;
			return 0;
		}
	}

	//5.OCRģ���ͷ�
	int errorCodeDestroy = OCR_OK;
	OCR_Destroy(handle, &errorCodeDestroy);
	if (errorCodeDestroy != OCR_OK)
	{
		std::cout << "OCRģ���ͷ�ʧ��" << std::endl;
		return 0;
	}
}