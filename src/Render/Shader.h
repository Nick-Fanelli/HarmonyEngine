#pragma once

#include <GL/glew.h>
#include <GLUT/glut.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "harmonypch.h"

namespace HarmonyEngine {

    class Shader {

        const char* m_VertexFilePath;
        const char* m_FragmentFilePath;

        GLuint m_ProgramID, m_VertexID, m_FragmentID;

        void AttachVertextShader(const std::string& source) {
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

        void AttachFragmentShader(const std::string& source) {
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

        void Link() {
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

    public:
        static void Unbind() { glUseProgram(0); }

        Shader() = default;
        Shader(const Shader&) = default;

        Shader(const char* vertexFilePath, const char* fragmentFilePath) : 
        m_VertexFilePath(vertexFilePath), m_FragmentFilePath(fragmentFilePath), m_ProgramID(glCreateProgram()) {}

        void Create() {
            std::string vertexSource = FileUtils::ReadFile(m_VertexFilePath);
            std::string fragmentSource = FileUtils::ReadFile(m_FragmentFilePath);

            AttachVertextShader(vertexSource);
            AttachFragmentShader(fragmentSource);

            Link();
        }

        void Bind() const { glUseProgram(m_ProgramID); }

        void Dispose() const {
            Unbind();
            glDeleteProgram(m_ProgramID);
        }

        const GLuint& GetProgrmID() const { return m_ProgramID; }

        void AddUniformVec4(const char* varName, const glm::vec4& vec4) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniform4f(location, vec4.x, vec4.y, vec4.z, vec4.w);
        }

        void AddUniformVec3(const char* varName, const glm::vec3& vec3) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniform3f(location, vec3.x, vec3.y, vec3.z);
        }

        void AddUniformMat4(const char* varName, const glm::mat4& mat4) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat4));
        }

        void AddUnformInt(const char* varName, int integer) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniform1i(location, integer);
        }

        void AddUniformIntArray(const char* varName, int size, const int* array) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniform1iv(location, size, array);
        }

        void AddUniformUintArray(const char* varName, int size, const uint32_t* array) const {
            auto location = glGetUniformLocation(m_ProgramID, varName);
            glUniform1uiv(location, size, array);
        }

    };

}