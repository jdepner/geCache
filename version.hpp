
/********************************************************************************************* 

    version.hpp

    Copyright (c) 2016, Jan C. Depner


    This file is part of geCache.

    geCache is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    geCache is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with geCache.  If not, see <http://www.gnu.org/licenses/>.

*********************************************************************************************/


#ifndef VERSION

#define     VERSION     "PFM Software - geCache V1.05 - 01/15/21"

#endif

/*! <pre>


    Version 1.00
    Jan C. Depner (PFM Software)
    10/01/16

    First working version.


    Version 1.01
    Jan C. Depner (PFM Software)
    10/09/16

    - Fixed bug when building cache with polygon when first cache sub-area is not
      in the polygon.
    - Fixed second counter bug introduced when switched to half second update.
    - Cleaned up remaining time calculation.
    - Save and load cache now open the file dialog in the last used save/load directory.


    Version 1.02
    Jan C. Depner (PFM Software)
    04/25/17

    - Saves the bounds of the rectangle or polygon to an area file (same name as the saved cache directory plus a .are extension) along 
      with the saved cache directory.
    - Loads the bounds of the rectangle or polygon from an area file along with the saved cache directory.  This allows you to reload
      a cached area, modify the bounds, build the cache, and then save it to the same or a new name.


    Version 1.03
    Jan C. Depner (PFM Software)
    05/11/19

    - Now also saves the bounds of the rectangle or polygon to a kml file (in addition to the area (.are) file) along 
      with the saved cache directory.


    Version 1.04
    Jan C. Depner (PFM Software)
    05/13/19

    - Got rid of the generic area (.are) file.  It was redundant after writing the kml area file.


    Version 1.05
    Jan C. Depner (PFM Software)
    01/15/21

    - Fixed the NW corner button.  I wasn't checking for bounds_clicked = NO_BOUNDS, I was checking
      for bounds_clicked being non-zero.  NW corner is zero, DOH!

</pre>*/
