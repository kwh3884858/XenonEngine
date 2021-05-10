#include "XenonCompiler.h"
#include "Engine/VirtualMachine/XenonScriptAssemblerMachine.h"

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
        m_xsam->InitializeInstructionList(xenonFile);
    }

}