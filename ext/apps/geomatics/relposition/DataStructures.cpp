//==============================================================================
//
//  This file is part of GNSSTk, the ARL:UT GNSS Toolkit.
//
//  The GNSSTk is free software; you can redistribute it and/or modify
//  it under the terms of the GNU Lesser General Public License as published
//  by the Free Software Foundation; either version 3.0 of the License, or
//  any later version.
//
//  The GNSSTk is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU Lesser General Public License for more details.
//
//  You should have received a copy of the GNU Lesser General Public
//  License along with GNSSTk; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110, USA
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin.
//  Copyright 2004-2022, The Board of Regents of The University of Texas System
//
//==============================================================================

//==============================================================================
//
//  This software was developed by Applied Research Laboratories at the
//  University of Texas at Austin, under contract to an agency or agencies
//  within the U.S. Department of Defense. The U.S. Government retains all
//  rights to use, duplicate, distribute, disclose, or release this software.
//
//  Pursuant to DoD Directive 523024
//
//  DISTRIBUTION STATEMENT A: This software has been approved for public
//                            release, distribution is unlimited.
//
//==============================================================================

/**
 * @file DataStructures.cpp
 * Implement various data containers for program DDBase.
 */

//------------------------------------------------------------------------------------
// system includes

// GNSSTk

// DDBase
#include "DDBase.hpp"
#include "DataStructures.hpp"

//------------------------------------------------------------------------------------
using namespace std;
using namespace gnsstk;

//------------------------------------------------------------------------------------
Station& findStationInList(map<string,Station>& SL, string& label)
{
try {
   map<string,Station>::const_iterator it;
   it = SL.find(label);
   if(it == SL.end()) {                      // create a new Station
      Station st;
      SL[label] = st;
   }
   return SL[label];
}
catch(Exception& e) { GNSSTK_RETHROW(e); }
catch(std::exception& e) { Exception E("std except: "+string(e.what())); GNSSTK_THROW(E); }
catch(...) { Exception e("Unknown exception"); GNSSTK_THROW(e); }
}

//------------------------------------------------------------------------------------
Station::Station(void) noexcept
{
   fixed = usePRS = false;
   temp = press = rhumid = 0.0;
   time = CommonTime::BEGINNING_OF_TIME;
   pTropModel = NULL;
   TropType = "Saas";
   ant_azimuth = 0.0;
      // shove all known systems into the allowed list as there are no
      // restrictions.
   for (SatelliteSystem i : SatelliteSystemIterator())
   {
      PRS.allowedGNSS.push_back(i);
   }
}

//------------------------------------------------------------------------------------
Station::~Station(void) noexcept
{
   delete pTropModel;
}

//------------------------------------------------------------------------------------
ObsFile::ObsFile(void) noexcept
{
   name = string("");
   label = string("");
   nread = -1;
   valid = false;
   inC1 = -1;
   inP1 = -1; inP2 = -1;
   inL1 = -1; inL2 = -1;
   inD1 = -1; inD2 = -1;
   inS1 = -1; inS2 = -1;
}

//------------------------------------------------------------------------------------
ObsFile& ObsFile::operator=(const ObsFile& of) noexcept
{
   name = of.name;
   label = of.label;
   nread = of.nread;     // number of records read (-1=unopened, 0=header read)
   valid = of.valid;
   inC1 = of.inC1;
   inP1 = of.inP1; inP2 = of.inP2;
   inL1 = of.inL1; inL2 = of.inL2;
   inD1 = of.inD1; inD2 = of.inD2;
   inS1 = of.inS1; inS2 = of.inS2;
   return *this;
}

//------------------------------------------------------------------------------------
ObsFile::ObsFile(const ObsFile& of) noexcept
{
   *this = of;
}

//------------------------------------------------------------------------------------
ObsFile::~ObsFile(void)
{
   ins.clear();
   ins.close();
}

//------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------
