/* -*- c++ -*- */
/*
 * Copyright 2008 Free Software Foundation, Inc.
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
#ifndef INCLUDED_FEC_ENCODE_CONVOLUTIONAL_27_BB_H
#define INCLUDED_FEC_ENCODE_CONVOLUTIONAL_27_BB_H

#include <gr_sync_interpolator.h>


class fec_encode_convolutional_27_bb;

typedef boost::shared_ptr<fec_encode_convolutional_27_bb> fec_encode_convolutional_27_bb_sptr;

fec_encode_convolutional_27_bb_sptr fec_make_encode_convolutional_27_bb();


class fec_encode_convolutional_27_bb : public gr_sync_interpolator
{
private:
  friend fec_encode_convolutional_27_bb_sptr fec_make_encode_convolutional_27_bb();

  fec_encode_convolutional_27_bb();
  unsigned char d_encstate;


 public:
  ~fec_encode_convolutional_27_bb();

  int work (int noutput_items,
	    gr_vector_const_void_star &input_items,
	    gr_vector_void_star &output_items);
};

#endif /* INCLUDED_FEC_ENCODE_CONVOLUTIONAL_27_BB_H */
