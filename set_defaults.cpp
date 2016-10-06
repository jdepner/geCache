
/********************************************************************************************* 

    set_defaults.cpp

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

void set_defaults (MISC *misc, OPTIONS *options, uint8_t reset)
{
  if (!reset)
    {
#ifdef _MSC_VER
      misc->process_id = _getpid ();
#else
      misc->process_id = getpid ();
#endif

      misc->ge_linked = 0;
      misc->second_count = 0;
    }

  options->cache_mbr.min_x = -81.63642;
  options->cache_mbr.min_y = 28.29890;
  options->cache_mbr.max_x = -81.48429;
  options->cache_mbr.max_y = 28.45114;
  options->position_form = 0;
  options->font = QApplication::font ();
  options->stash_dir = ".";
  options->build_box_size = 4000;
  options->cache_update_frequency = 6;
  options->icon_size = 32;
  options->warning_color = QColor (255, 0, 0, 255);
  options->start_tab = ABOUT_TAB;
  options->shape_tab = RECT_TAB;
  options->polygon.clear ();
  options->window_width = 700;
  options->window_height = 700;
  options->window_x = 0;
  options->window_y = 0;


  //  I'm going to try to figure out what your Google Earth executable is called and where your GoogleEarth folder (Windows) or
  //  .googleearth/Cache directory (Linux) is located.

#ifdef _MSC_VER

  //  On Windows it's always called (AFAIK) googleearth.exe so we'll just leave it at that

  options->ge_name = "googleearth.exe";


  //  If there is no USERPROFILE environment variable we're kinda SOL.

  if (getenv ("USERPROFILE") != NULL)
    {
      //  On Windows the Google Earth folder is usually either $USERPROFILE\AppData\Local\Google\GoogleEarth or $USERPROFILE\AppData\LocalLow\Google\GoogleEarth.

      options->ge_dir = QString (getenv ("USERPROFILE")) + "\\AppData\\Local\\Google\\GoogleEarth";


      //  If we don't find Local\Google\GoogleEarth we'll go with LocalLow\Google\GoogleEarth and let the user try to find it.

      if (!QFile (options->ge_dir).exists ()) options->ge_dir = QString (getenv ("USERPROFILE")) + "\\AppData\\LocalLow\\Google\\GoogleEarth";
    }
  else
    {
      //  This should NEVER happen

      QMessageBox::warning (0, geCache::tr ("geCache Error"),
                            geCache::tr ("$USERPROFILE environment variable not set!\nYou will have to set your Google Earth folder name manually in the <b>Preferences</b> tab."));

      options->ge_dir = "AppData\\Local\\Google\\GoogleEarth";
    }

#else

  //  On Linux it could be googleearth or google-earth

  options->ge_name = "google-earth";

  QString ge_exe = QStandardPaths::findExecutable (options->ge_name);


  //  If we don't find google-earth we'll just go with googleearth and let the user try to find it.

  if (ge_exe.isEmpty ()) options->ge_name = "googleearth";


  //  If there is no HOME environment variable we're kinda SOL.

  if (getenv ("HOME") != NULL)
    {
      //  On Linux the Google Earth directory is always called (AFAIK) $HOME/.googleearth/Cache.

      options->ge_dir = QString (getenv ("HOME")) + "/.googleearth/Cache";
    }
  else
    { 
      //  This should NEVER happen

      QMessageBox::warning (0, geCache::tr ("geCache Error"),
                            geCache::tr ("$HOME environment variable not set!\nYou will have to set your Google Earth folder name manually in the <b>Preferences</b> tab."));

      options->ge_dir = ".googleearth/Cache";
    }

#endif

}
