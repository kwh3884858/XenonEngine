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

    XenonFile*const FileReader::ReadFile(const char* const fileName) const
{
        assert(fileName != nullptr);

        std::ifstream fins;
        XenonFile* xenonFile = new XenonFile;

        fins.open(fileName);

        // If it could not open the file then exit.
        if (!fins.fail())
        {
            fins >> std::noskipws;
            char charBuffer = '\0';

            while (!fins.eof())
            {
                fins >> charBuffer;
                if (charBuffer == XenonFile::CL) // CL, carriage return
                {
                    xenonFile->m_lineSize++;
                }
                if (charBuffer == XenonFile::LF) //LF, line feed
                {
                    continue;
                }
                xenonFile->m_content.Add(charBuffer);
            }
            fins.close();
        }
        else
        {
            std::cerr << "Error: " << strerror(errno);
            return nullptr;
        }
        fins.close();

        return xenonFile;
    }

    Algorithm::String FileReader::GetApplicationPath() const
    {
        char buffer[MAX_PATH];
        GetModuleFileNameA(NULL, buffer, MAX_PATH);
        return String(buffer);
    }

}

