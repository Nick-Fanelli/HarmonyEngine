#include "Utils.h"

namespace HarmonyEngine {

    namespace FileUtils {
        std::string ReadFile(const char* filepath) {
            std::ifstream fileStream(filepath);
            std::string result;
            std::string line;

            while(std::getline(fileStream, line)) {
                result.append(line).append("\n");
            }

            return result;
        }

        std::string ReadFile(const std::string& filepath) {
            std::ifstream fileStream(filepath);
            std::string result;
            std::string line;

            while(std::getline(fileStream, line)) {
                result.append(line).append("\n");
            }

            return result;
        }

        bool FileExists(const char* filepath) {
            if(FILE* file = fopen(filepath, "r")) {
                fclose(file);
                return true;
            } else {
                return false;
            }
        }

        bool FileExists(const std::string& filepath) {
            if(FILE* file = fopen(filepath.c_str(), "r")) {
                fclose(file);
                return true;
            } else {
                return false;
            }
        }
    }

    namespace OpenGLUtils {

        int GetGUPMaxTextureSlots() {
            int queryResult;
            glGetIntegerv(GL_MAX_TEXTURE_IMAGE_UNITS, &queryResult);
            return queryResult;
        }

    }
}