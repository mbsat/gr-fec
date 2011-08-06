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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

//#define general_work_debug
//#define write_debugfile
//#define debug_p


#include <fec_decode_viterbi_ahrpt_metop_cb.h>
#include <gr_io_signature.h>
#include <stdio.h>

#define ST_IDLE      0
#define ST_SYNCING   1
#define ST_SYNCED    2



fec_decode_viterbi_ahrpt_metop_cb_sptr 
fec_make_decode_viterbi_ahrpt_metop_cb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after)
{
  return gnuradio::get_initial_sptr(new fec_decode_viterbi_ahrpt_metop_cb(sync_check, ber_threshold, insync_after, outsync_after, reset_after));
}

fec_decode_viterbi_ahrpt_metop_cb::fec_decode_viterbi_ahrpt_metop_cb(bool sync_check, float ber_threshold,  int insync_after, int outsync_after, int reset_after)
  : gr_block("decode_viterbi_ahrpt_metop_cb",
		      gr_make_io_signature(1, 1, sizeof(gr_complex)),
		      gr_make_io_signature(1, 1, sizeof(char))),
	     d_sync_check(sync_check),
	     d_ber_threshold(ber_threshold),
	     d_insync_after(insync_after),
	     d_outsync_after(outsync_after),
             d_reset_after(reset_after)  

{
    float RATE = 0.5;
    float ebn0 = 12.0;
    float esn0 = RATE*pow(10.0, ebn0/10);
    gen_met(d_mettab, 100, esn0, 0.0, 256);
    
    do_reset();
    enter_idle();

}
   
void fec_decode_viterbi_ahrpt_metop_cb::forecast(int noutput_items, gr_vector_int &ninput_items_required)
{
   ninput_items_required[0] = 2048;
}



//*****************************************************************************
//    DO DECODER RESET TO ZERO STATE
//*****************************************************************************
void
fec_decode_viterbi_ahrpt_metop_cb::do_reset() { 
     d_valid_packet_count = 0;
     d_invalid_packet_count = 0;   
     d_chan_len = TestBitsLen;

     viterbi_chunks_init(d_state0);        //main viterbi decoder state memory
     viterbi_chunks_init(d_00_st0);
     viterbi_chunks_init(d_180_st0);
     enter_idle(); 

}
//#############################################################################

//*****************************************************************************
//    ENTER idle state
//*****************************************************************************
void
fec_decode_viterbi_ahrpt_metop_cb::enter_idle() { 
     d_state = ST_IDLE;
     d_valid_packet_count = 0;
     d_shift = 0;
     d_even_symbol = true;
     d_bits = 0;
     d_sym_count = 0;
     d_even_symbol = true;
}
//#############################################################################


//*****************************************************************************
//    ENTER syncing state
//*****************************************************************************
void
fec_decode_viterbi_ahrpt_metop_cb::enter_syncing() { 
     d_state = ST_SYNCING;
     d_valid_packet_count = 0;
}
//#############################################################################

//*****************************************************************************
//    ENTER synced state
//*****************************************************************************
void
fec_decode_viterbi_ahrpt_metop_cb::enter_synced() { 
     d_state = ST_SYNCED;
     d_invalid_packet_count = 0;
}
//#############################################################################


