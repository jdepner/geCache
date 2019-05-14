
/********************************************************************************************* 

    computeSize.cpp

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

void computeSize (MISC *misc, OPTIONS *options)
{
  double mheight, mwidth, center_x, center_y, az, x, y;


  //  Set the default flag for positionBuildGoogleEarth.

  misc->view_flag = true;
  misc->poly_flag = false;


  /******************************************************** Rectangle ********************************************************************/

  //  We always compute the information for a rectangle build...

  invgp (NV_A0, NV_B0, options->cache_mbr.min_y, options->cache_mbr.min_x, options->cache_mbr.max_y, options->cache_mbr.min_x, &mheight, &az);

  center_x = options->cache_mbr.min_x + (options->cache_mbr.max_x - options->cache_mbr.min_x) / 2.0;
  center_y = options->cache_mbr.min_y + (options->cache_mbr.max_y - options->cache_mbr.min_y) / 2.0;

  invgp (NV_A0, NV_B0, center_y, options->cache_mbr.min_x, center_y, options->cache_mbr.max_x, &mwidth, &az);

  QString mtr;
  mtr.sprintf ("Width = %.1f meters", mwidth);
  misc->meterWidth->setText (mtr);
  mtr.sprintf ("Height = %.1f meters", mheight);
  misc->meterHeight->setText (mtr);

  misc->build_area_mbr = options->cache_mbr;


  //  Compute the box size in degrees.

  newgp (center_y, center_x, 0.0, options->build_box_size, &y, &x);
  misc->box_size_y_deg = y - center_y;

  newgp (center_y, center_x, 90.0, options->build_box_size, &y, &x);
  misc->box_size_x_deg = x - center_x;


  //  Compute the sizes of the borders for the box size we're actually going to be moving.  There is always at least 1.25 times the defined box size in 
  //  the X direction and 1.1 times the box size in the Y direction regardless of aspect ratio in Google Earth.  I'm trying to eliminate some of the
  //  image redundancy without missing any imagery.

  int32_t x_size = (int32_t) ((float) options->build_box_size / 1.25 + 0.5);
  int32_t y_size = (int32_t) ((float) options->build_box_size / 1.1 + 0.5);

  newgp (center_y, center_x, 0.0, y_size, &y, &x);
  misc->y_border = (misc->box_size_y_deg - (y - center_y)) / 2;

  newgp (center_y, center_x, 90.0, x_size, &y, &x);
  misc->x_border = (misc->box_size_x_deg - (x - center_x)) / 2;


  //  Make the initial small area MBR for the build process.

  misc->view_area_mbr.min_y = misc->build_area_mbr.min_y;
  misc->view_area_mbr.min_x = misc->build_area_mbr.min_x;
  misc->view_area_mbr.max_y = misc->view_area_mbr.min_y + misc->box_size_y_deg;
  misc->view_area_mbr.max_x = misc->view_area_mbr.min_x + misc->box_size_x_deg;


  //  Figure out how many iterations it will take to do the build so that we can set up a progress bar.

  misc->iterations = 1;
  uint8_t done = false, direction = 0;
  NV_F64_XYMBR test_mbr = misc->view_area_mbr;

  do
     {
      switch (direction)
        {
        case 0:

          //  Move right one box

          test_mbr.min_x += misc->box_size_x_deg;


          //  If we've passed the end of the area...

          if (test_mbr.max_x >= misc->build_area_mbr.max_x)
            {
              //  Reset min_x to the previous location.

              test_mbr.min_x -= misc->box_size_x_deg;


              //  Move up one box

              test_mbr.min_y += misc->box_size_y_deg;


              //  If we've passed the top of the area, finish.

              if (test_mbr.min_y >= misc->build_area_mbr.max_y)
                {
                  done = true;
                }
              else
                {
                  direction = 1;
                }
            }

          break;

        case 1:

          //  Move left one box

          test_mbr.min_x -= misc->box_size_x_deg;


          //  If we've passed the end of the area...

          if (test_mbr.max_x <= misc->build_area_mbr.min_x)
            {
              //  Reset min_x to the previous location.

              test_mbr.min_x += misc->box_size_x_deg;


              //  Move up one box

              test_mbr.min_y += misc->box_size_y_deg;


              //  If we've passed the top of the area, finish.

              if (test_mbr.min_y >= misc->build_area_mbr.max_y)
                {
                  done = true;
                }
              else
                {
                  direction = 0;
                }
            }

          break;
        }

      test_mbr.max_y = test_mbr.min_y + misc->box_size_y_deg;
      test_mbr.max_x = test_mbr.min_x + misc->box_size_x_deg;

      misc->iterations++;

    } while (!done);


  //  Compute a rough estimate of how long this will take.

  misc->total_rect_time = misc->iterations * options->cache_update_frequency + 20;


  int32_t hour = misc->total_rect_time / 3600;
  int32_t minute = (misc->total_rect_time / 60) % 60;
  int32_t second = misc->total_rect_time % 60;

  misc->rectEstTime->setText (geCache::tr ("Estimated time to build = %1:%2:%3").arg (hour, 2, 10, zero).arg (minute, 2, 10, zero).arg (second, 2, 10, zero));



  /******************************************************** Polygon *********************************************************************/

  //  Check to see if we're doing a polygon build (if we're on the polygon tab and we have polygon points).

  if (options->shape_tab == POLY_TAB && options->polygon.size ())
    {
      misc->poly_flag = true;


      //  Compute the build MBR based on the cache area polygon.
 
      misc->build_area_mbr.min_x = 999.0;
      misc->build_area_mbr.max_x = -999.0;
      misc->build_area_mbr.min_y = 999.0;
      misc->build_area_mbr.max_y = -999.0;

      for (uint32_t i = 0 ; i < options->polygon.size () ; i++)
        {
          misc->build_area_mbr.min_x = qMin (options->polygon[i].x, misc->build_area_mbr.min_x);
          misc->build_area_mbr.max_x = qMax (options->polygon[i].x, misc->build_area_mbr.max_x);
          misc->build_area_mbr.min_y = qMin (options->polygon[i].y, misc->build_area_mbr.min_y);
          misc->build_area_mbr.max_y = qMax (options->polygon[i].y, misc->build_area_mbr.max_y);
        }

      invgp (NV_A0, NV_B0, misc->build_area_mbr.min_y, misc->build_area_mbr.min_x, misc->build_area_mbr.max_y, misc->build_area_mbr.min_x, &mheight, &az);

      center_x = misc->build_area_mbr.min_x + (misc->build_area_mbr.max_x - misc->build_area_mbr.min_x) / 2.0;
      center_y = misc->build_area_mbr.min_y + (misc->build_area_mbr.max_y - misc->build_area_mbr.min_y) / 2.0;

      invgp (NV_A0, NV_B0, center_y, misc->build_area_mbr.min_x, center_y, misc->build_area_mbr.max_x, &mwidth, &az);


      //  Compute the box size in degrees.

      newgp (center_y, center_x, 0.0, options->build_box_size, &y, &x);
      misc->box_size_y_deg = y - center_y;

      newgp (center_y, center_x, 90.0, options->build_box_size, &y, &x);
      misc->box_size_x_deg = x - center_x;


      //  Compute the sizes of the borders for the box size we're actually going to be moving.  There is always at least 1.25 times the defined box size in 
      //  the X direction and 1.1 times the box size in the Y direction regardless of aspect ratio in Google Earth.  I'm trying to eliminate some of the
      //  image redundancy without missing any imagery.

      int32_t x_size = (int32_t) ((float) options->build_box_size / 1.25 + 0.5);
      int32_t y_size = (int32_t) ((float) options->build_box_size / 1.1 + 0.5);

      newgp (center_y, center_x, 0.0, y_size, &y, &x);
      misc->y_border = (misc->box_size_y_deg - (y - center_y)) / 2;

      newgp (center_y, center_x, 90.0, x_size, &y, &x);
      misc->x_border = (misc->box_size_x_deg - (x - center_x)) / 2;


      //  Make the initial small area MBR for the build process.

      misc->view_area_mbr.min_y = misc->build_area_mbr.min_y;
      misc->view_area_mbr.min_x = misc->build_area_mbr.min_x;
      misc->view_area_mbr.max_y = misc->view_area_mbr.min_y + misc->box_size_y_deg;
      misc->view_area_mbr.max_x = misc->view_area_mbr.min_x + misc->box_size_x_deg;


      //  We need to set misc->view_flag to false if the first box is completely out of the polygon.

      NV_F64_COORD2 mbr_poly[4];

      mbr_poly[0].x = misc->view_area_mbr.min_x;
      mbr_poly[0].y = misc->view_area_mbr.min_y;
      mbr_poly[1].x = misc->view_area_mbr.min_x;
      mbr_poly[1].y = misc->view_area_mbr.max_y;
      mbr_poly[2].x = misc->view_area_mbr.max_x;
      mbr_poly[2].y = misc->view_area_mbr.max_y;
      mbr_poly[3].x = misc->view_area_mbr.max_x;
      mbr_poly[3].y = misc->view_area_mbr.min_y;

      int32_t poly_size = (int32_t) options->polygon.size ();

      if (!polygon_intersection (mbr_poly, 4, options->polygon.data (), poly_size)) misc->view_flag = false;


      //  Figure out how many iterations it will take to do the build so that we can set up a progress bar.

      misc->poly_iterations = 1;
      uint8_t done = false, direction = 0;
      NV_F64_XYMBR test_mbr = misc->view_area_mbr;


      /*  Debuggery

      FILE *fp;
      char name[1024];
      sprintf(name, "000_MBR.are");
      fp = fopen (name, "w");
      fprintf(fp,"%.11f,%.11f\n",misc->build_area_mbr.min_y,misc->build_area_mbr.min_x);
      fprintf(fp,"%.11f,%.11f\n",misc->build_area_mbr.max_y,misc->build_area_mbr.min_x);
      fprintf(fp,"%.11f,%.11f\n",misc->build_area_mbr.max_y,misc->build_area_mbr.max_x);
      fprintf(fp,"%.11f,%.11f\n",misc->build_area_mbr.min_y,misc->build_area_mbr.max_x);
      fclose (fp);

      sprintf(name, "Area_000.are");
      fp = fopen (name, "w");
      fprintf(fp,"%.11f,%.11f\n",test_mbr.min_y,test_mbr.min_x);
      fprintf(fp,"%.11f,%.11f\n",test_mbr.max_y,test_mbr.min_x);
      fprintf(fp,"%.11f,%.11f\n",test_mbr.max_y,test_mbr.max_x);
      fprintf(fp,"%.11f,%.11f\n",test_mbr.min_y,test_mbr.max_x);
      fclose (fp);

      */

      do
        {
          switch (direction)
            {
            case 0:

              //  Move right one box

              test_mbr.min_x += misc->box_size_x_deg;


              //  If we've passed the end of the area...

              if (test_mbr.max_x >= misc->build_area_mbr.max_x)
                {
                  //  Reset min_x to the previous location.

                  test_mbr.min_x -= misc->box_size_x_deg;


                  //  Move up one box

                  test_mbr.min_y += misc->box_size_y_deg;


                  //  If we've passed the top of the area, finish.

                  if (test_mbr.min_y >= misc->build_area_mbr.max_y)
                    {
                      done = true;
                    }
                  else
                    {
                      direction = 1;
                    }
                }

              break;

            case 1:

              //  Move left one box

              test_mbr.min_x -= misc->box_size_x_deg;


              //  If we've passed the end of the area...

              if (test_mbr.max_x <= misc->build_area_mbr.min_x)
                {
                  //  Reset min_x to the previous location.

                  test_mbr.min_x += misc->box_size_x_deg;


                  //  Move up one box

                  test_mbr.min_y += misc->box_size_y_deg;


                  //  If we've passed the top of the area, finish.

                  if (test_mbr.min_y >= misc->build_area_mbr.max_y)
                    {
                      done = true;
                    }
                  else
                    {
                      direction = 0;
                    }
                }

              break;
            }

          test_mbr.max_y = test_mbr.min_y + misc->box_size_y_deg;
          test_mbr.max_x = test_mbr.min_x + misc->box_size_x_deg;


          //  Count the boxes that will actually get viewed.

          NV_F64_COORD2 mbr_poly[4];

          mbr_poly[0].x = test_mbr.min_x;
          mbr_poly[0].y = test_mbr.min_y;
          mbr_poly[1].x = test_mbr.min_x;
          mbr_poly[1].y = test_mbr.max_y;
          mbr_poly[2].x = test_mbr.max_x;
          mbr_poly[2].y = test_mbr.max_y;
          mbr_poly[3].x = test_mbr.max_x;
          mbr_poly[3].y = test_mbr.min_y;

          int32_t poly_size = (int32_t) options->polygon.size ();

          if (polygon_intersection (mbr_poly, 4, options->polygon.data (), poly_size))
            {
              /*  Debuggery

              sprintf(name, "Area_%03d.are", misc->poly_iterations);
              fp = fopen (name, "w");
              fprintf(fp,"%.11f,%.11f\n",test_mbr.min_y,test_mbr.min_x);
              fprintf(fp,"%.11f,%.11f\n",test_mbr.max_y,test_mbr.min_x);
              fprintf(fp,"%.11f,%.11f\n",test_mbr.max_y,test_mbr.max_x);
              fprintf(fp,"%.11f,%.11f\n",test_mbr.min_y,test_mbr.max_x);
              fclose (fp);

              */

              misc->poly_iterations++;
            }

        } while (!done);


      //  Compute a rough estimate of how long this will take.

      misc->total_poly_time = misc->poly_iterations * options->cache_update_frequency + 20;


      int32_t hour = misc->total_poly_time / 3600;
      int32_t minute = (misc->total_poly_time / 60) % 60;
      int32_t second = misc->total_poly_time % 60;

      misc->polyEstTime->setText (geCache::tr ("Estimated time to build = %1:%2:%3").arg (hour, 2, 10, zero).arg (minute, 2, 10, zero).arg (second, 2, 10, zero));
      misc->numBoxes->setText (QString ("Number of areas = %1").arg (misc->poly_iterations));
    }
}
