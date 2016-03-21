#
# (C) Copyright 2016, TP-Link Inc, konstantin.mauch@tp-link.com
# 
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License as
# published by the Free Software Foundation; either version 2 of
# the License, or (at your option) any later version.
# 
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston,
# MA 02111-1307 USA
#

ifeq ($(strip $(platform)),)
$(error "Define 'platform', as one of those: 'platform=PC', 'platform=DRAGONBALL' ")
endif

ifeq ($(strip $(platform)),PC)
	PREFIX=
	CFLAGS=-O3

	OBJS= modsim.o datastruct.o hal_x86.o
	GRBG=*.o *~ m
else
	ifeq ($(strip $(platform)),DRAGONBALL)
		# Prefix for UCLIBC crosscompiler
		PREFIX=m68k-pic-coff-
		CFLAGS=-O3 -DUCSIMM -DQUASIFLOAT -I/opt/uClinux/m68k-pic-coff/include

		OBJS= modsim.o datastruct.o hal_m68k.o port_d.o
		GRBG=*.o *~ *.coff m
	endif
endif

# (cross-)compiler
CC=$(PREFIX)gcc
LD=$(PREFIX)ld

# Excessive debug info not needed when program is ready. Spoils 'realtime' operating mode. Keep commented-out.
# CFLAGS+=-DDEBUG_DATA

# Checking Data-IN (backward data stream on D-); expected to be same as X(CH2) in raw data (CSV file) 
# CFLAGS+= -DDIN_FEEDBACK

.o: .s
	$(CC) $(ASMFLAGS)   -o $@ -c $< 

.o: .c
	$(CC) $(CFLAGS)   -o $@ -c $< 

all:	m


m: $(OBJS)
	$(CC) $(CFLAGS)  -o m $(OBJS)

clean:
	rm $(GRBG)
