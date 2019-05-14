
/********************************************************************************************* 

    copyDir.cpp

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

uint8_t copyDir (const QString &source, const QString &dest)
{
  QFileInfo sourceInfo (source);

  if (sourceInfo.isDir ())
    {
      QDir destDir (dest);

      destDir.cdUp ();

      if (!destDir.mkdir (QFileInfo (dest).fileName ())) return (false);

      QDir sourceDir (source);

      QStringList files = sourceDir.entryList (QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot | QDir::Hidden | QDir::System);

      foreach (const QString &file, files)
        {
          const QString newSource = source + SEPARATOR + file;
          const QString newDest = dest + SEPARATOR + file;

          if (!copyDir (newSource, newDest)) return (false);
        }
    }
  else
    {
      if (!QFile::copy (source, dest)) return (false);
    }

  return (true);
}
