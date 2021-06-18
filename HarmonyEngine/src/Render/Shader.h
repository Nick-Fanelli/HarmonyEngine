#pragma once

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "harmonypch.h"

namespace HarmonyEngine {

    class Shader {

        const char* m_VertexFilePath;
        const char* m_FragmentFilePath;

        GLuint m_ProgramID, m_VertexID, m_FragmentID;

        std::unordered_map<std::string, std::string> m_Replacements;

        void AttachVertextShader(const std::string& source);
        void AttachFragmentShader(const std::string& source);
        
        void Link();

        void ApplyReplacements(std::string& vertexSource, std::string& fragmentSource);

    public:
        static void Unbind() { glUseProgram(0); }

        Shader() = default;
        Shader(const Shader&) = default;

        Shader(const char* vertexFilePath, const char* fragmentFilePath) : 
        m_VertexFilePath(vertexFilePath), m_FragmentFilePath(fragmentFilePath), m_ProgramID(glCreateProgram()) {}

        Shader(const char* vertexFilePath, const char* fragmentFilePath, std::unordered_map<std::string, std::string>& replacements) : 
        m_VertexFilePath(vertexFilePath), m_FragmentFilePath(fragmentFilePath), m_ProgramID(glCreateProgram()), m_Replacements(replacements) {}

        void Create();
        
        void Bind() const;
        void Dispose() const;

        void AddUniformVec4(const char* varName, const glm::vec4& vec4) const;
        void AddUniformVec3(const char* varName, const glm::vec3& vec3) const;
        void AddUniformMat4(const char* varName, const glm::mat4& mat4) const;
        void AddUnformInt(const char* varName, int integer) const;
        void AddUniformIntArray(const char* varName, int size, const int* array) const;
        void AddUniformUintArray(const char* varName, int size, const uint32_t* array) const;

        const GLuint& GetProgrmID() const { return m_ProgramID; }

    };

}