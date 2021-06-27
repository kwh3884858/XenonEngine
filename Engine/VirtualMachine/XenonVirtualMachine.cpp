#include "Engine/VirtualMachine/XenonVirtualMachine.h"

#include "Engine/FileManager/FileManager.h"
#include "CrossPlatform/XenonFile.h"

#include "Algorithms/Vector.h"

namespace XenonEnigne
{
using Algorithm::Vector;
using CrossPlatform::XenonFile;

    bool XenonVirtualMachine::Initialize()
    {
        XenonFile*const fileContent = FileManager::Get().ReadFile("main.xon");

        return true;
    }

    void XenonVirtualMachine::LoadScript(Algorithm::StreamingVector<char>* streamedFile)
    {
        
    }

    //bool XenonVirtualMachine::Laxer(const char* const content, Token** const tokens)
    //{
    //    unsigned int index = 0;
    //    char currentChar = content[index];
    //    Vector< Token* > outputTokens;
    //    while (currentChar != '\0') {
    //        switch (currentChar)
    //        {
    //        case '(':
    //            
    //        default:
    //            break;
    //        }
    //    }
    //    return true;
    //}

}