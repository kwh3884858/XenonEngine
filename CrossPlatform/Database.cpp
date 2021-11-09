#include "Database.h"
#include "CrossPlatform/Converter/EngineConfigYamlConverter.h"
#include "Windows/File/FileReader.h"

namespace CrossPlatform
{
    using namespace Algorithm;
    bool Database::Initialize()
    {
        File::FileReader fileReader;
        String applicationPath = fileReader.GetApplicationPath();
        int pos = applicationPath.IndexOf("XenonEngine.exe");

        String instructionPath = applicationPath.Substring(0, pos);
        instructionPath.Append("EngineConfig.yaml");

        YAML::Node config = YAML::LoadFile(instructionPath.CString());
        engineConfig = config.as<EngineConfig>();
        return true;
    }

}