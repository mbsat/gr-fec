/* -*- c++ -*- */
/*
 * Copyright 2008 Free Software Foundation, Inc.
 * 
 * This file is part of GNU Radio
 * 
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

%include "gnuradio.i"			// the common stuff

%{
#include <fec_encode_convolutional_27_bb.h>
#include <fec_decode_viterbi_bpsk_fb.h>
#include <fec_decode_viterbi_ahrpt_metop_cb.h>
#include <fec_decode_viterbi_ahrpt_fengyun_cb.h>
%}

%include "fec_encode_convolutional_27_bb.i"
%include "fec_decode_viterbi_bpsk_fb.i"
%include "fec_decode_viterbi_ahrpt_metop_cb.i"
%include "fec_decode_viterbi_ahrpt_fengyun_cb.i"
