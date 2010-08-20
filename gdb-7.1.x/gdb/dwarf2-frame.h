/* Frame unwinder for frames with DWARF Call Frame Information.

   Copyright (C) 2003, 2004, 2005, 2007, 2008, 2009, 2010
   Free Software Foundation, Inc.

   Contributed by Mark Kettenis.

   This file is part of GDB.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

#ifndef DWARF2_FRAME_H
#define DWARF2_FRAME_H 1

struct gdbarch;
struct objfile;
struct frame_info;

/* Register rule.  */

enum dwarf2_frame_reg_rule
{
  /* Make certain that 0 maps onto the correct enum value; the
     corresponding structure is being initialized using memset zero.
     This indicates that CFI didn't provide any information at all
     about a register, leaving how to obtain its value totally
     unspecified.  */
  DWARF2_FRAME_REG_UNSPECIFIED = 0,

  /* The term "undefined" comes from the DWARF2 CFI spec which this
     code is moddeling; it indicates that the register's value is
     "undefined".  GCC uses the less formal term "unsaved".  Its
     definition is a combination of REG_UNDEFINED and REG_UNSPECIFIED.
     The failure to differentiate the two helps explain a few problems
     with the CFI generated by GCC.  */
  DWARF2_FRAME_REG_UNDEFINED,
  DWARF2_FRAME_REG_SAVED_OFFSET,
  DWARF2_FRAME_REG_SAVED_REG,
  DWARF2_FRAME_REG_SAVED_EXP,
  DWARF2_FRAME_REG_SAME_VALUE,

  /* These are defined in Dwarf3.  */
  DWARF2_FRAME_REG_SAVED_VAL_OFFSET,
  DWARF2_FRAME_REG_SAVED_VAL_EXP,

  /* These aren't defined by the DWARF2 CFI specification, but are
     used internally by GDB.  */
  DWARF2_FRAME_REG_FN,		/* Call a registered function.  */
  DWARF2_FRAME_REG_RA,		/* Return Address.  */
  DWARF2_FRAME_REG_RA_OFFSET,	/* Return Address with offset.  */
  DWARF2_FRAME_REG_CFA,		/* Call Frame Address.  */
  DWARF2_FRAME_REG_CFA_OFFSET	/* Call Frame Address with offset.  */
};

/* Register state.  */

struct dwarf2_frame_state_reg
{
  /* Each register save state can be described in terms of a CFA slot,
     another register, or a location expression.  */
  union {
    LONGEST offset;
    ULONGEST reg;
    unsigned char *exp;
    struct value *(*fn) (struct frame_info *this_frame, void **this_cache,
			 int regnum);
  } loc;
  ULONGEST exp_len;
  enum dwarf2_frame_reg_rule how;
};

/* Set the architecture-specific register state initialization
   function for GDBARCH to INIT_REG.  */

extern void dwarf2_frame_set_init_reg (struct gdbarch *gdbarch,
				       void (*init_reg) (struct gdbarch *, int,
					     struct dwarf2_frame_state_reg *,
					     struct frame_info *));

/* Set the architecture-specific signal trampoline recognition
   function for GDBARCH to SIGNAL_FRAME_P.  */

extern void
  dwarf2_frame_set_signal_frame_p (struct gdbarch *gdbarch,
				   int (*signal_frame_p) (struct gdbarch *,
							  struct frame_info *));

/* Set the architecture-specific adjustment of .eh_frame and .debug_frame
   register numbers.  */

extern void
  dwarf2_frame_set_adjust_regnum (struct gdbarch *gdbarch,
				  int (*adjust_regnum) (struct gdbarch *,
							int, int));

/* Append the DWARF-2 frame unwinders to GDBARCH's list.  */

void dwarf2_append_unwinders (struct gdbarch *gdbarch);

/* Return the frame base methods for the function that contains PC, or
   NULL if it can't be handled by the DWARF CFI frame unwinder.  */

extern const struct frame_base *
  dwarf2_frame_base_sniffer (struct frame_info *this_frame);

/* Register the DWARF CFI for OBJFILE.  */

void dwarf2_frame_build_info (struct objfile *objfile);

/* Compute the DWARF CFA for a frame.  */

CORE_ADDR dwarf2_frame_cfa (struct frame_info *this_frame);

#endif /* dwarf2-frame.h */