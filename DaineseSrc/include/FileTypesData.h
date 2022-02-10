#pragma once

#include <map>
#include <string>

#include "FileTypeInfo.h"

namespace gwDainese
{
    class FileTypesData{
        public:
            FileTypesData();
            std::string GetExtension(DaineseDataTypes type);
            double GetFrequency(DaineseDataTypes type);
            std::string GetContentsType(DaineseDataTypes type);
            std::string GetReturnType(DaineseDataTypes type);
            double GetMultiplier(DaineseDataTypes type);
            double GetOffset(DaineseDataTypes type);
        private:
            std::map<DaineseDataTypes, FileTypeInfo> fileData;
            void addItem(DaineseDataTypes, std::string ext, std::string desc, double, 
                        std::string contentsType, std::string returnType,
                        double multiplier = 1.0f, double offset = 0.0f);
    };
}