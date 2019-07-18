#pragma once
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <opencv2\opencv.hpp>

#include "arcsoft_face_sdk.h"
#include "amcomdef.h"
#include "asvloffscreen.h"
#include "merror.h"
#include "dirent.h"
#include "DetectedResult.h"
#include "PreloadInfo.h"
#include "json.h"

/*
*	FACEDEBUG 如果开启
*	将在终端输出调试信息
*
*	OTHERINFO 如果开启
*	将检测诸如年龄，是否为活体，性别等信息
*
*/
// #define FACEDEBUG 
// #define OTHERINFO

using namespace cv;
using namespace std;