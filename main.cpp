
/********************************************************************************************* 

    main.cpp

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


/*  geCache  */

#include "geCache.hpp"


int
main (int argc, char **argv)
{
    QApplication a (argc, argv);

#ifdef _MSC_VER
    a.setStyle (QStyleFactory::create ("Windows"));
#else
    a.setStyle (QStyleFactory::create ("Plastique"));
#endif

    geCache *ac = new geCache ();

    ac->setWindowTitle (VERSION);

    ac->show ();

    return a.exec ();
}
