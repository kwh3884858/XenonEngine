#include "XenonCompiler.h"
#include "Engine/VirtualMachine/XenonScriptAssemblerMachine.h"
#include "Engine/FileManager/FileManager.h"
#include <cassert>

namespace XenonEnigne 
{
    XenonCompiler::XenonCompiler()
    {
    }

    XenonCompiler::~XenonCompiler()
    {
    }

    void XenonCompiler::Initialize()
    {
        m_xsam = new XenonScriptAssemblerMachine;
        XenonFile*const instructionListFile = FileManager::get().ReadFile("XenonScriptAssemblerMachineInstrction.xet");
        assert(instructionListFile != nullptr);
        bool result = m_xsam->InitializeInstructionList(instructionListFile);
        assert(result == true);

        XenonFile* const delimiterListFile = FileManager::get().ReadFile("../Data/XenonScriptAssemblerDelimiter.xet");
        result = m_xsam->InitializeDelimiterList(delimiterListFile);
        assert(result == true);

    }

}