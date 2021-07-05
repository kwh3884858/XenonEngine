#include "Windows/File/FileReader.h"
#include "Algorithms/Vector.h"
#include "CrossPlatform/XenonFile.h"
#include "Algorithms/StreamingVector.h"

#include <fstream>
#include <iostream> // For std::cerr, std::ios::beg
#include <assert.h>
#include <windows.h>

namespace File
{
    using Algorithm::Vector;
    using CrossPlatform::XenonFile;
    using Algorithm::StreamingVector;

    XenonFile*const FileReader::ReadFile(const Algorithm::String fileName) const
    {
        std::ifstream fins;
        XenonFile* xenonFile = new XenonFile;
        char fileNameA[MAX_PATH];
        fileName.CString(fileNameA);

        fins.open(fileNameA);

        // If it could not open the file then exit.
        if (fins.good())
        {
            fins >> std::noskipws;
            char charBuffer = '\0';

            while(true)
            {
                fins >> charBuffer;
                if (fins.eof())
                {
                    break;
                }
                if (charBuffer == XenonFile::CL) // CL, carriage return
                {
                    continue;
                }
                if (charBuffer == XenonFile::LF) //LF, line feed
                {
                    xenonFile->m_lineSize++;
                }
                xenonFile->m_content.Add(charBuffer);
            }
            fins.close();
        }
        else
        {
            assert(true == false);
            fins.close();
            printf("FIle Cannot Open!");
            std::cerr << "Error: " << strerror(errno);
            return nullptr;
        }

        xenonFile->m_fileName = fileName;
        return xenonFile;
    }

    Algorithm::StreamingVector<char>* FileReader::ReadStreamFile(const Algorithm::String fileName) const
    {
        std::ifstream fins;
        Algorithm::StreamingVector<char>* streamedFile = new Algorithm::StreamingVector<char>;
        char fileNameA[MAX_PATH];
        fileName.CString(fileNameA);

        fins.open(fileNameA, std::ios::in | std::ios::binary | std::ios::ate);

        // If it could not open the file then exit.
        if (fins.good())
        {
            fins >> std::noskipws;
            int fileSize = fins.tellg();
            char* memoryBlock = new char[fileSize];
            fins.seekg(0, std::ios::beg);
            fins.read(memoryBlock, fileSize);
            streamedFile->Replace(memoryBlock, fileSize);

            fins.close();
            delete memoryBlock;
            memoryBlock = nullptr;
        }
        else
        {
            assert(true == false);
            fins.close();
            printf("FIle Cannot Open!");
            std::cerr << "Error: " << strerror(errno);
            return nullptr;
        }

        return streamedFile;
    }

    bool FileReader::WriteFile(const Algorithm::String fileName, const Algorithm::StreamingVector<char>& stream) const
    {
        std::ofstream fouts;
        char fileNameA[MAX_PATH];
        fileName.CString(fileNameA);

        fouts.open(fileNameA, std::ios::out | std::ios::binary);
        if (fouts.good())
        {
            fouts.write(stream.Begin(), stream.Count());
            fouts.close();
        }
        else
        {
            assert(true == false);
            fouts.close();
            printf("FIle Cannot Open!");
            std::cerr << "Error: " << strerror(errno);
            return false;
        }

        return true;
    }

    Algorithm::String FileReader::GetApplicationPath() const
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return Algorithm::String(buffer);
    }

}

