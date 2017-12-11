// Copyright 2016-2017 Alessandro Fabbri, Chiara Mizzi, Stefano Sinigardi

/***************************************************************************
This file is part of mob_to_json.

mob_to_json is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

mob_to_json is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with mob_to_json. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/


#ifndef MOB_CLASSES_HPP
#define MOB_CLASSES_HPP

#include <string>
#include <vector>

#include <physycom/split.hpp>

enum
{
  ERROR_NOGEOREF,
  ERROR_NOID_NOGEOREF,
  ERROR_NO_NETWORK,
  ERROR_NOUNCERT
};

class GNSS_record
{
  public:
  size_t timestamp;          // unix time
  std::string date;          // yyyy-mm-dd hh:mm:ss
  double lat, lon;           // [degree]
  double uncert_radius;      // [meters]
  size_t callid, geid;
  std::string network;
  GNSS_record()
  {};
  GNSS_record(const std::vector<std::string> &line)
  {
    callid = stoi(line[0]);
    lat = stod(line[1]);
    lon = stod(line[2]);
  }
  GNSS_record(const jsoncons::json &j)
  {
    lat = j["lat"].as<double>();
    lon = j["lon"].as<double>();
    timestamp = size_t(j["timestamp"].as<int>());
    date = j["date"].as<std::string>();
  }

  // json converter
  jsoncons::json to_json() const
  {
    jsoncons::json j;
    j["lat"] = lat;
    j["lon"] = lon;
    j["timestamp"] = timestamp;
    return j;
  }
};

// MOB UMTS 3G file sample line
// 
// header : GEID, CALL_ID,       DATE,         TIME, NETWORK, LATITUDE [deg], LONGITUDE [deg], UNCERT_SEM_MNR [m], UNCERT_SEM_MJR [m], WCEL_AS1, CPICH_ECN0_AS1 [dB], CPICH_RSCP_AS1 [dBm], SPEED_KMH, RRC_DCH_TIME [s]
// data   :   12,      23, 2014-11-28, 01:02:03.456,   11102,       44.12345,       11.654321,                 33,                 64,    12034,                -5.9,                  -97,      33.1,           230.75
// index  :    0,       1,          2,            3,       4,              5,               6,                  7,                  8,        9,                  10,                   11,        12,               13
namespace UMTS
{
  constexpr int GEID_OFFSET = 0;
  constexpr int CALLID_OFFSET = 1;
  constexpr int DATE_OFFSET = 2;
  constexpr int TIME_OFFSET = 3;
  constexpr int NET_OFFSET = 4;
  constexpr int LAT_OFFSET = 5;
  constexpr int LON_OFFSET = 6;
  constexpr int UNCERT_SEM_MNR_OFFSET = 7;
  constexpr int UNCERT_SEM_MJR_OFFSET = 8;
  constexpr int SPEED_OFFSET = 12;
}

class GNSS_record_UMTS: public GNSS_record
{
  public:
  GNSS_record_UMTS()
  {};
  GNSS_record_UMTS(const std::vector<std::string> &line)
  {
    using namespace UMTS;
    try
    {
      geid = stoi(line[GEID_OFFSET]);
    }
    catch (...)
    {
      geid = 0;
    }
    callid = stoi(line[CALLID_OFFSET]);
    network = line[NET_OFFSET];
    lat = stod(line[LAT_OFFSET]);
    lon = stod(line[LON_OFFSET]);
    try
    {
      uncert_radius = stod(line[UNCERT_SEM_MJR_OFFSET]);
    }
    catch (...)
    {
      if (geid != 0) throw int(ERROR_NOGEOREF);
      else throw int(ERROR_NOID_NOGEOREF);
    }

    date = line[DATE_OFFSET] + " " + line[TIME_OFFSET];

    // Convert date/time to utc
    std::vector<std::string> date_v, time_v;
    physycom::split(date_v, line[DATE_OFFSET], std::string("-"), physycom::token_compress_off);
    physycom::split(time_v, line[TIME_OFFSET], std::string(":"), physycom::token_compress_off);
    struct tm time_now;
    time_now.tm_year = stoi(date_v[0]) - 1900;
    time_now.tm_mon = stoi(date_v[1]) - 1;
    time_now.tm_mday = stoi(date_v[2]);
    time_now.tm_hour = stoi(time_v[0]);
    time_now.tm_min = stoi(time_v[1]);
    time_now.tm_sec = stoi(time_v[2]);
    timestamp = (size_t)mktime(&time_now);
  }
};

// MOB LTE 4G file sample line ---- IMMEDIATE
// 
// header : CALL_ID,       DATE,         TIME, NETWORK, LATITUDE [deg], LONGITUDE [deg], UNCERT_SEM_MNR [meters], UNCERT_SEM_MJR [meters], ORIENT_MJR_AXIS [deg], ALTITUDE [meters],    CELLID, RSRP, RSRQ, SPEED [km/h], RAB TIME [s]
// data   : 8739483, 2017-07-14, 01:02:03.004,   56789,      44.123456,       10.123456,                      16,                      18,                   106,                45, 179643466,  -78,   -9,          4.7,           10
// index  :       0,          1,            2,       3,              4,               5,                       6,                       7,                     8,                 9,        10,   11,   12,           13,           14  
namespace LTE_Immediate
{
  constexpr int CALLID_OFFSET = 0;
  constexpr int DATE_OFFSET = 1;
  constexpr int TIME_OFFSET = 2;
  constexpr int NET_OFFSET = 3;
  constexpr int LAT_OFFSET = 4;
  constexpr int LON_OFFSET = 5;
  constexpr int UNCERT_SEM_MJR_OFFSET = 7;
}

class GNSS_record_LTE_I: public GNSS_record
{
  public:
  GNSS_record_LTE_I()
  {};
  GNSS_record_LTE_I(const std::vector<std::string> &line)
  {
    using namespace LTE_Immediate;
    geid = 0;

    callid = stoi(line[CALLID_OFFSET]);
    network = line[NET_OFFSET];
    lat = stod(line[LAT_OFFSET]);
    lon = stod(line[LON_OFFSET]);
    try
    {
      uncert_radius = stod(line[UNCERT_SEM_MJR_OFFSET]);
    }
    catch (...)
    {
      uncert_radius = 0;
    }

    date = line[DATE_OFFSET] + " " + line[TIME_OFFSET];

    // Convert date/time to utc
    std::vector<std::string> date_v, time_v;
    physycom::split(date_v, line[DATE_OFFSET], std::string("-"), physycom::token_compress_off);
    physycom::split(time_v, line[TIME_OFFSET], std::string(":"), physycom::token_compress_off);
    struct tm time_now;
    time_now.tm_year = stoi(date_v[0]) - 1900;
    time_now.tm_mon = stoi(date_v[1]) - 1;
    time_now.tm_mday = stoi(date_v[2]);
    time_now.tm_hour = stoi(time_v[0]);
    time_now.tm_min = stoi(time_v[1]);
    time_now.tm_sec = stoi(time_v[2]);
    timestamp = (size_t)mktime(&time_now);
  }
};

// MOB LTE 4G file sample line ---- LOGGED
// 
// header : CALL_ID,       DATE,                    TIME, NETWORK, LATITUDE [deg], LONGITUDE [deg], UNCERT_SEM_MNR [meters], UNCERT_SEM_MJR [meters], ORIENT_MJR_AXIS [deg], ALTITUDE [meters],    CELLID, RSRP, RSRQ,
// data   : 8739483, 2017-07-14, 2017-07-14 01:02:03.004,   56789,      44.123456,       10.123456,                      16,                      18,                   106,                45, 179643466,  -78,   -9,
// index  :       0,          1,                       2,       3,              4,               5,                       6,                       7,                     8,                 9,        10,   11,   12,
namespace LTE_Logged
{
  constexpr int CALLID_OFFSET = 0;
  constexpr int DATE_OFFSET = 1;
  constexpr int TIME_OFFSET = 2;
  constexpr int NET_OFFSET = 3;
  constexpr int LAT_OFFSET = 4;
  constexpr int LON_OFFSET = 5;
  constexpr int UNCERT_SEM_MJR_OFFSET = 7;
}

class GNSS_record_LTE_L: public GNSS_record
{
  public:
  GNSS_record_LTE_L()
  {};
  GNSS_record_LTE_L(const std::vector<std::string> &line)
  {
    using namespace LTE_Immediate;
    geid = 0;

    callid = stoi(line[CALLID_OFFSET]);
    network = line[NET_OFFSET];

    lat = stod(line[LAT_OFFSET]);
    lon = stod(line[LON_OFFSET]);
    try
    {
      uncert_radius = stod(line[UNCERT_SEM_MJR_OFFSET]);
    }
    catch (...)
    {
      uncert_radius = 0;
    }

    date = line[TIME_OFFSET];

    // Convert date/time to utc
    std::vector<std::string> timestamp_v, date_v, time_v;
    physycom::split(timestamp_v, date, std::string(" "), physycom::token_compress_off);
    physycom::split(date_v, timestamp_v[0], std::string("-"), physycom::token_compress_off);
    physycom::split(time_v, timestamp_v[1], std::string(":"), physycom::token_compress_off);
    struct tm time_now;
    time_now.tm_year = stoi(date_v[0]) - 1900;
    time_now.tm_mon = stoi(date_v[1]) - 1;
    time_now.tm_mday = stoi(date_v[2]);
    time_now.tm_hour = stoi(time_v[0]);
    time_now.tm_min = stoi(time_v[1]);
    time_now.tm_sec = stoi(time_v[2]);
    timestamp = (size_t)mktime(&time_now);
  }
};

// Geometric parameters
#define GEODESIC_DEG_TO_M 111070.4   // conversion [deg] -> [meter] on equatorial circle

// Bounding Box
class Bbox
{
  public:
  double lat_min, lat_max, lat_cen;  // degrees
  double lon_min, lon_max, lon_cen;  // degrees
  double area;                       // m^2
  double len;                        // perimeter [meters]

  Bbox(double lat_min_, double lat_max_, double lon_min_, double lon_max_)
  {
    lat_min = lat_min_;
    lat_max = lat_max_;
    lon_min = lon_min_;
    lon_max = lon_max_;

    lat_cen = 0.5*(lat_min + lat_max);
    lon_cen = 0.5*(lon_min + lon_max);

    // isosceles trapezoid approximation
    double dslat = GEODESIC_DEG_TO_M * (lat_max - lat_min);
    double dslon_min = GEODESIC_DEG_TO_M * cos(lat_min) * (lat_max - lat_min);
    double dslon_max = GEODESIC_DEG_TO_M * cos(lat_max) * (lat_max - lat_min);

    area = 0.25 * (dslon_min + dslon_max) * sqrt(4 * dslat*dslat - (dslon_max - dslon_min)*(dslon_max - dslon_min));
    len = 2 * dslat + dslon_min + dslon_max;
  }

  void dump_bbox_map(std::string basename)
  {
    std::ofstream out(basename + ".html");
    out << R"(<!DOCTYPE html>
<html>
  <head>
    <meta name="viewport" content="initial-scale=1.0, user-scalable=no">
    <meta charset="utf-8">
    <title>)" << basename << R"(</title>
    <style>
      /* Always set the map height explicitly to define the size of the div
       * element that contains the map. */
      #map {
        height: 100%;
      }
      /* Optional: Makes the sample page fill the window. */
      html, body {
        height: 100%;
        margin: 0;
        padding: 0;
      }
    </style>
    <script>
      function initMap() {
        var map = new google.maps.Map(document.getElementById('map'), {
          zoom: 17,
          center: {lat: )" << std::fixed << std::setprecision(8) << lat_cen << R"(, lng: )" << lon_cen << R"(},
          mapTypeId: 'terrain'
        });

        var rectangle = new google.maps.Rectangle({
          strokeColor: '#FF0000',
          strokeWeight: 2,
          strokeOpacity: 0.7,
          fillOpacity: 0.0,
          map: map,
          bounds: {
            north: )" << lat_max << R"(,
            south: )" << lat_min << R"(,
            east: )" << lon_max << R"(,
            west: )" << lon_min << R"(
          }
        });

        var northEast = new google.maps.LatLng()" << lat_max << R"(, )" << lon_max << R"();
        var southWest = new google.maps.LatLng()" << lat_min << R"(, )" << lon_min << R"();
        var bounds = new google.maps.LatLngBounds(southWest,northEast);
        map.fitBounds(bounds);

        var textbox = document.createElement('h1');
        textbox.style.color = 'black';
        textbox.style.background = 'white';
        textbox.innerHTML = 'Area = )" << std::fixed << std::setprecision(2) << area << R"( m^2<br />Length = )" << len << R"( m';
        var textdiv = document.createElement('div');
        textdiv.appendChild(textbox);
        map.controls[google.maps.ControlPosition.BOTTOM_RIGHT].push(textdiv);
      }
    </script>
  </head>
  <body>
    <div id="map"></div>
    <script async defer
    src="https://maps.googleapis.com/maps/api/js?callback=initMap">
    </script>
  </body>
</html>
)";
    out.close();

  }
};

#endif


