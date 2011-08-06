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

GR_SWIG_BLOCK_MAGIC(fec,decode_viterbi_ahrpt_fengyun_cb);

fec_decode_viterbi_ahrpt_fengyun_cb_sptr fec_make_decode_viterbi_ahrpt_fengyun_cb (bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);

class fec_decode_viterbi_ahrpt_fengyun_cb : public gr_block
{
private:
  fec_decode_viterbi_ahrpt_fengyun_cb();
};
