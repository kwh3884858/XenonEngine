#include "StringSort.h"

namespace Algorithm
{
	int INSERT_SORT_THRESHOLD = 2;
	enum {
		MAX_CHARACTER = 256,
		CHARACTER_OFFSET = 2,
		TOTAL_LENGTH = MAX_CHARACTER + CHARACTER_OFFSET
	};

	void MSD(Vector<const String*>& pOutFileList)
	{
		Vector<const String*> auxiliaryArray(pOutFileList);
		MSDStringSort(pOutFileList, 0, pOutFileList.Count() - 1, 0, auxiliaryArray);
	}

	void MSDStringSort(Vector<const String*>& pOutFileList, int low, int high, int d, Vector<const String*>& auxiliaryArray)
	{
		if (low + INSERT_SORT_THRESHOLD >= high)
		{
			InsertSort(pOutFileList, low, high, d);
		}
		else
		{
			int msdCharacterArray[TOTAL_LENGTH];
			memset(msdCharacterArray, 0, TOTAL_LENGTH * sizeof(int));
			for (int i = low; i <= high; i++)
			{
				const String& currentString = *pOutFileList[i];
				int index = CharAt(currentString, d);
				//-1 means empty, character from 0 ~ 255 => 1 is empty, character from 2 ~ 257, total 258
				msdCharacterArray[index + 2] ++;
			}
			for (int i = 1; i < TOTAL_LENGTH; i++)
			{
				// Avaliable value from 1 is empty and 2 ~ 257 => 0 is empty index, 1 ~ 256 become character index
				msdCharacterArray[i] += msdCharacterArray[i - 1]; 
			}
			for (int i = low; i <= high; i++)
			{
				const String& currentString = *pOutFileList[i];
				//0 ~ 256, 0 means empty, 1 means 0
				char character = CharAt(currentString, d) + 1; 
				int index = msdCharacterArray[(int)character]++;
				auxiliaryArray[index] = pOutFileList[i];
			}
			for (int i = low; i <= high; i++)
			{
				pOutFileList[i] = auxiliaryArray[i - low];
			}
			for (int i = 0; i < MAX_CHARACTER; i++)
			{
				MSDStringSort(pOutFileList, low + msdCharacterArray[i], low + msdCharacterArray[i + 1] - 1, d + 1, auxiliaryArray); // position of sub string array should start from the beginning position, low position, of array.
			}
		}
	}

	char CharAt(const String& conetnt, int d) 
	{
		return d < conetnt.Count() ? conetnt[d] : -1;
	}
	void InsertSort(Vector<const String*>& pOutFileList, int low, int high, int d)
	{
		for (int i = low + 1; i <= high; i++)
		{
			for (int j = i; j > low && (*pOutFileList[j])[d] < (*pOutFileList[j - 1])[d]; j--)
			{
				const String* tmp = pOutFileList[j];
				pOutFileList[j] = pOutFileList[j - 1];
				pOutFileList[j - 1] = tmp;
			}
		}
	}
}