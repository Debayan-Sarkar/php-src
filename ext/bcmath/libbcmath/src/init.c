/* init.c: bcmath library file. */
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
#include <stdbool.h>
#include <stddef.h>
#include <string.h>
#include "zend_alloc.h"

/* new_num allocates a number and sets fields to known values. */
bc_num _bc_new_num_ex(size_t length, size_t scale, bool persistent)
{
	/* PHP Change: malloc() -> pemalloc(), removed free_list code, merged n_ptr and n_value */
	bc_num temp = safe_pemalloc(1, sizeof(bc_struct) + length, scale, persistent);
	temp->n_sign = PLUS;
	temp->n_len = length;
	temp->n_scale = scale;
	temp->n_refs = 1;
	temp->n_value = (char *) temp + sizeof(bc_struct);
	memset(temp->n_value, 0, length + scale);
	return temp;
}


/* "Frees" a bc_num NUM.  Actually decreases reference count and only
   frees the storage if reference count is zero. */
void _bc_free_num_ex(bc_num *num, bool persistent)
{
	if (*num == NULL) {
		return;
	}
	(*num)->n_refs--;
	if ((*num)->n_refs == 0) {
		pefree(*num, persistent);
	}
	*num = NULL;
}


/* Initialize the number package! */

void bc_init_numbers(void)
{
	BCG(_zero_) = _bc_new_num_ex(1, 0, 1);
	BCG(_one_) = _bc_new_num_ex(1, 0, 1);
	BCG(_one_)->n_value[0] = 1;
	BCG(_two_) = _bc_new_num_ex(1, 0, 1);
	BCG(_two_)->n_value[0] = 2;
}


/* Initialize a number NUM by making it a copy of zero. */
void bc_init_num(bc_num *num)
{
	*num = bc_copy_num(BCG(_zero_));
}
