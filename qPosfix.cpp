
/*********************************************************************************************

    This is public domain software that was developed by or for the U.S. Naval Oceanographic
    Office and/or the U.S. Army Corps of Engineers.

    This is a work of the U.S. Government. In accordance with 17 USC 105, copyright protection
    is not available for any work of the U.S. Government.

    Neither the United States Government, nor any employees of the United States Government,
    nor the author, makes any warranty, express or implied, without even the implied warranty
    of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE, or assumes any liability or
    responsibility for the accuracy, completeness, or usefulness of any information,
    apparatus, product, or process disclosed, or represents that its use would not infringe
    privately-owned rights. Reference herein to any specific commercial products, process,
    or service by trade name, trademark, manufacturer, or otherwise, does not necessarily
    constitute or imply its endorsement, recommendation, or favoring by the United States
    Government. The views and opinions of authors expressed herein do not necessarily state
    or reflect those of the United States Government, and shall not be used for advertising
    or product endorsement purposes.
*********************************************************************************************/


/****************************************  IMPORTANT NOTE  **********************************

    Comments in this file that start with / * ! are being used by Doxygen to document the
    software.  Dashes in these comment blocks are used to create bullet lists.  The lack of
    blank lines after a block of dash preceeded comments means that the next block of dash
    preceeded comments is a new, indented bullet list.  I've tried to keep the Doxygen
    formatting to a minimum but there are some other items (like <br> and <pre>) that need
    to be left alone.  If you see a comment that starts with / * ! and there is something
    that looks a bit weird it is probably due to some arcane Doxygen syntax.  Be very
    careful modifying blocks of Doxygen comments.

*****************************************  IMPORTANT NOTE  **********************************/



#include "qPosfix.hpp"


/***************************************************************************/
/*!

  - Module :        qFixpos

  - Programmer :    Jan C. Depner

  - Date :          09/07/16

  - Purpose :       This routine converts decimal degrees to degrees, minutes,
                    seconds, and hemisphere in the form specified by 'form'.
                    This is essentially a replacement for the original fixpos
                    but it uses QStrings.

  - Arguments:
                    - degs        =   decimal degrees
                    - type        =   QPOS_LAT or QPOS_LON
                    - form        =   QPOS_HDMS, QPOS_HDM, QPOS_HD, QPOS_DMS, QPOS_DM, QPOS_D
                    - deg         =   degrees
                    - min         =   minutes
                    - sec         =   seconds
                    - hem         =   hemisphere
                    
  - Return Value:
                    - string containing the position in 'form' format

\***************************************************************************/

QString qFixpos (double degs, double *deg, double *min, double *sec, char *hem, int32_t type, int32_t form)
{
  double               abs_degs;
  QString              string;


  /*  Compute the degrees, minutes, seconds, and hemisphere.  */
    
  abs_degs = fabs (degs) * 3600.0;

  *sec = fmod (abs_degs, 60.0);
  abs_degs -= *sec;
  abs_degs /= 60.0;
  *min = fmod (abs_degs, 60.0);
  abs_degs -= *min;
  abs_degs /= 60.0;
  *deg = abs_degs;

  *deg = (int32_t) (*deg + 0.00001);
  *min = (int32_t) (*min + 0.00001);
  *sec += 0.000000001;

  if (*sec >= 60.0)
    {
      *min += 1.0;
      *sec -= 60.0;
    }

  if (*min >= 60.0)
    {
      *deg += 1.0;
      *min -= 60.0;
    }

  if (type)
    {
      if (degs < 0.0)
        {
          *hem = 'W';
        }
      else
        {
          *hem = 'E';
        }
    }
  else
    {
      if (degs < 0.0)
        {
          *hem = 'S';
        }
      else
        {
          *hem = 'N';
        }
    }


  /*  Build the output string and reset the min, sec, and hem if needed.  */

  switch (form)
    {
    case QPOS_HDMS:
      if (type)
        {
          string = QString ("%1 %2%3 %4' %5''").arg (*hem).arg ((int32_t) *deg, 3, 10, zero).arg (degC).arg ((int32_t) *min, 2, 10, zero).arg (*sec, 5, 'f', 2, zero);
        }
      else
        {
          string = QString ("%1 %2%3 %4' %5''").arg (*hem).arg ((int32_t) *deg, 2, 10, zero).arg (degC).arg ((int32_t) *min, 2, 10, zero).arg (*sec, 5, 'f', 2, zero);
        }
      break;

    case QPOS_HDM:
      *min += *sec / 60.0;
      *sec = 0.0;

      if (type)
        {
          string = QString ("%1 %2%3 %4'").arg (*hem).arg ((int32_t) *deg, 3, 10, zero).arg (degC).arg (*min, 8, 'f', 5, zero);
        }
      else
        {
          string = QString ("%1 %2%3 %4'").arg (*hem).arg ((int32_t) *deg, 2, 10, zero).arg (degC).arg (*min, 8, 'f', 5, zero);
        }
      break;

    case QPOS_HD:
      *deg += *min / 60.0 + *sec / 3600.0;
      *min = 0.0;
      *sec = 0.0;

      if (type)
        {
          string = QString ("%1 %2%3").arg (*hem).arg (*deg, 12, 'f', 8, zero).arg (degC);
        }
      else
        {
          string = QString ("%1 %2%3").arg (*hem).arg (*deg, 11, 'f', 8, zero).arg (degC);
        }
      break;

    case QPOS_DMS:
      if (degs < 0.0)
        {
          *hem = '-';
        }
      else
        {
          *hem = ' ';
        }

      if (type)
        {
          string = QString ("%1%2%3 %4' %5''").arg (*hem).arg ((int32_t) *deg, 3, 10, zero).arg (degC).arg ((int32_t) *min, 2, 10, zero).arg (*sec, 5, 'f', 2, zero);
        }
      else
        {
          string = QString ("%1%2%3 %4' %5''").arg (*hem).arg ((int32_t) *deg, 2, 10, zero).arg (degC).arg ((int32_t) *min, 2, 10, zero).arg (*sec, 5, 'f', 2, zero);
        }

      if (degs < 0.0)
        {
          if ((int32_t) *deg)
            {
              *deg = -*deg;
            }
          else if ((int32_t) *min)
            {
              *min = -*min;
            }
          else
            {
              *sec = -*sec;
            }
        }
      break;

    case QPOS_DM:
      if (degs < 0.0)
        {
          *hem = '-';
        }
      else
        {
          *hem = ' ';
        }
      *min += *sec / 60.0;
      *sec = 0.0;

      if (type)
        {
          string = QString ("%1%2%3 %4'").arg (*hem).arg ((int32_t) *deg, 3, 10, zero).arg (degC).arg (*min, 8, 'f', 5, zero);
        }
      else
        {
          string = QString ("%1%2%3 %4'").arg (*hem).arg ((int32_t) *deg, 2, 10, zero).arg (degC).arg (*min, 8, 'f', 5, zero);
        }

      if (degs < 0.0)
        {
          if ((int32_t) *deg)
            {
              *deg = -*deg;
            }
          else
            {
              *min = -*min;
            }
        }
      break;

    case QPOS_D:
      *deg += *min / 60.0 + *sec / 3600.0;
      *min = 0.0;
      *sec = 0.0;
      if (degs < 0.0)
        {
          *hem = '-';
        }
      else
        {
          *hem = ' ';
        }

      if (type)
        {
          string = QString ("%1%2%3").arg (*hem).arg (*deg, 12, 'f', 8, zero).arg (degC);
        }
      else
        {
          string = QString ("%1%2%3").arg (*hem).arg (*deg, 11, 'f', 8, zero).arg (degC);
        }

      *deg = degs;
      *min = 0.0;
      *sec = 0.0;
      break;
    }


  return (string);
}





