#pragma once
#include "stdafx.h"

class DetectedResult
{
public:
	DetectedResult();
	~DetectedResult();

public:
	// 以下类型要注意是否包含指针
	ASF_FaceFeature feature;
	ASF_AgeInfo ageInfo;
	ASF_GenderInfo genderInfo;
	ASF_LivenessInfo livenessInfo;
	// ...

	// 当前人脸的位置
	// faceRect[0],faceRect[1]: (x1,y1)
	// faceRect[2],faceRect[3]: (x2,y2)
	// (x1,y1)───────────┐
	//    │              │
	//    │              │
	//    │              │
	//    └───────────(x2,y2) 
	MInt32 faceRect[4];
	
	
	// 当前人脸的置信度
	MFloat confidenceLevel;

	std::string pathInPreload;

	int indexInPreload;

	bool identifiable;
};

