#include "CrossPlatform/File/FileReader.h"
#include "Algorithms/Vector.h"

#include <fstream>
#include <assert.h>

namespace File
{
    using Algorithm::Vector;

    void FileReader::ReadFile(const char* const fileName, char** file)
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
                contentBuffer.add(charBuffer);
            }
            fins.close();
        }
        else
        {
            std::cerr << "Error: " << strerror(errno);
            return false;
        }
        fins.close();

        char* content = new char[contentBuffer.size()];
        memcpy_s(content, contentBuffer.size(), contentBuffer.begin(), contentBuffer.size());

        // The last character will push twish, because EOF will not make different with
        //original character, so we should set the last one to terminal character to
        //finish the sentense
        content[contentBuffer.size() - 1] = '\0';

        *file = content;

        return true;
    }

}