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
#ifndef INCLUDED_FEC_DECODE_VITERBI_BPSK_FB_H
#define INCLUDED_FEC_DECODE_VITERBI_BPSK_FB_H


#define debug_paket_decoder


#define TestBitsLen 1024 


#include <gr_block.h>

extern "C" {
#include <./viterbi_lib/viterbi.h>
}


class fec_decode_viterbi_bpsk_fb;

typedef boost::shared_ptr<fec_decode_viterbi_bpsk_fb> fec_decode_viterbi_bpsk_fb_sptr;

fec_decode_viterbi_bpsk_fb_sptr fec_make_decode_viterbi_bpsk_fb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);


class fec_decode_viterbi_bpsk_fb : public gr_block
{
  friend fec_decode_viterbi_bpsk_fb_sptr fec_make_decode_viterbi_bpsk_fb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);

  fec_decode_viterbi_bpsk_fb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after);
  

  bool               d_sync_check;
  float	             d_ber_threshold;
  int	     	     d_insync_after;
  int	     	     d_outsync_after;
  int                d_reset_after;

  float ber_calc(struct viterbi_state *state0, struct viterbi_state *state1, unsigned int symsnr, unsigned char *insymbols);
 
  void do_reset();
  void enter_idle();
  void enter_syncing();
  void enter_synced();




  int            d_mettab[2][256];			//metric table for continuous decoder

  struct viterbi_state d_state0[64];			//main viterbi decoder state 0 memory
  struct viterbi_state d_state1[64];			//main viterbi decoder state 1 memory

   


  unsigned char  d_viterbi_in[4];			
  int            d_count;	
  unsigned int   d_shift;      
  bool           d_even_bit; 			
    


  struct viterbi_state d_00_st0[64];				
  struct viterbi_state d_00_st1[64];				

  struct viterbi_state d_180_st0[64];				
  struct viterbi_state d_180_st1[64];				  


  float         d_ber[2];
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


#endif /* INCLUDED_FEC_DECODE_VITERBI_BPSK_FB_H */
