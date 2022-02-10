#pragma once
namespace gwDainese
{
    enum class DaineseDataTypes {
        a_lon_gps,      // longitudinal acceleration, calculated - fore and aft.
        a_lat_gps,      // latitudinal acceleration, calculated - left and right.
        acc_x,          // acceleration in x.
        acc_y,          // acceleration in y.
        acc_z,          // acceleration in z.
        altitude_meas,  // altitude, measured
        altitude_calc,  // altitude, calculated.
        banking_gps,    // lean angle.
        course,         // heading, in degrees?
        gps_yaw,        // yaw, calculated from gps.
        hhmm,           // hours/minutes.
        lat_dez,        // raw latitude, degrees.
        latitude,       // calculated latitude, degrees.
        lon_dez,        // raw longitude, degrees
        longitude,      // calculated longitude, degrees.
        mmdd,           // month/day.
        riskAccInt,     // risk of accident factor?
        sshh,           // seconds/hundredths.
        v_bat,          // battery voltage.
        v_gps,          // velocity, measured from gps.
        v_sat_meas,     // velocity from satellite, measured.
        v_sat_calc,     // velocity from satellite, calculated.
    };
}