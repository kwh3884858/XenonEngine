#include "Windows/File/FileReader.h"
#include "Algorithms/Vector.h"

#include <fstream>
#include <iostream> // For std::cerr
#include <assert.h>

namespace File
{
    using Algorithm::Vector;

    bool FileReader::ReadFile(const char* const fileName, char** file)const
    {
        std::ifstream fins;

        char charBuffer = '\0';
        Vector<char> contentBuffer;

        int i = 0;

        //memset(content, 0, sizeof(content));
        assert(fileName != nullptr);
        assert(file != nullptr);
        assert(*file != nullptr);

        fins.open(fileName);
        assert(fins.peek() != EOF);
        if (fins.peek() == EOF) {

            std::cerr << "Error: File is empty.";
            return false;
        }
        // If it could not open the file then exit.
        if (!fins.fail())
        {
            fins >> std::noskipws;
            while (!fins.eof())
            {
                //fins >> tcontent[i++];
                fins >> charBuffer;
                contentBuffer.Add(charBuffer);
            }
            fins.close();
        }
        else
        {
            std::cerr << "Error: " << strerror(errno);
            return false;
        }
        fins.close();

        char* content = new char[contentBuffer.Count()];
        memcpy_s(content, contentBuffer.Count(), contentBuffer.begin(), contentBuffer.Count());

        // The last character will push twish, because EOF will not make different with
        //original character, so we should set the last one to terminal character to
        //finish the sentense
        content[contentBuffer.Count() - 1] = '\0';

        *file = content;

        return true;
    }

}

