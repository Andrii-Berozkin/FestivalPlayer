// Copyright Andrii Berozkin, All Rights Reserved.



#pragma once

#include <opencv2/core/core.hpp>



class UImageResolverHelper
{
public:

	static bool Compare(const int& X, const int& Y, const int& Tolerance = 0)
	{
		return abs(X - Y) <= Tolerance;
	}

	static bool CompareRGB(const cv::Point3_<uint8_t>& X, const cv::Point3_<uint8_t>& Y, const int Tolerance = 0)
	{
		return Compare(X.x, Y.x, Tolerance) && 
			Compare(X.y, Y.y, Tolerance) && 
			Compare(X.z, Y.z, Tolerance);
	}

	static bool FindColorInMat(const cv::Mat& SourceMat, const cv::Point3_<uint8_t>& TargetColor, const int& Tolerance = 0)
	{
		for (int x = 0; x < SourceMat.rows; x++)
		{
			for (int y = 0; y < SourceMat.cols; y++)
			{
				if (CompareRGB(SourceMat.at<cv::Point3_<uint8_t>>(x, y), TargetColor, Tolerance))
				{
					return true;
				}
			}
		}

		return false;
	}

	static bool FindColorInMatArr(const cv::Mat& SourceMat, const std::vector<cv::Point3_<uint8_t>>& TargetColorsArr, const std::vector<int>& IndividualTolerance)
	{
		if (TargetColorsArr.size() != IndividualTolerance.size()) { printf("Size of TargetColors dosen`t matcht size of IndividualTolerance"); return false; }

		const int iMax = TargetColorsArr.size();

		for (int x = 0; x < SourceMat.rows; x++)
		{
			for (int y = 0; y < SourceMat.cols; y++)
			{
				for (int i = 0; i < iMax; i++)
				{
					if (CompareRGB(SourceMat.at<cv::Point3_<uint8_t>>(x, y), TargetColorsArr[i], IndividualTolerance[i]))
					{
						return true;
					}
				}
			}
		}

		return false;
	}
};