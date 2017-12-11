// Copyright 2017 Andrea Checcoli

/***************************************************************************
This file is part of recomob.

recomob is free software : you can redistribute it and / or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

recomob is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with recomob. If not, see <http://www.gnu.org/licenses/>.
***************************************************************************/


#include <iostream>
#include <map>
#include <set>

#include <boost/filesystem.hpp>
#include <jsoncons/json.hpp>

#include "mob_classes.hpp"
#include "points_polygon.h"

using namespace std;

#define MAJOR 1
#define MINOR 0

void usage(char * progname)
{
  vector<string> tokens;
  physycom::split(tokens, progname, "/\\");
  cout << "Usage: " << tokens.back() << " path/to/csv" << endl;
}



// Assign time slot

// 1) Auto mode, based on minutes

std::string find_slot_auto_ranges_minutes(const std::string &date, const std::string &time_min = "00:00", const std::string &time_max = "23:59", int dtmin = 10)
{ // 3/10/2016 12:14:34
  // extract date components and convert hour to minutes since 00:00:00
  std::vector<std::string> tok;
  physycom::split(tok, date, "- :", physycom::token_compress_off);
  std::stringstream slot;
  slot << tok[0] << tok[1] << tok[2] << "_";
  size_t tmin_now = stoi(tok[3]) * 60 + stoi(tok[4]);
  // convert time_min and time_max to minutes since 00:00:00
  physycom::split(tok, time_min, ":", physycom::token_compress_off);
  size_t tmin_min = stoi(tok[0]) * 60 + stoi(tok[1]);
  physycom::split(tok, time_max, ":", physycom::token_compress_off);
  size_t tmin_max = stoi(tok[0]) * 60 + stoi(tok[1]);
  // prepare the output string
  if (tmin_now >= tmin_min && tmin_now < tmin_max)
  {
    size_t slot_index = (tmin_now - tmin_min) / dtmin;
    size_t tmin_slot = tmin_min + slot_index*dtmin;
    size_t hi = tmin_slot / 60;
    size_t mi = tmin_slot - hi * 60;
    size_t hf, mf;
    if (tmin_max > tmin_slot + dtmin)
    {
      hf = (tmin_slot + dtmin) / 60;
      mf = tmin_slot + dtmin - hf * 60;
    }
    else
    {
      hf = tmin_max / 60;
      mf = tmin_max - hf * 60;
    }
    slot << std::setw(2) << std::setfill('0') << hi << "."
      << std::setw(2) << std::setfill('0') << mi << "-"
      << std::setw(2) << std::setfill('0') << hf << "."
      << std::setw(2) << std::setfill('0') << mf;
  }
  else
  {
    slot.str(std::string());
  }
  return slot.str();
}

int main(int argc, char** argv)
{
  cout << "recomob v" << MAJOR << "." << MINOR << endl;


  // Parsing command line
  string input_name;
  if (argc == 2)
  {
    input_name = argv[1];
  }
  else
  {
    cerr << "ERROR: No flags specified. Read usage and relaunch properly." << endl;
    usage(argv[0]);
    exit(3);
  }
  cout << "Input file : " << input_name << endl;

  // Containers
  vector<GNSS_record> data;

  // General purpose variables
  string line;
  vector<string> tokens;

  // Parse input
  ifstream input(input_name);
  if (!input)
  {
    cerr << "ERROR: Unable to open " << input_name << ". Quitting..." << endl;
    exit(5);
  }
  while (getline(input, line))
  {
    physycom::split(tokens, line, string(","), physycom::token_compress_off);
    try
    {
      data.push_back(GNSS_record_UMTS(tokens));
    }
    catch (...)
    {
    }
  }

  cout << "size " << data.size() << endl;

  // Filling map
  map<int, map<int, vector<GNSS_record>>> datamap;
  for (const auto &record : data)
  {
    datamap[record.geid][record.callid].push_back(record);
  }

  //timedatamap 
  map<string, map< int, map< int, vector<GNSS_record>>>> timedatamap;
  string pablo;
  for (const auto &g : datamap)
  {
    for (const auto &c : g.second)
    {

      pablo = find_slot_auto_ranges_minutes(datamap[g.first][c.first].front().date, "08:00", "23:50", 10);
      if (pablo.size()) timedatamap[pablo][g.first][c.first] = datamap[g.first][c.first];
    }
  }

  //setting the path
  std::string _outputDir = "Analytics";
  boost::filesystem::create_directory(_outputDir);
  std::cout << _outputDir << "\n";

  //creating a map based on sites'coordinates

  vector<polygon<double>> site_sest;
  ifstream sesfile("Coord_Sestiere.txt");
  string riga;
  vector<string> tokenationses;
  polygon<double> tpolygonses;
  while (getline(sesfile, riga))
  {
    tokenationses = split(riga, '\t');
    if (tokenationses.size() < 1)
    {
      cerr << "not enough input in row " << line << endl;
      exit(1);
    }
    if (tokenationses.size() == 1)
    {
      if (tpolygonses.name != "") site_sest.push_back(tpolygonses);  //il primo elemento nasce vuoto. così si evita di pushare il primo vuoto
      tpolygonses.lat.resize(0);
      tpolygonses.lon.resize(0);
      tpolygonses.name = tokenationses[0];
    }
    else if (tokenationses.size() == 2)
    {
      tpolygonses.lat.push_back(stod(tokenationses[1]));
      tpolygonses.lon.push_back(stod(tokenationses[0]));
    }
  }
  sesfile.close();
  site_sest.push_back(tpolygonses);

  //shift index from string to int

  map<string, int> tempi_n2i;
  int cnt = 0;
  for (auto t : timedatamap)
  {
    tempi_n2i[t.first] = cnt++;
  }

  //Number of Geid detected on the map

  vector<int> total_count_sest;
  for (const auto &t : timedatamap)
  {
    total_count_sest.push_back(t.second.size());
  }

  //Geid record ofstream

  ofstream wfile("total.txt");
  for (int i = 0; i < total_count_sest.size(); ++i)
  {
    wfile << total_count_sest[i] << endl;
  }
  wfile.close();

  //Stats: geid's count for every site

  std::set<int> NmbrGeid;
  std::set<int> NmbrCallid;
  map<string, map<string, int>> totali;
  for (const auto &t : timedatamap)
  {
    for (const auto&g : t.second)  // loopo sui geid
    {
      NmbrGeid.insert(g.first);
      int siti_no = 0;
      for (const auto &c : g.second) //loopo sui callid
      {
        NmbrCallid.insert(c.first);
        for (const auto &e : site_sest) //loopo sui siti
        {
          if (e.wn_PnPoly(c.second.back().lat, c.second.back().lon))
          {
            totali[t.first][e.name]++;
            break;
          }
          else
          {
            ++siti_no;
          }
        }
        if (siti_no == site_sest.size()) totali[t.first]["nowhere"]++; //still in the map but in an undefined area
        break;
      }
    }
  }

  // Stats ofstream

  ofstream cfile("check.txt");
  for (auto &t : timedatamap)
  {
    cfile << t.first.substr(t.first.find_last_of("_") + 1) << "\t";
    for (const auto &cnt : totali[t.first])
    {
      cfile << cnt.second << "\t";
    }
    cfile << " T " << total_count_sest[tempi_n2i[t.first]] << endl;
  }
  cfile.close();


  return 0;
}


