
/********************************************************************************************* 

    env_in_out.cpp

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


#include "geCache.hpp"


double settings_version = 1.00;


/*!
  These functions store and retrieve the program settings (environment) from a .ini file.  On both Linux and Windows
  the file will be called geCache.ini.  On Linux, the file will be stored in your $HOME directory.  On Windows, it
  will be stored in your $USERPROFILE folder.  If you make a change to the way a variable is used and you want to
  force the defaults to be restored just change the settings_version to a newer number (I've been using the program
  version number from version.hpp - which you should be updating EVERY time you make a change to the program!).  You
  don't need to change the settings_version though unless you want to force the program to go back to the defaults
  (which can annoy your users).  So, the settings_version won't always match the program version.
*/

uint8_t envin (OPTIONS *options)
{
  double saved_version = 0.0;


  //  Get the INI file name

#ifdef _MSC_VER
  QString ini_file = QString (getenv ("USERPROFILE")) + "/geCache.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/geCache.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("geCache");


  saved_version = settings.value (QString ("settings version"), saved_version).toDouble ();


  //  If the settings version has changed we need to leave the values at the new defaults since they may have changed.

  if (settings_version != saved_version) return (false);


  options->position_form = settings.value (QString ("position form"), options->position_form).toInt ();

  options->stash_dir = settings.value (QString ("stash directory"), options->stash_dir).toString ();

  options->cache_mbr.min_y = settings.value (QString ("cache south boundary latitude"), options->cache_mbr.min_y).toDouble ();
  options->cache_mbr.max_y = settings.value (QString ("cache north boundary latitude"), options->cache_mbr.max_y).toDouble ();
  options->cache_mbr.min_x = settings.value (QString ("cache west boundary longitude"), options->cache_mbr.min_x).toDouble ();
  options->cache_mbr.max_x = settings.value (QString ("cache east boundary longitude"), options->cache_mbr.max_x).toDouble ();

  int32_t size = settings.beginReadArray ("Polygon points");

  try
    {
      options->polygon.resize (size);
    }
  catch (std::bad_alloc&)
    {
      QMessageBox::critical (0, geCache::tr ("geCache"), geCache::tr ("Unable to allocate polygon point memory!  Reason : %1").arg (strerror (errno)));
      exit (-1);
    }


  for (int32_t i = 0 ; i < size ; i++)
    {
      settings.setArrayIndex (i);

      options->polygon[i].y = settings.value ("lat").toDouble ();
      options->polygon[i].x = settings.value ("lon").toDouble ();
    }

  settings.endArray ();


  options->cache_update_frequency = settings.value (QString ("cache update frequency"), options->cache_update_frequency).toInt ();
  options->build_box_size = settings.value (QString ("build box size"), options->build_box_size).toInt ();
  options->icon_size = settings.value (QString ("toolbar icon size"), options->icon_size).toInt ();
  options->start_tab = settings.value (QString ("start tab"), options->start_tab).toInt ();
  options->shape_tab = settings.value (QString ("shape tab"), options->shape_tab).toInt ();

  options->ge_name = settings.value (QString ("google earth name"), options->ge_name).toString ();

  int32_t red = settings.value (QString ("warning color/red"), options->warning_color.red ()).toInt ();
  int32_t green = settings.value (QString ("warning color/green"), options->warning_color.green ()).toInt ();
  int32_t blue = settings.value (QString ("warning color/blue"), options->warning_color.blue ()).toInt ();
  int32_t alpha = settings.value (QString ("warning color/alpha"), options->warning_color.alpha ()).toInt ();
  options->warning_color.setRgb (red, green, blue, alpha);

  QString defaultFont = options->font.toString ();
  QString fontString = settings.value (QString ("font"), defaultFont).toString ();
  options->font.fromString (fontString);


  options->window_width = settings.value (QString ("width"), options->window_width).toInt ();
  options->window_height = settings.value (QString ("height"), options->window_height).toInt ();
  options->window_x = settings.value (QString ("x position"), options->window_x).toInt ();
  options->window_y = settings.value (QString ("y position"), options->window_y).toInt ();


  settings.endGroup ();


  return (true);
}


void envout (OPTIONS *options)
{
  //  Get the INI file name

#ifdef _MSC_VER
  QString ini_file = QString (getenv ("USERPROFILE")) + "/geCache.ini";
#else
  QString ini_file = QString (getenv ("HOME")) + "/geCache.ini";
#endif

  QSettings settings (ini_file, QSettings::IniFormat);
  settings.beginGroup ("geCache");


  settings.setValue (QString ("settings version"), settings_version);

  settings.setValue (QString ("position form"), options->position_form);

  settings.setValue (QString ("stash directory"), options->stash_dir);

  settings.setValue (QString ("cache south boundary latitude"), options->cache_mbr.min_y);
  settings.setValue (QString ("cache north boundary latitude"), options->cache_mbr.max_y);
  settings.setValue (QString ("cache west boundary longitude"), options->cache_mbr.min_x);
  settings.setValue (QString ("cache east boundary longitude"), options->cache_mbr.max_x);

  if (options->polygon.size ())
    {
      settings.beginWriteArray ("Polygon points");

      for (uint32_t i = 0 ; i < options->polygon.size () ; i++)
        {
          settings.setArrayIndex (i);
          settings.setValue ("lat", options->polygon.at (i).y);
          settings.setValue ("lon", options->polygon.at (i).x);
        }
      settings.endArray ();
    }

  settings.setValue (QString ("cache update frequency"), options->cache_update_frequency);
  settings.setValue (QString ("build box size"), options->build_box_size);
  settings.setValue (QString ("toolbar icon size"), options->icon_size);
  settings.setValue (QString ("start tab"), options->start_tab);
  settings.setValue (QString ("shape tab"), options->shape_tab);

  settings.setValue (QString ("google earth name"), options->ge_name);

  settings.setValue (QString ("warning color/red"), options->warning_color.red ());
  settings.setValue (QString ("warning color/green"), options->warning_color.green ());
  settings.setValue (QString ("warning color/blue"), options->warning_color.blue ());
  settings.setValue (QString ("warning color/alpha"), options->warning_color.alpha ());

  QString fontString = options->font.toString ();
  settings.setValue (QString ("font"), fontString);


  settings.setValue (QString ("width"), options->window_width);
  settings.setValue (QString ("height"), options->window_height);
  settings.setValue (QString ("x position"), options->window_x);
  settings.setValue (QString ("y position"), options->window_y);


  settings.endGroup ();
}
