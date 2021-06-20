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

        String applicationPath = FileManager::get().GetApplicationPath();
        int pos = applicationPath.Find("XenonEngine.exe");

        String instructionPath = applicationPath.Substring(0, pos);
        instructionPath.Append("XenonScriptAssemblerMachineInstrction.xet");
        XenonFile*const instructionListFile = FileManager::get().ReadFile(instructionPath);
        assert(instructionListFile != nullptr);
        bool result = m_xsam->InitializeInstructionList(instructionListFile);
        assert(result == true);

        String delimiterPath = applicationPath.Substring(0, pos);
        delimiterPath.Append("XenonScriptAssemblerDelimiter.xet");
        XenonFile* const delimiterListFile = FileManager::get().ReadFile(delimiterPath);
        result = m_xsam->InitializeDelimiterList(delimiterListFile);
        assert(result == true);

        String assemblerPath = applicationPath.Substring(0, pos);
        assemblerPath.Append("Main.xea");
        XenonFile* const assemblerFile = FileManager::get().ReadFile(assemblerPath);
        m_xsam->Compiler(assemblerFile);
    }

}