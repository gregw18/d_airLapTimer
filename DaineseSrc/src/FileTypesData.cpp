#include <typeinfo>

#include "FileTypesData.h"

using namespace std;

namespace gwDainese
{
    // Store details for file types in one place.
    FileTypesData::FileTypesData() {
        map<DaineseDataTypes, FileTypeInfo> fileData;
        addItem(DaineseDataTypes::altitude_meas, ".A0N", "meters", 6.25, typeid(unsigned long).name(), typeid(double).name(), 0.01);
        addItem(DaineseDataTypes::altitude_calc, ".C02", "meters", 6.25, typeid(float).name(), typeid(float).name());
        addItem(DaineseDataTypes::acc_x, ".A01", "m/s^2", 1000, typeid(unsigned short).name(), typeid(double).name(), 0.005, -32767);
        addItem(DaineseDataTypes::acc_y, ".A02", "m/s^2", 1000, typeid(unsigned short).name(), typeid(double).name(), 0.005, -32767);
        addItem(DaineseDataTypes::acc_z, ".A03", "m/s^2", 1000, typeid(unsigned short).name(), typeid(double).name(), 0.005, -32767);
        addItem(DaineseDataTypes::a_lon_gps, ".C06", "m/s^2", 25, typeid(unsigned short).name(), typeid(double).name(), 0.001, -32768);
        addItem(DaineseDataTypes::a_lat_gps, ".C09", "m/s^2", 100, typeid(unsigned short).name(), typeid(double).name(), 0.001, -32768);
        addItem(DaineseDataTypes::banking_gps, ".C0A", "deg", 100, typeid(float).name(), typeid(float).name());
        addItem(DaineseDataTypes::course, ".A0K", "deg", 6.25, typeid(unsigned short).name(), typeid(double).name(), 0.01);
        addItem(DaineseDataTypes::gps_yaw, ".C08", "deg/s", 100, typeid(unsigned short).name(), typeid(double).name(), 0.005, -32768);
        addItem(DaineseDataTypes::hhmm, ".A0J", "hours/minutes", 6.25, typeid(unsigned short).name(), typeid(unsigned short).name());
        addItem(DaineseDataTypes::lat_dez, ".A0L", "degrees", 6.25, typeid(unsigned long).name(), typeid(double).name(), 0.0000001);
        addItem(DaineseDataTypes::lon_dez, ".A0M", "degrees", 6.25, typeid(unsigned long).name(), typeid(double).name(), 0.0000001);
        addItem(DaineseDataTypes::latitude, ".C01", "degrees", 12.5, typeid(double).name(), typeid(double).name());
        addItem(DaineseDataTypes::longitude, ".C00", "degrees", 12.5, typeid(double).name(), typeid(double).name());
        addItem(DaineseDataTypes::mmdd, ".A0O", "month/day", 6.25, typeid(unsigned short).name(), typeid(unsigned short).name());
        addItem(DaineseDataTypes::riskAccInt, ".A0V", "???", 1000, typeid(unsigned short).name(), typeid(unsigned short).name());
        addItem(DaineseDataTypes::sshh, ".A0P", "seconds/hundredths", 6.25, typeid(unsigned short).name(), typeid(unsigned short).name());
        addItem(DaineseDataTypes::v_bat, ".A0D", "volts", 50, typeid(unsigned short).name(), typeid(double).name(), 0.001);
        addItem(DaineseDataTypes::v_gps, ".C05", "km/h", 100, typeid(unsigned short).name(), typeid(double).name(), 0.01);
        addItem(DaineseDataTypes::v_sat_meas, ".A0H", "km/h", 6.25, typeid(short).name(), typeid(short).name());
        addItem(DaineseDataTypes::v_sat_calc, ".C04", "km/h", 6.25, typeid(float).name(), typeid(float).name());
    }

    string FileTypesData::GetExtension(DaineseDataTypes type){
        return fileData[type].fileExtension;
    }

    double FileTypesData::GetFrequency(DaineseDataTypes type) {
        return fileData[type].samplingFrequency;
    }

    string FileTypesData::GetContentsType(DaineseDataTypes type){
        return fileData[type].fileContentsType;
    }

    string FileTypesData::GetReturnType(DaineseDataTypes type){
        return fileData[type].returnDataType;
    }

    double FileTypesData::GetMultiplier(DaineseDataTypes type){
        return fileData[type].multiplier;
    }

    double FileTypesData::GetOffset(DaineseDataTypes type){
        return fileData[type].offset;
    }

    void FileTypesData::addItem(DaineseDataTypes dataType,
                string fileExtension,
                string contentsDesc,
                double samplingFrequency,
                string fileContentsType, 
                string returnDataType,
                double multiplier,
                double offset) {
        FileTypeInfo thisInfo (dataType, fileExtension, contentsDesc, samplingFrequency, 
                                fileContentsType, returnDataType, multiplier, offset);
        fileData[thisInfo.dataType] = thisInfo;
    }
}
