#include "Windows/File/FileReader.h"
#include "Algorithms/Vector.h"
#include "CrossPlatform/XenonFile.h"

#include <fstream>
#include <iostream> // For std::cerr
#include <assert.h>
#include <windows.h>

namespace File
{
    using Algorithm::Vector;
    using CrossPlatform::XenonFile;

    XenonFile*const FileReader::ReadFile(const Algorithm::String fileName) const
    {
        std::ifstream fins;
        XenonFile* xenonFile = new XenonFile;
        char fileNameA[MAX_PATH];
        fileName.CString(fileNameA);

        fins.open(fileNameA);

        // If it could not open the file then exit.
        if (!fins.fail())
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
            printf("FIle Cannot Open!");
            std::cerr << "Error: " << strerror(errno);
            return nullptr;
        }
        fins.close();
        xenonFile->m_fileName = fileName;
        return xenonFile;
    }

    Algorithm::String FileReader::GetApplicationPath() const
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return Algorithm::String(buffer);
    }

}

