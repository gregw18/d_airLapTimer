#include <string>
#include <typeinfo>

#include "DaineseDataTypes.h"

namespace gwDainese
{
    class FileTypeInfo{
        public:
            DaineseDataTypes dataType;
            std::string fileExtension;
            std::string contentsDesc;
            double samplingFrequency;
            std::string fileContentsType;    // Type of data inside the file.
            std::string returnDataType;      // Type to return data as.
            double multiplier;          // Used for cases where have to scale stored value to get actual value.
            double offset;              // Used for cases where have to shift stored value to get actual value.
            
            FileTypeInfo(){
                dataType = DaineseDataTypes::hhmm;
                fileExtension = "";
                contentsDesc = "";
                samplingFrequency = 0;
                fileContentsType = typeid(short).name();
                returnDataType = typeid(short).name();
            }

            FileTypeInfo(DaineseDataTypes type, 
                        std::string ext, 
                        std::string desc,
                        double freq,
                        std::string contentsType,
                        std::string returnType,
                        double mult,
                        double off):
                dataType(type), 
                fileExtension(ext),
                contentsDesc(desc), 
                samplingFrequency(freq),
                fileContentsType(contentsType),
                returnDataType(returnType),
                multiplier(mult),
                offset(off){
            }

    };
}
