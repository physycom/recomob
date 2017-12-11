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


#pragma once
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include <numeric>
#include <string>
#include <algorithm>
#include <iomanip>
#include <ctime>
#include <chrono>
#include <random>
#include <complex>

#ifndef M_PI
#define M_PI 3.1415926535
#endif
using namespace std;


std::vector<std::string> split(std::string const& original, char separator)
{
	std::vector<std::string> results;
	std::string::const_iterator start = original.begin(), end = original.end(), next = std::find(start, end, separator);
	while (next != end)
	{
		results.push_back(std::string(start, next));
		start = next + 1;
		next = std::find(start, end, separator);
	}
	results.push_back(std::string(start, next));
	return results;
}

/*float Q_rsqrt(float num)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;
	x2 = num * 0.5F;
	y = num;
	i = *(long *)&y;
	i = 0x5f3759df - (i >> 1);
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));
	y = y * (threehalfs - (x2 * y * y));
	return (y < 0) ? -y : y;
}*/



//struct points

template <typename T>struct points
{
	vector<T> lat, lon;
	points() {};
	points(const string& filename, char sep = ' ');
};


template <typename T> points<T>::points(const string &filename, char sep)
{
	string line;
	vector<string> tokens;
	ifstream is(filename);
	if (!is)
	{
		cerr << "File not found! Given: " << filename << endl;
		exit(1);
	}
	while (getline(is, line))
	{
		tokens = split(line, sep);
		if (tokens.size() < 2)
		{
			cerr << "not enough input in row " << line << endl;
			exit(1);
		}
		this->lat.push_back((T)stod(tokens[0]));
		this->lon.push_back((T)stod(tokens[1]));
	}
}

//struct polygon

template <typename T>struct polygon
{
	string name;
	vector<T> lat, lon;
	polygon() {name="";};
	polygon(const string& filename, char sep = "\t");
    bool wn_PnPoly(T lat, T lon) const;

	private:
		inline T isLeft(T lat,T lon,T lat_1,T lon_1,T plat,T plon) const;
};


template <typename T> polygon<T>::polygon(const string &filename, char sep)
{
	string line;
	vector<string> tokens;
	ifstream is(filename);
	if (!is)
	{
		cerr << "File not found! Given: " << filename << endl;
		exit(1);
	}
	while (getline(is, line))
	{
		tokens = split(line, sep);
		if (tokens.size() < 1)
		{
			cerr << "not enough input in row " << line << endl;
			exit(1);
		}
		if (tokens.size() = 1)
			{
				this->name.push_back(tokens[0]);
			}
		else if(tokens.size() = 2)
		{
		this->lat.push_back((T)stod(tokens[0]));
		this->lon.push_back((T)stod(tokens[1]));	
		}
		
	}
}

//inclusion of a point in polygon with Daniel Sunday algorithm (see ref. @ http://www.geomalgorithms.com/a03-_inclusion.html)

template<typename T> inline T polygon<T>::isLeft(T lat,T lon,T lat_1,T lon_1,T plat,T plon) const
{
	return ((lon_1 - lon) * (plat - lat) - (plon - lon) * (lat_1 - lat));
}


template <typename T> bool polygon<T>::wn_PnPoly(T p_lat, T p_lon) const
{
	bool checkite;
	int  i,wn= 0;	// the winding number counter
 		for (i=0; i<this->lat.size()-1; ++i)	// loop through all edges of the polygon
 		{				// edge from V[i] to  V[i+1]
			if (this->lat[i] <= p_lat)
			{	// start y <= P.y
				if (this->lat[i+1] > p_lat) // an upward crossing
				{
					if (isLeft( lat[i], lon[i], lat[i+1], lon[i+1], p_lat, p_lon) > 0) // P left of edge
					{
						++wn; // have a valid up intersect
					}
				}
			}
			else
			{		// start y > P.y (no test needed)
				if (this->lat[i+1] <= p_lat) // a downward crossing
				{
					if (isLeft( lat[i], lon[i], lat[i+1], lon[i+1], p_lat, p_lon) < 0) // P right of edge
					{
						--wn; // have a valid down intersect
					}
				}
			}
		}
		checkite = (wn!=0) ? true : false;
		return checkite;
}

