#pragma once
#include "stdafx.h"

class FaceDete
{
public:
	FaceDete();

	~FaceDete();

	void SetAPPID(const char appid[]);

	void SetSDKKey(const char sdkkey[]);

	void SetPreloadPath(string path);

	void SetConfLevel(MFloat Level);

	size_t GetRestrSize();

	/*
	* @ 返回值
	*	0 成功
	*	1 失败
	*/
	int Activation();

	/*
	* @ 返回值
	*	0 成功
	*	1 失败
	*/

	int InitEngine(); 

	/*
	* @ 返回值
	*	0 成功
	*	1 失败
	*/
	int UninitEngine();

	/*
	*
	*	typedef struct {
	*		MPChar Version;							// 版本号
	*		MPChar BuildDate;						// 构建日期
	*		MPChar CopyRight;						// Copyright
	*	}ASF_VERSION;
	*
	*/
	const ASF_VERSION* GetVersion();

	/*
	* @ 返回值
	*	>0 加载成功的数量
	*	0 加载失败，有可能是路径错误
	*/
	int Loadregface();

	/*
	* @ 参数
	*	image [可能]包含人脸的图像
	*	detectedResult 识别的结果
	* @ 无返回
	*/
	int DetectFaces(Mat &image, Json::Value &detectedResult);

	// 以下的函数暂用于debug/test测试
	void DrawRetangle(Mat& frame, MInt32 faceRect[4]);

private:

	/*
	* 从图像中提取人脸特征值，只提取单张人脸
	*
	* @ 参数
	*	DetectedResult
	* @ 返回值
	*  1 识别失败
	*  0  识别成功
	*/
	void GetFeaturefromImage(Mat & image, ASF_FaceFeature &feature);

	/*
	* 当前识别结果的人来数据进行比对,即识别部分
	* 
	* @ 参数
	*	DetectedResult
	* @ 返回值
	*  1 识别失败
	*  0  识别成功
	*/
	int CompareFeature(DetectedResult& result);

private:
	MRESULT res;
	MHandle handle;
	string preloadPath;
	MFloat threshold_confidenceLevel;
	/*
	*	struct PreloadInfo {
	*		eature feature;
	*  		std::string filename;
	*	};
	*/
	vector <PreloadInfo> preLoadVec;

	Json::Value peopleInfo;

	char * APPID;
	char * SDKKey;
};

