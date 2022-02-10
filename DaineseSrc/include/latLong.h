#pragma once

#include <vector>


namespace gwDainese
{
    // Helper for when need to pass around latitude/longitude pair.
    struct latLongPoint{
        public:
            double latitude;
            double longitude;
            
            latLongPoint() {
                latitude = 0;
                longitude = 0;
            }

            latLongPoint(double lat, double lon):
                latitude(lat), 
                longitude(lon)
                {}
    };

    struct TrackData{
        public:
            latLongPoint startFinishPoint;
            latLongPoint trackEntrancePoint;
            std::vector<latLongPoint> trackExitPoints;

            TrackData(latLongPoint myStart, latLongPoint pitOut, const std::vector<latLongPoint>& pitIns) :
                startFinishPoint(myStart),
                trackEntrancePoint(pitOut),
                trackExitPoints(pitIns)
            { }
    };
}

