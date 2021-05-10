#pragma once

#include "harmonypch.h"

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
    }

}