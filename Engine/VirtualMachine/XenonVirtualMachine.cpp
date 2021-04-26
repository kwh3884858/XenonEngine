#include "Engine/VirtualMachine/XenonVirtualMachine.h"
#include "Engine/FileManager/FileManager.h"
#include "Algorithms/Vector.h"

namespace XenonEnigne
{
using Algorithm::Vector;

    bool XenonVirtualMachine::Initialize()
    {
        char* fileContent;
        FileManager::get().ReadFile("main.xon", &fileContent);

        char buffer;
        return true;
    }

    bool XenonVirtualMachine::Laxer(const char* const content, Token** const tokens)
    {
        unsigned int index = 0;
        char currentChar = content[index];
        Vector< Token* > outputTokens;
        while (currentChar != '\0') {
            switch (currentChar)
            {
            case '(':
                
            default:
                break;
            }
        }
        return true;
    }

}