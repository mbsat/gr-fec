#
# Copyright 2004,2005,2006,2008,2009,2010 Free Software Foundation, Inc.
# 
# This file is part of GNU Radio
# 
# GNU Radio is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 3, or (at your option)
# any later version.
# 
# GNU Radio is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
# 
# You should have received a copy of the GNU General Public License
# along with GNU Radio; see the file COPYING.  If not, write to
# the Free Software Foundation, Inc., 51 Franklin Street,
# Boston, MA 02110-1301, USA.
# 

include $(top_srcdir)/Makefile.common

# list of programs run by "make check" and "make distcheck"
#TESTS = test_all

# ----------------------------------------------------------------
# howto C++ library: libgnuradio-howto.so
# ----------------------------------------------------------------

# C/C++ headers get installed in ${prefix}/include/$(modname)
modinclude_HEADERS = \
	fec_encode_convolutional_27_bb.h \
        fec_decode_viterbi_bpsk_fb.h \
        fec_decode_viterbi_ahrpt_metop_cb.h \
        fec_decode_viterbi_ahrpt_fengyun_cb.h

lib_LTLIBRARIES = libgnuradio-fec.la

libgnuradio_fec_la_SOURCES = \
	fec_encode_convolutional_27_bb.cc \
        fec_decode_viterbi_bpsk_fb.cc \
        fec_decode_viterbi_ahrpt_metop_cb.cc \
        fec_decode_viterbi_ahrpt_fengyun_cb.cc
        
libgnuradio_fec_la_LIBADD =  
	$(GNURADIO_CORE_LA)

libgnuradio_fec_la_LDFLAGS = $(NO_UNDEFINED) $(LTVERSIONFLAGS)



