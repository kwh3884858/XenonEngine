#include "XenonCompiler.h"
#include "Engine/VirtualMachine/XenonScriptAssemblerMachine.h"
#include "Engine/VirtualMachine/XenonVirtualMachine.h"
#include "Engine/FileManager/FileManager.h"
#include "Algorithms/StreamingVector.h"
#include <cassert>

namespace XenonEngine 
{
    using Algorithm::StreamingVector;

    XenonCompiler::XenonCompiler()
    {
    }

    XenonCompiler::~XenonCompiler()
    {
    }

    void XenonCompiler::Initialize()
    {
        m_xsam = new XenonScriptAssemblerMachine;
        m_xvm = new XenonVirtualMachine;

        String applicationPath = FileManager::Get().GetApplicationPath();
        int pos = applicationPath.Find("XenonEngine.exe");

        String instructionPath = applicationPath.Substring(0, pos);
        instructionPath.Append("XenonScriptAssemblerMachineInstrction.xet");
        XenonFile* instructionListFile = FileManager::Get().ReadFile(instructionPath);
        assert(instructionListFile != nullptr);
        bool result = m_xsam->InitializeInstructionList(instructionListFile);
        delete instructionListFile;
        instructionListFile = nullptr;
        assert(result == true);

        String delimiterPath = applicationPath.Substring(0, pos);
        delimiterPath.Append("XenonScriptAssemblerDelimiter.xet");
        XenonFile*  delimiterListFile = FileManager::Get().ReadFile(delimiterPath);
        result = m_xsam->InitializeDelimiterList(delimiterListFile);
        delete delimiterListFile;
        delimiterListFile = nullptr;
        assert(result == true);

        Recompile();
    }

    void XenonCompiler::Shutdown()
    {
        delete m_xsam;
        delete m_xvm;
    }

    void XenonCompiler::Recompile()
    {
        String applicationPath = FileManager::Get().GetApplicationPath();
        int pos = applicationPath.Find("XenonEngine.exe");

        String assemblerPath = applicationPath.Substring(0, pos);
        assemblerPath.Append("Main.xea");
        XenonFile*  assemblerFile = FileManager::Get().ReadFile(assemblerPath);
        m_xsam->Compiler(assemblerFile);
        m_xsam->BuildXEX(assemblerFile);
        delete assemblerFile;
        assemblerFile = nullptr;

        String executableFilePath = applicationPath.Substring(0, pos);
        executableFilePath.Append("Main.xex");
        StreamingVector<char>* streamedFile = FileManager::Get().ReadStreamFile(executableFilePath);
        m_xvm->LoadScript(streamedFile);
        bool result = m_xvm->RunScript();
        assert(result == true);
    }

}