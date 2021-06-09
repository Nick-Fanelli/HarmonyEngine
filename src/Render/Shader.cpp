#include "Shader.h"

using namespace HarmonyEngine;

void Shader::AttachVertextShader(const std::string& source) {
    GLint result = GL_FALSE;
    int infoLogLength;

    m_VertexID = glCreateShader(GL_VERTEX_SHADER);
    const char* sourcePointer = source.c_str();
    glShaderSource(m_VertexID, 1, &sourcePointer, NULL);
    glCompileShader(m_VertexID);

    // Check Compilation Status
    glGetShaderiv(m_VertexID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(m_VertexID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0) {
        std::vector<char> errorMessage(infoLogLength + 1);
        glGetShaderInfoLog(m_VertexID, infoLogLength, NULL, &errorMessage[0]);
        Log::Error(&errorMessage[0]);
    }

    Log::Info("Attaching Vertex Shader...");
    glAttachShader(m_ProgramID, m_VertexID);
    Log::Success("Attached Vertex Shader!");
}

void Shader::AttachFragmentShader(const std::string& source) {
    GLint result = GL_FALSE;
    int infoLogLength;

    m_FragmentID = glCreateShader(GL_FRAGMENT_SHADER);
    const char* sourcePointer = source.c_str();
    glShaderSource(m_FragmentID, 1, &sourcePointer, NULL);
    glCompileShader(m_FragmentID);

    // Check Compilation Status
    glGetShaderiv(m_FragmentID, GL_COMPILE_STATUS, &result);
    glGetShaderiv(m_FragmentID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0) {
        std::vector<char> errorMessage(infoLogLength + 1);
        glGetShaderInfoLog(m_FragmentID, infoLogLength, NULL, &errorMessage[0]);
        Log::Error(&errorMessage[0]);
    }

    Log::Info("Attaching Fragment Shader...");
    glAttachShader(m_ProgramID, m_FragmentID);
    Log::Success("Attached Fragment Shader!");
}

void Shader::Link() {
    glLinkProgram(m_ProgramID);

    GLint result = GL_FALSE;
    int infoLogLength;

    // Check Linking Status
    glGetProgramiv(m_ProgramID, GL_LINK_STATUS, &result);
    glGetProgramiv(m_ProgramID, GL_INFO_LOG_LENGTH, &infoLogLength);

    if(infoLogLength > 0) {
        std::vector<char> errorMessage(infoLogLength + 1);
        glGetProgramInfoLog(m_ProgramID, infoLogLength, NULL, &errorMessage[0]);
        Log::Error(&errorMessage[0]);
    }

    glDetachShader(m_ProgramID, m_VertexID);
    glDetachShader(m_ProgramID, m_FragmentID);

    glDeleteShader(m_VertexID);
    glDeleteShader(m_FragmentID);
}

void Shader::ApplyReplacements(std::string& vertexSource, std::string& fragmentSource) {
    for(auto& replacement : m_Replacements) {
        std::regex regex = std::regex("\\$" + replacement.first + "\\$");

        vertexSource = std::regex_replace(vertexSource, regex, replacement.second);
        fragmentSource = std::regex_replace(fragmentSource, regex, replacement.second);
    }
}

void Shader::Create() {
    std::string vertexSource = FileUtils::ReadFile(m_VertexFilePath);
    std::string fragmentSource = FileUtils::ReadFile(m_FragmentFilePath);

    ApplyReplacements(vertexSource, fragmentSource);

    AttachVertextShader(vertexSource);
    AttachFragmentShader(fragmentSource);

    Link();
}

void Shader::Bind() const { glUseProgram(m_ProgramID); }

void Shader::Dispose() const {
    Unbind();
    glDeleteProgram(m_ProgramID);
}

void Shader::AddUniformVec4(const char* varName, const glm::vec4& vec4) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
}

void Shader::AddUniformVec3(const char* varName, const glm::vec3& vec3) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniform3f(location, vec3.x, vec3.y, vec3.z);
}

void Shader::AddUniformMat4(const char* varName, const glm::mat4& mat4) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
}

void Shader::AddUnformInt(const char* varName, int integer) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniform1i(location, integer);
}

void Shader::AddUniformIntArray(const char* varName, int count, const int* array) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniform1iv(location, count, array);
}

void Shader::AddUniformUintArray(const char* varName, int size, const uint32_t* array) const {
    auto location = glGetUniformLocation(m_ProgramID, varName);
    glUniform1uiv(location, size, array);
}