/* sub.c: bcmath library file. */
/*
    Copyright (C) 1991, 1992, 1993, 1994, 1997 Free Software Foundation, Inc.
    Copyright (C) 2000 Philip A. Nelson

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.  (LICENSE)

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to:

      The Free Software Foundation, Inc.
      59 Temple Place, Suite 330
      Boston, MA 02111-1307 USA.

    You may contact the author by:
       e-mail:  philnelson@acm.org
      us-mail:  Philip A. Nelson
                Computer Science Department, 9062
                Western Washington University
                Bellingham, WA 98226-9062

*************************************************************************/

#include "bcmath.h"
#include "private.h"
#include <stddef.h>
#include <stdbool.h>
#include <string.h>

/* Here is the full subtract routine that takes care of negative numbers.
   N2 is subtracted from N1 and the result placed in RESULT.  SCALE_MIN
   is the minimum scale for the result. */

bc_num bc_sub(bc_num n1, bc_num n2, size_t scale_min)
{
	bc_num diff = NULL;

	if (n1->n_sign != n2->n_sign) {
		diff = _bc_do_add(n1, n2, scale_min);
		diff->n_sign = n1->n_sign;
	} else {
		/* subtraction must be done. */
		/* Compare magnitudes. */
		switch (_bc_do_compare(n1, n2, false)) {
			case -1:
				/* n1 is less than n2, subtract n1 from n2. */
				diff = _bc_do_sub(n2, n1, scale_min);
				diff->n_sign = (n2->n_sign == PLUS ? MINUS : PLUS);
				break;
			case 0: {
				/* They are equal! return zero! */
				size_t res_scale = MAX (scale_min, MAX(n1->n_scale, n2->n_scale));
				diff = bc_new_num (1, res_scale);
				memset(diff->n_value, 0, res_scale + 1);
				break;
			}
			case 1:
				/* n2 is less than n1, subtract n2 from n1. */
				diff = _bc_do_sub(n1, n2, scale_min);
				diff->n_sign = n1->n_sign;
				break;
		}
	}

	return diff;
}
