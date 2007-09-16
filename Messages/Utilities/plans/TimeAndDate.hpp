// Oliver Kullmann, 2.12.2006 (Swansea)
/* Copyright 2006 - 2007 Oliver Kullmann
This file is part of the OKlibrary. OKlibrary is free software; you can redistribute
it and/or modify it under the terms of the GNU General Public License as published by
the Free Software Foundation and included in this library; either version 3 of the
License, or any later version. */

/*!
  \file Messages/Utilities/plans/TimeAndDate.hpp
  \brief Plans for the messages-utility module on time-and-date components

  \todo Time and Date:
  Representations of time and date should move from General to Messages.

  \todo Improving Messages::Utilities::TimeDateStandardImplementation
  On the one hand it would be nice if we could specify different levels (say, the
  given implementation as "full", one implementation without timezone, seconds and day as "basic",
  and one implementation with full names for day and month (possibly also with day-number and
  week-number) as "extensive"), but since this is not provided by the standard, we had to define it
  all ourselves (that is, the order of the components). So perhaps the given implementation
  is a reasonable compromise. (But the best thing would be if the format string
  could be determined, and then one would simply replace the abbreviated forms by
  their full forms, and potentially add something.)

*/