// these optional algorithms check if a point is included, or not, in a polygon

/*
template <typename T> vector<bool> polygon<T>::sommafasi(const points<T> &point)
{
	T t1LAT, t1LONG;
	T t2LAT, t2LONG;
	T t1NORM, t2NORM;
	T num, normainv, angolo, sum;
	vector<bool> check(point.lat.size());
	int j;
	for (int k = 0; k < (int)point.lat.size(); ++k)
	{
		sum = (T)0.0;
		for (j = 0; j < (int)(this->lat.size()-1); ++j)
		{
			t1LAT = this->lat[j] - point.lat[k];
			t1LONG = this->lon[j] - point.lon[k];
			t1NORM = t1LAT*t1LAT + t1LONG*t1LONG;
			t2LAT = this->lat[j + 1] - point.lat[k];
			t2LONG = this->lon[j + 1] - point.lon[k];
			t2NORM = t2LAT*t2LAT + t2LONG*t2LONG;
			num = (t1LAT * t2LAT + t1LONG * t2LONG);
			normainv =1./sqrt(t1NORM*t2NORM);//Q_rsqrt(float (t1NORM * t2NORM));
			std::cout << 1./sqrt(t1NORM*t2NORM) << " " << Q_rsqrt(float (t1NORM * t2NORM))<< std::endl; std::cin.get();
			angolo = acos(num*normainv);
			sum += angolo;
		}
		t1LAT = this->lat[j] - point.lat[k];
		t1LONG = this->lon[j] - point.lon[k];
		t1NORM = t1LAT*t1LAT + t1LONG*t1LONG;
		t2LAT = this->lat[0] - point.lat[k];
		t2LONG = this->lon[0] - point.lon[k];
		t2NORM = t2LAT*t2LAT + t2LONG*t2LONG;
		num = (t1LAT * t2LAT + t1LONG * t2LONG);
		normainv = 1./sqrt(t1NORM*t2NORM);// Q_rsqrt(float (t1NORM * t2NORM));
		std::cout << 1./sqrt(t1NORM*t2NORM) << " " << Q_rsqrt(float (t1NORM * t2NORM))<< std::endl; std::cin.get();

		angolo = acos(num*normainv);
		sum += angolo;

		std::cout << sum - 2*M_PI << " " << sum << std::endl;
		check[k] = ((fabs(sum - 2 * M_PI) < 1e-2f) ? true : false);
	}
	return check;
}

template <typename T> vector<bool> polygon<T>::raycast(const points<T> &point)
{
	T vt;
	int cn;
	vector<bool> check(point.lat.size());
	int i;
	for (int j = 0; j < (int)point.lat.size(); ++j)
	{
		cn = 0;
		for (i = 0; i < (int)(this->lat.size() - 1); ++i)
		{
			if (((this->lon[i] <= point.lon[j]) && (this->lon[i + 1] > point.lon[j]))
				|| ((this->lon[i] > point.lon[j]) && (this->lon[i + 1] <= point.lon[j])))
			{
				vt = (point.lon[j] - this->lon[i]) / (this->lon[i + 1] - this->lon[i]);
				if (point.lat[j] < (this->lat[i] + vt * (this->lat[i + 1] - this->lat[i])))
					++cn;
			}
		}
		if (((this->lon[i] <= point.lon[j]) && (this->lon[0] > point.lon[j]))
			|| ((this->lon[i] > point.lon[j]) && (this->lon[0] <= point.lon[j])))
		{
			vt = (point.lon[j] - this->lon[i]) / (this->lon[0] - this->lon[i]);
			if (point.lat[j] < (this->lat[i] + vt * (this->lat[0] - this->lat[i])))
				++cn;
		}
		check[j]=((cn % 2) != 0) ? true : false;
	}
	return check;
}


// this is an optional control of consistency between different algorithms doing the same task

/*bool control(vector<bool> metodo_1, vector<bool> metodo_2)
{
	int cnt = 0;
	for (int i = 0; i < (int)metodo_1.size(); ++i)
	{
		cnt += (metodo_1[i]==metodo_2[i]);
	}
	return (cnt == metodo_1.size()) ;
}*/

