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
#ifndef INCLUDED_FEC_DECODE_VITERBI_AHRPT_METOP_CB_H
#define INCLUDED_FEC_DECODE_VITERBI_AHRPT_METOP_CB_H


#define debug_paket_decoder


#define TestBitsLen 1024 


#include <gr_block.h>

//extern "C" {
//#include <./viterbi_lib/viterbi.h>
//}
#include <fec_decode_viterbi_bpsk_fb.h>

class fec_decode_viterbi_ahrpt_metop_cb;

typedef boost::shared_ptr<fec_decode_viterbi_ahrpt_metop_cb> fec_decode_viterbi_ahrpt_metop_cb_sptr;

fec_decode_viterbi_ahrpt_metop_cb_sptr fec_make_decode_viterbi_ahrpt_metop_cb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);


class fec_decode_viterbi_ahrpt_metop_cb : public gr_block
{
  friend fec_decode_viterbi_ahrpt_metop_cb_sptr fec_make_decode_viterbi_ahrpt_metop_cb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);

  fec_decode_viterbi_ahrpt_metop_cb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);
  

  bool               d_sync_check;
  float	             d_ber_threshold;
  int	     	     d_insync_after;
  int	     	     d_outsync_after;
  int                d_reset_after;

  float ber_calc1(struct viterbi_state *state0, struct viterbi_state *state1, unsigned int symsnr, unsigned char *insymbols_I, unsigned char *insymbols_Q);
  void  phase_move_two(unsigned char phase_state, unsigned int symsnr, unsigned char *in_I, unsigned char *in_Q, unsigned char *out_I, unsigned char *out_Q);

  void  do_reset();
  void  enter_idle();
  void  enter_syncing();
  void  enter_synced();




  int            d_mettab[2][256];			//metric table for continuous decoder

  struct viterbi_state d_state0[64];			//main viterbi decoder state 0 memory
  struct viterbi_state d_state1[64];			//main viterbi decoder state 1 memory

   


  unsigned char  d_viterbi_in[4];			
  bool           d_viterbi_enable;	
  unsigned int   d_shift, d_shift_main_decoder;
  unsigned int   d_phase;
  bool           d_valid_ber_found;
        
  unsigned char  d_bits, d_sym_count; 			
  bool           d_even_symbol_last, d_curr_is_even, d_even_symbol; 


  struct viterbi_state d_00_st0[64];				
  struct viterbi_state d_00_st1[64];				

  struct viterbi_state d_180_st0[64];				
  struct viterbi_state d_180_st1[64];				  


  float         d_ber[4][8];
  unsigned int  d_chan_len;				//BER test input syms  
  unsigned char d_valid_packet_count;      		//count packets with valid BER
  unsigned char d_invalid_packet_count;
  unsigned char d_state;
      
public:

  void forecast(int noutput_items, gr_vector_int &ninput_items_required);

  int general_work(int noutput_items,
		   gr_vector_int &ninput_items,
		   gr_vector_const_void_star &input_items,
		   gr_vector_void_star &output_items);
};


#endif /* INCLUDED_FEC_DECODE_VITERBI_AHRPT_METOP_CB_H */
