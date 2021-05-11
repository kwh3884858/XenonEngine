#include "XenonCompiler.h"
#include "Engine/VirtualMachine/XenonScriptAssemblerMachine.h"
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
        XenonFile*const xenonFile = FileManager::get().ReadFile("XenonScriptAssemblerMachineInstrction.xet");
        bool result = m_xsam->InitializeInstructionList(xenonFile);
        assert(result == true);
    }

}