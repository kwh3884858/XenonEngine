#pragma once
#include "Algorithms/String.h"

namespace Algorithm
{
	// MSD (Most Significant Digit)
	void MSD(Vector<String>& pOutFileList);
	void MSDStringSort(Vector<String>& pOutFileList, int low, int high, int d, Vector<String>& auxiliaryArray);
	void InsertSort(Vector<String>& pOutFileList, int low, int high, int d);
}
