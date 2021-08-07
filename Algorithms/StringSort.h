#pragma once
#include "Algorithms/String.h"

namespace Algorithm
{
	// MSD (Most Significant Digit)
	void MSD(Vector<const String*>& pOutFileList);
	void MSDStringSort(Vector<const String*>& pOutFileList, int low, int high, int d, Vector<const String*>& auxiliaryArray);
	int CharAt(const String& conetnt, int d) ;
	void InsertSort(Vector<const String*>& pOutFileList, int low, int high, int d);
}
