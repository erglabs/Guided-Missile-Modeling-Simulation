

           _mm512_stream_si512(&PMC0[i+0],vzi64);                                        
	   _mm512_stream_si512(&PMC1[i+0],vzi64);                                          
	   _mm512_stream_si512(&PMC2[i+0],vzi64);                                         
	   _mm512_stream_si512(&PMC3[i+0],vzi64);                                         
	   _mm512_stream_si512(&FCRef[i+0],vzi64);                                        
           _mm512_stream_si512(&FCAct[i+0],vzi64);                                         
           _mm512_stream_si512(&FCIns[i+0],vzi64);                                       
           _mm512_stream_si512(&TSC[i+0],vzi64);                                         
           _mm512_stream_pd(&core_util[i+0],vzf64);                                       
           _mm512_stream_pd(&core_avg[i+0],vzf64);                                         
	   _mm512_stream_si512(&PMC0[i+8],vzi64);                                         
	   _mm512_stream_si512(&PMC1[i+8],vzi64);                                        
	   _mm512_stream_si512(&PMC2[i+8],vzi64);                                         
	   _mm512_stream_si512(&PMC3[i+8],vzi64);                                         
	   _mm512_stream_si512(&FCRef[i+8],vzi64);                                       
           _mm512_stream_si512(&FCAct[i+8],vzi64);                                        
           _mm512_stream_si512(&FCIns[i+8],vzi64);                                        
           _mm512_stream_si512(&TSC[i+8],vzi64);                                          
           _mm512_stream_pd(&core_util[i+8],vzf64);                                       
           _mm512_stream_pd(&core_avg[i+8],vzf64);                                        
	   _mm512_stream_si512(&PMC0[i+16],vzi64);                                        
	   _mm512_stream_si512(&PMC1[i+16],vzi64);                                         
	   _mm512_stream_si512(&PMC2[i+16],vzi64);                                        
	   _mm512_stream_si512(&PMC3[i+16],vzi64);                                        
	   _mm512_stream_si512(&FCRef[i+16],vzi64);                                      
           _mm512_stream_si512(&FCAct[i+16],vzi64);                                       
           _mm512_stream_si512(&FCIns[i+16],vzi64);                                       
           _mm512_stream_si512(&TSC[i+16],vzi64);                                          
           _mm512_stream_pd(&core_util[i+16],vzf64);                                      
           _mm512_stream_pd(&core_avg[i+16],vzf64);                                       
	   _mm512_stream_si512(&PMC0[i+24],vzi64);                                       
	   _mm512_stream_si512(&PMC1[i+24],vzi64);                                        
	   _mm512_stream_si512(&PMC2[i+24],vzi64);                                       
	   _mm512_stream_si512(&PMC3[i+24],vzi64);                                        
	   _mm512_stream_si512(&FCRef[i+24],vzi64);                                       
           _mm512_stream_si512(&FCAct[i+24],vzi64);                                       
           _mm512_stream_si512(&FCIns[i+24],vzi64);                                       
           _mm512_stream_si512(&TSC[i+24],vzi64);                                         
           _mm512_stream_pd(&core_util[i+24],vzf64);                                      
           _mm512_stream_pd(&core_avg[i+24],vzf64);
