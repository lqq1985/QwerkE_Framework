#include "ShaderComponent.h"
#include "../../QwerkE_Common/Utilities/FileIO/FileUtilities.h"
#include "../../Systems/ShaderFactory/ShaderFactory.h"
#include "../../Systems/Services.h"

namespace QwerkE {

    ShaderComponent::ShaderComponent()
    {
    }

    ShaderComponent::ShaderComponent(const char* shaderPath)
    {
        // read and compile file data
        if (FileExists(shaderPath))
        {
            m_Name = GetFileNameWithExt(shaderPath);
            m_Type = GetFileExtension(shaderPath);

            // compile
            if (m_Type == "vert") // TODO: Make an enum for this type
            {
                m_ShaderHandle = ((ShaderFactory*)QwerkE::Services::GetService(eEngineServices::Factory_Shader))->CreateVertexShader(shaderPath);
            }
            else if (m_Type == "frag")
            {
                m_ShaderHandle = ((ShaderFactory*)QwerkE::Services::GetService(eEngineServices::Factory_Shader))->CreateFragmentShader(shaderPath);
            }
            else if (m_Type == "geo")
            {
                m_ShaderHandle = ((ShaderFactory*)QwerkE::Services::GetService(eEngineServices::Factory_Shader))->CreateGeometryShader(shaderPath);
            }
            else
            {
                m_ShaderHandle = 0;
            }
        }
        if (!m_ShaderHandle)
        {
            m_Name = "Error";
            m_Type = "Error";
            OutputPrint("\nError compiling Shader Component: %s\n", shaderPath);
        }
    }

    ShaderComponent::~ShaderComponent()
    {
        // delete shader and shader string data
        glDeleteShader(m_ShaderHandle);
        delete[] m_ShaderData;
    }

}
