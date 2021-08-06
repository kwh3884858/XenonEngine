#include "StringSort.h"

namespace Algorithm
{
	int INSERT_SORT_THRESHOLD = 2;
	enum {
		MAX_CHARACTER = 256,
		CHARACTER_OFFSET = 2,
		TOTAL_LENGTH = MAX_CHARACTER + CHARACTER_OFFSET
	};

	void MSD(Vector<String>& pOutFileList)
	{
		Vector<String> auxiliaryArray;
		MSDStringSort(pOutFileList, 0, pOutFileList.Count() - 1, 0, auxiliaryArray);
	}

	void MSDStringSort(Vector<String>& pOutFileList, int low, int high, int d, Vector<String>& auxiliaryArray)
	{
		if (low + INSERT_SORT_THRESHOLD >= high)
		{
			InsertSort(pOutFileList, low, high, d);
		}
		else
		{
			int m_msdCharacterArray[MAX_CHARACTER];
			memset(m_msdCharacterArray, 0, MAX_CHARACTER * sizeof(int));
			for (int i = low; i <= high; i++)
			{
				int index = pOutFileList[d][i];
				m_msdCharacterArray[index + 2] ++; //-1 ~ 255 => 1 ~ 257, total 258
			}
			for (int i = 1; i < TOTAL_LENGTH; i++)
			{
				m_msdCharacterArray[i] += m_msdCharacterArray[i - 1];
			}
			for (int i = low; i <= high; i++)
			{
				char character = pOutFileList[i][d] + 1; //0 ~ 256, 0 means empty, 1 means 0
				int index = m_msdCharacterArray[(int)character]++;
				auxiliaryArray[index] = pOutFileList[i];
			}
			for (int i = low; i <= high; i++)
			{
				pOutFileList[i] = auxiliaryArray[i - low];
			}
			for (int i = 0; i < MAX_CHARACTER; i++)
			{
				MSDStringSort(pOutFileList, low + m_msdCharacterArray[i], low + m_msdCharacterArray[i + 1] - 1, d + 1, auxiliaryArray); // position of sub string array should start from the beginning position, low position, of array.
			}
		}
	}

	void InsertSort(Vector<String>& pOutFileList, int low, int high, int d)
	{
		assert(d >= 0 && d < pOutFileList[low].Count());
		for (int i = low + 1; i <= high; i++)
		{
			for (int j = i; j > low && pOutFileList[j][d] < pOutFileList[j - 1][d]; j--)
			{
				Swap(pOutFileList[j], pOutFileList[j - 1]);
			}
		}
	}
}