//*****************************************************************************
//    VITERBI DECODER, calculate BER between hard input bits and decode-encoded bits
//*****************************************************************************
float
fec_decode_viterbi_ahrpt_metop_cb::ber_calc(
				struct viterbi_state *state0,   //state 0 viterbi decoder
				struct viterbi_state *state1,   //state 1 viterbi decoder
				unsigned int symsnr,  	        
				unsigned char *insymbols_I, unsigned char *insymbols_Q ) {				
  unsigned char   viterbi_in[4];		  
  unsigned char   insymbols_interleaved_depunctured[symsnr*4];
  unsigned char   decoded_data[symsnr];			         //viterbi decoded data buffer  [symsnr is many more as necessary]
  unsigned int    decoded_data_count = 0;			
  unsigned char  *p_decoded_data = &decoded_data[0];     	//pointer to viterbi decoded data

  unsigned char   encoded_data[symsnr*2];                   	//encoded data buffer

  unsigned int    difference_count;                        	//count of diff. between reencoded data and input symbols
  float           ber; 
  unsigned char   symbol_count =0;
  unsigned int    bits = 0;

   
  //decode test packet of incoming symbols 
    for (unsigned int i = 0; i < symsnr; i++ ) {
        
	if((symbol_count %2) == 0) {   //0 
	  viterbi_in[bits % 4] = insymbols_I[i];
          insymbols_interleaved_depunctured[bits] = insymbols_I[i];   
          bits++;
          viterbi_in[bits % 4] = insymbols_Q[i];
          insymbols_interleaved_depunctured[bits] = insymbols_Q[i];

    		if ((bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(viterbi_in, d_mettab, state0, state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (bits % 16 == 11) {
        	    viterbi_get_output(state0, p_decoded_data++);
                    decoded_data_count++;
      		  }
    	        }	
            bits++;
	}
	else{    //1
	  viterbi_in[bits % 4] = 128;
          insymbols_interleaved_depunctured[bits] = 128;  
          bits++;
          viterbi_in[bits % 4] = insymbols_Q[i];
          insymbols_interleaved_depunctured[bits] = insymbols_Q[i];

    		if ((bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(viterbi_in, d_mettab, state0, state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (bits % 16 == 11) {
        	    viterbi_get_output(state0, p_decoded_data++);
                    decoded_data_count++;
      		  }
    	        }	         
          bits++;

	  viterbi_in[bits % 4] = insymbols_I[i];
          insymbols_interleaved_depunctured[bits] = insymbols_I[i];  
          bits++;
          viterbi_in[bits % 4] = 128;
          insymbols_interleaved_depunctured[bits] = 128;
    		if ((bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(viterbi_in, d_mettab, state0, state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (bits % 16 == 11) {
        	    viterbi_get_output(state0, p_decoded_data++);
                    decoded_data_count++;
      		  }
    	        }	         
            bits++;	  

	}
        symbol_count++;
    
    }
  
   //for (unsigned int x=0; x< decoded_data_count; x++){
   //printf("%c",decoded_data[x]);
   //}

   //printf("Viterbi decoder : BER CALC: input bits=%i, \t depunctured_bits=%i, \t decoded_data_count=%i \n", symsnr*2, bits, decoded_data_count ); 

  //now we have decodet and we will reencode and compare difference between input symbols and reencoded data
  encode(&encoded_data[0], &decoded_data[1], decoded_data_count-1, 0);

  // compare
  difference_count = 0;
  bits = 0;
  for (unsigned int k=0; k <  decoded_data_count*16-16; k++){
         if (insymbols_interleaved_depunctured[k] != 128){
         difference_count += ( (insymbols_interleaved_depunctured[k] > 128 ) != (encoded_data[k])); 
         bits++;
         }          
  }
  
   //calculate BER
   
   ber =  float(difference_count)/float(bits);
   

return ber;

}



//*****************************************************************************
//    VITERBI DECODER, GENERAL WORK FUNCTION
//
//*****************************************************************************
int 
fec_decode_viterbi_ahrpt_metop_cb::general_work(int noutput_items,
				 gr_vector_int &ninput_items,
				 gr_vector_const_void_star &input_items,
				 gr_vector_void_star &output_items)

{
  	
  	gr_complex *in_syms = (gr_complex *)input_items[0];
  	unsigned char *out = (unsigned char *)output_items[0];
       
  	int ninputs = ninput_items[0];

        unsigned char input_symbols_buffer_I[ninputs+1];    //buffer for to char translated input symbols I
        unsigned char input_symbols_buffer_Q[ninputs+1];    //buffer for to char translated input symbols Q
  	unsigned int chan_len;
        
  	//translate all complex insymbols to char and save these to input_symbols_buffer's I and Q
  	float sample;
	for (unsigned int i = 0; i < ninputs; i++) {
    		// Translate and clip [-1.0..1.0] to [28..228]
    		sample = in_syms[i].real()*100.0+128.0; 
    		if (sample > 255.0)
			sample = 255.0;
    		else if (sample < 0.0)
			sample = 0.0;
    		input_symbols_buffer_I[i] = (unsigned char)(floor(sample));

                sample = in_syms[i].imag()*100.0+128.0; 
    		if (sample > 255.0)
			sample = 255.0;
    		else if (sample < 0.0)
			sample = 0.0;
    		input_symbols_buffer_Q[i] = (unsigned char)(floor(sample));
         	}  

     

/*//testing to delete

        if (d_even_symbol == true) {//pokud je sudy symbol jako prvni 
          d_ber[0] = ber_calc(d_00_st0, d_00_st1, 2048, input_symbols_buffer_I, input_symbols_buffer_Q);
          printf("Viterbi decoder :  sudy:  d_ber[0] %4f  \n", d_ber[0] );
	}
        else{   //jinak pokud je prvni lichy symbol, to vse kvuli spravnemu depuncturingu ve funkci BER 	
          d_ber[0] = ber_calc(d_180_st0, d_180_st1, 2048, input_symbols_buffer_I+1, input_symbols_buffer_Q+1);
          printf("Viterbi decoder : lichy:  d_ber[0] %4f  \n", d_ber[0] );
        }
       
//end of testing to delete

*/

  d_chan_len =2048;


 /*
  if(ninputs*2 < TestBitsLen ){
    d_chan_len = ninputs*2; 
    printf("Viterbi decoder : count of input items ninputs %i was lower than TestBitsLen %i  \n", ninputs, TestBitsLen);
   }
   else{
    d_chan_len = TestBitsLen;
   } 
*/

   // printf("Viterbi decoder : input syms %i \n", ninputs);    


    switch (d_state) { 
      case ST_IDLE:
  
        //process short paket of data and calculate BER TestBitsLen
        d_ber[0] = ber_calc(d_00_st0, d_00_st1, TestBitsLen, input_symbols_buffer_I, input_symbols_buffer_Q);		
        d_ber[1] = ber_calc(d_180_st0, d_180_st1, TestBitsLen, input_symbols_buffer_I+1, input_symbols_buffer_Q+1);	

        //check BER if we are synced
        if ((d_ber[0] < d_ber_threshold) && (d_ber[0] < d_ber[1])){   
          //probably no shift necessary
          d_shift = 0;
          printf("Viterbi decoder : ST_IDLE:  d_ber[0] %4f < d_ber[1] %4f NOSHIFT_DATA \n", d_ber[0], d_ber[1] );
          printf(" \n"); 
          printf("Viterbi decoder : ST_IDLE >> ST_SYNCING  \n");
          enter_syncing();
	}
        else if ((d_ber[1] < d_ber_threshold) && (d_ber[1] < d_ber[0])){
          //probably we must shift input data
	  d_shift = 1;
          printf("Viterbi decoder : ST_IDLE: d_ber[1] %4f < d_ber[0] %4f SHIFT_DATA\n", d_ber[1], d_ber[0] );
          printf("Viterbi decoder : \n");
          printf("Viterbi decoder : ST_IDLE >> ST_SYNCING \n");
          enter_syncing();
	}       
        else {
        //both BER exceed threshold
        printf("Viterbi decoder : ST_IDLE: Both, shifted and also unshifterd BER's exceed threshold, waiting for next  paket of symbols\n"); 
         d_valid_packet_count++;
         if (d_valid_packet_count == d_reset_after){
         do_reset();
         
         printf("Viterbi decoder : ST_IDLE: during a long time was no valid BER's, do DECODER RESET\n"); 
         }
	}


       break; 

       case ST_SYNCING:
        //process short or complete paket of data and calculate BER TestBitsLen
        if (d_even_symbol) {
          d_ber[0] = ber_calc(d_180_st0, d_180_st1, TestBitsLen, input_symbols_buffer_I, input_symbols_buffer_Q);
        }
        else {
          d_ber[0] = ber_calc(d_180_st0, d_180_st1, TestBitsLen, input_symbols_buffer_I+1, input_symbols_buffer_Q+1);
	}

        if (d_ber[0] < d_ber_threshold){
          d_valid_packet_count++;
          printf("Viterbi decoder : ST_SYNCING: PACKET %i BER = %4f < d_ber_threshold %4f \n",d_valid_packet_count, d_ber[0], d_ber_threshold);
	  if (d_valid_packet_count == d_insync_after){
            printf("Viterbi decoder : ST_SYNCING >> ST_SYNCED, BER of %i PACKET was lower than d_ber_threshold (%4f) \n",d_valid_packet_count, d_ber_threshold);
	    enter_synced();
	  }
	}
        else{
          enter_idle();
          printf("Viterbi decoder : ST_SYNCING >> ST_IDLE because BER of %i PACKET's was bigger like d_ber_threshold (%4f) \n",d_valid_packet_count, d_ber_threshold);
	}
       break;

       case ST_SYNCED:
        //process short paket of data and calculate BER TestBitsLen
       
        if (d_sync_check){
          viterbi_chunks_init(d_00_st0);
          if (d_even_symbol) {
            d_ber[0] = ber_calc(d_180_st0, d_180_st1, TestBitsLen, input_symbols_buffer_I, input_symbols_buffer_Q);
          }
          else {
            d_ber[0] = ber_calc(d_180_st0, d_180_st1, TestBitsLen, input_symbols_buffer_I+1, input_symbols_buffer_Q+1);
	  }
          //printf("Viterbi decoder : ST_SYNCED:  actually BER = %4f , ninputs = %i \n", d_ber[0], ninputs);
          
          if (d_ber[0] > d_ber_threshold){
            printf("Viterbi decoder : ST_SYNCED:  BER = %4f > d_ber_threshold %4f, ninputs = %i \n", d_ber[0], d_ber_threshold, ninputs);
            
            d_invalid_packet_count++;
	    if (d_invalid_packet_count == d_outsync_after){
              printf("Viterbi decoder : ST_SYNCED: enter_idle because %i packets was out of threshold \n \n", d_invalid_packet_count);
	      enter_idle();
	    }
	  }
        }
       break;

      default:
	throw std::runtime_error("Viterbi decoder: bad state\n");
      }

        

    

    //main continuous viterbi decoder loop, with depuncturing

    unsigned int out_byte_count = 0;
    for (unsigned int i = d_shift; i < ninputs; i++ ) {
        
	if((d_sym_count %2) == 0) {   //0
          d_even_symbol = true;
	  d_viterbi_in[d_bits % 4] = input_symbols_buffer_I[i];  d_bits++;
          d_viterbi_in[d_bits % 4] = input_symbols_buffer_Q[i];
    		if ((d_bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(d_viterbi_in, d_mettab, d_state0, d_state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (d_bits % 16 == 11) {
        	    viterbi_get_output(d_state0, out++);
        	    out_byte_count++;
      		  }
    	        }	
          d_bits++;
	}
	else{    //1
          
	  d_viterbi_in[d_bits % 4] = 128;  d_bits++;
          d_viterbi_in[d_bits % 4] = input_symbols_buffer_Q[i];
    		if ((d_bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(d_viterbi_in, d_mettab, d_state0, d_state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (d_bits % 16 == 11) {
        	    viterbi_get_output(d_state0, out++);
        	    out_byte_count++;
      		  }
    	        }	         
          d_bits++;

	  d_viterbi_in[d_bits % 4] = input_symbols_buffer_I[i];  d_bits++;
          d_viterbi_in[d_bits % 4] = 128;
    		if ((d_bits % 4) == 3) {
      		  // Every fourth symbol, perform butterfly operation
      		  viterbi_butterfly2(d_viterbi_in, d_mettab, d_state0, d_state1);
      		  // Every sixteenth symbol, read out a byte
      		  if (d_bits % 16 == 11) {
        	    viterbi_get_output(d_state0, out++);
        	    out_byte_count++;
      		  }
    	        }	         
          d_bits++;	  
	}
        d_sym_count++;
    }
    d_shift = 0;   //no shift next time
 
    if (d_sym_count %2 == 0 ){
      d_even_symbol = true;       //first bit in next processed input syms paket will be even.
    
    }
    else {
      d_even_symbol = false;
    }
   //printf("ninputs=%i \t d_sym_count=%i \n", ninputs, d_sym_count);

  consume_each (ninputs);
  return (out_byte_count); 

    


#ifdef general_work_debug
  printf("Viterbi decoder,        ninputs %i \n", ninputs);
  int pb = ninputs/16;
  printf("Viterbi decoder,     less symbs %i \n", (ninputs-pb*16));
  printf("Viterbi decoder,  noutput_items %i \n", noutput_items);
  //printf("Viterbi decoder,        d_count %i \n", d_count);
#endif  


}




