#include "Engine/VirtualMachine/XenonVirtualMachine.h"
#include "Engine/FileManager/FileManager.h"

namespace XenonEnigne
{


    bool XenonVirtualMachine::Initialize()
    {
        char* fileContent;
        FileManager::get().ReadFile("main.xon", &fileContent);

        char buffer;

    }

    bool XenonVirtualMachine::Laxer(const char* const content, Token** const tokens)
    {
        unsigned int index = 0;
        char currentChar = content[index];
        Vector< Token* > outputTokens;
        while (currentChar != "\0") {
            switch (currentChar)
            {
            case '(':
                
            default:
                break;
            }
        }
    }

}