/***************************************************************************/
/*!

  - Module :        qPosfix

  - Programmer :    Jan C. Depner

  - Date :          09/07/16

  - Purpose :       This routine converts a string containing the latitude or
                    longitude in any of the following formats to decimal
                    degrees.

                        Hemisphere Degrees Minutes Seconds.decimal
                        Hemisphere Degrees Minutes.decimal
                        Hemisphere Degrees.decimal
                        SignDegrees Minutes Seconds.decimal
                        SignDegrees Minutes.decimal
                        SignDegrees.decimal
                        Degrees° Minutes' Seconds.decimal" Hemisphere
                        Degrees° Minutes.decimal' Hemisphere
                        Degrees.decimal°

                    This is essentially a replacement for the original posfix
                    but it uses QStrings.

  - Arguments:
                    - *string     =   character string returned
                    - *degs       =   degrees decimal
                    - type        =   QPOS_LAT or QPOS_LON
                    
  - Return Value:   void

\***************************************************************************/

void qPosfix (QString string, double *degs, int32_t type)
{
  double           fdeg, fmin, fsec, f1, f2, f3;


  /*  Check for and clear sign, degree, minute, second, and/or hemisphere indicators.          */

  int32_t sign = 0;
  for (int32_t i = 0 ; i < string.length () ; i++)
    {
      /*  Set the sign flag for the string.  */

      if (type)
        {
          if (string[i] == 'W' || string[i] == 'w' || string[i] == '-')
            {
              string[i] = ' ';
              sign = 1;
            }
        }
      else
        {
          if (string[i] == 'S' || string[i] == 's' || string[i] == '-')
            {
              string[i] = ' ';
              sign = 1;
            }
        }


      //  The 65533 character is a degree symbol that comes from Google Earth.  This only happens when using Qt5.  In Qt4 it's the normal degree symbol (degC, see
      //  qPosfix.hpp).

      if (string[i] == 'n' || string[i] == 'N' || string[i] == 'e' || string[i] == 'E' || string[i] == '+' ||
          string[i] == '\'' || string[i] == '"' || string[i] == 65533 || string[i] == degC) string[i] = ' ';
    }


  QString simple_string = string.simplified ();


  fdeg = 0.0;
  fmin = 0.0;
  fsec = 0.0;
  f1 = 0.0;
  f2 = 0.0;
  f3 = 0.0;


  /*  Convert the string to degrees, minutes, and seconds.        */

  int32_t count = simple_string.count (' ');

  f1 = simple_string.section (' ', 0, 0).toDouble ();
  f2 = simple_string.section (' ', 1, 1, QString::SectionSkipEmpty).toDouble ();
  f3 = simple_string.section (' ', 2, 2, QString::SectionSkipEmpty).toDouble ();


  //  Based on the number of values scanned, compute the total degrees.  Note that this switch falls through... on purpose.

  switch (count)
    {
    case 2:
      fsec = f3 / 3600.0;

    case 1:
      fmin = f2 / 60.0;

    case 0:
      fdeg = f1;
    }

  fdeg += fmin + fsec;
  *degs = fdeg;
  if (sign) *degs = - *degs;
}
