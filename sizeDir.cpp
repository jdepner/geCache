
/********************************************************************************************* 

    sizeDir.cpp

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


#include "geCacheDef.hpp"

int64_t sizeDir (const QString &path)
{
  int64_t total_size = 0;

  QFileInfo pathInfo (path);

  if (pathInfo.isDir ())
    {
      QDir dir (path);

      QFileInfoList list = dir.entryInfoList (QDir::Files | QDir::Dirs | QDir::Hidden | QDir::System| QDir::NoSymLinks | QDir::NoDotAndDotDot);

      for (int32_t i = 0; i < list.size (); i++)
        {
          QFileInfo fileInfo = list.at (i);

          if(fileInfo.isDir ())
            {
              total_size += sizeDir (fileInfo.absoluteFilePath ());
            }
          else
            {
              total_size += fileInfo.size ();
            }
        }
    }

  return (total_size);
} 
