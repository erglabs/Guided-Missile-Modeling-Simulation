
#ifndef __GMS_DESCRIPTIVE_STATISTICS_H__
#define __GMS_DESCRIPTIVE_STATISTICS_H__


namespace file_info {

     const unsigned int gGMS_DESCRIPTIVE_STATISTICS_MAJOR = 1U;
     const unsigned int gGMS_DESCRIPTIVE_STATISTICS_MINOR = 0U;
     const unsigned int gGMS_DESCRIPTIVE_STATISTICS_MICRO = 0U;
     const unsigned int gGMS_DESCRIPTIVE_STATISTICS_FULLVER =
           1000U*gGMS_DESCRIPTIVE_STATISTICS_MAJOR+
	   100U*gGMS_DESCRIPTIVE_STATISTICS_MINOR+
	   10U*gGMS_DESCRIPTIVE_STATISTICS_MICRO;
     const char * const pgGMS_DESCRIPTIVE_STATISTICS_CREATE_DATE = "08-11-2020 09:22AM +00200 (SUN 08 NOV 2020 GMT+2)";
     const char * const pgGMS_DESCRIPTIVE_STATISTICS_BUILD_DATE  = __DATE__ ":" __TIME__;
     const char * const pgGMS_DESCRIPTIVE_STATISTICS_AUTHOR      = "CHARLES P. REEVE NATIONAL BUREAU OF STANDARDS, translated to C++ by Bernard Gingold beniekg@gmail.com";
}

#include <math.h>
#include <cstdint>
#include "GMS_config.h"
#include "GMS_cephes.h" // to eliminate cmath.h implemented by GLIBC
                        // used only in scalar code.
                        // Vector code is handled by SVML calls

namespace gms {

      namespace math {

      /*
           C
C-----------------------------------------------------------------------
C   BARTLT   WRITTEN BY CHARLES P. REEVE, STATISTICAL ENGINEERING
C            DIVISION, NATIONAL BUREAU OF STANDARDS, GAITHERSBURG,
C            MARYLAND  20899
C
C   FOR: PERFORMING BARTLETT'S TEST FOR HOMOGENEITY OF VARIANCES ON
C        THREE OR MORE VARIANCES (THE F TEST SHOULD BE USED IN THE CASE
C        OF TWO VARIANCES).  IF THE INPUT PARAMETERS ARE NOT VALID AN 
C        ERROR FLAG IS SET AND NOTHING FURTHER IS COMPUTED, OTHERWISE 
C        THE FOLLOWING ARE COMPUTED: 
C
C           1) THE CHI-SQUARED STATISTIC (CH2),
C           2) THE CUMULATIVE DISTRIBUTION FUNCTION OF THE CHI-SQUARED
C              DISTRIBUTION EVALUATED AT CH2 (CH2CDF), AND
C           3) THE POOLED VARIANCE (VARP) AND ITS CORRESPONDING
C              DEGREES OF FREEDOM (DFP) 
C
C        THE VALUES IN 3) MAY BE USEFUL ONLY IF THE VARIANCES ARE
C        DETERMINED TO BE EQUAL.  THE VALUE OF CH2CDF IS GOOD TO SIX
C        DECIMAL PLACES.
C
C   SUBPROGRAMS CALLED: CDFGAM (GAMMA CUMULATIVE DISTRIBUTION FUNCTION)
C
C   CURRENT VERSION COMPLETED FEBRUARY 3, 1987
C
C   REFERENCES: 
C
C   1) SNEDECOR, GEORGE W. AND COCHRAN, WILLIAM G., 'STATISTICAL
C      METHODS', 6TH EDITION, IOWA STATE UNIVERSITY PRESS, PP. 296-298.
C
C   2) BROWNLEE, K.A., 'STATISTICAL THEORY AND METHODOLOGY IN SCIENCE 
C      AND ENGINEERING', JOHN WILEY & SONS, 1960, PP. 225-227.
C-----------------------------------------------------------------------
C   DEFINITION OF PASSED PARAMETERS: 
C
C    * VAR = VECTOR (LENGTH N) OF VARIANCES (REAL)
C
C     * DF = VECTOR (LENGTH N) OF DEGREES OF FREEDOM CORRESPONDING
C            TO THE VARIANCES (REAL)
C
C      * N = NUMBER OF VARIANCES [>2] (INTEGER)
C
C      CH2 = THE CHI-SQUARED STATISTIC ASSOCIATED WITH BARTLETT'S TEST
C            (REAL) 
C
C   CH2CDF = THE CUMULATIVE DISTRIBUTION FUNCTION OF THE CHI-SQUARED
C            DISTRIBUTION WITH N-1 DEGREES OF FREEDOM EVALUATED AT CH2
C            (REAL) 
C
C     VARP = THE POOLED VARIANCE DETERMINED FROM THE N VARIANCES (REAL)
C
C      DFP = THE DEGREES OF FREEDOM ASSOCIATED WITH THE POOLED
C            VARIANCE (REAL)
C
C    IFLAG = THE ERROR FLAG ON OUTPUT (INTEGER)   INTERPRETATION: 
C              0 -> NO ERRORS DETECTED
C            1,2 -> ERROR FLAGS FROM SUBROUTINE CDFGAM
C              3 -> N<3
C              4 -> AT LEAST ONE DF(I) IS <= 0.0
C              5 -> AT LEAST ONE VARIANCE(I) IS < 0.0
C
C   * INDICATES PARAMETERS REQUIRING INPUT VALUES 
C-----------------------------------------------------------------------
C
        */
              __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      void bartlt(float * __restrict __ATTR_ALIGN__(64) var,
	                  float * __restrict __ATTR_ALIGN__(64) df,
			  const int32_t n,
			  float &ch2,
			  float &ch2cdf,
			  float &varp,
			  float &dfp,
			  int32_t &iflag) {
                if(n < 3) {
                   iflag = 3;
		   return;
		}
		for(int32_t i = 0; i != n; ++i) {
                    if(df[i] <= 0.0f) {
                       iflag = 4;
		       return;
		    }
		    if(var[i] < 0.0f) {
                       iflag = 5;
		       return;
		    }
		}

	       float a,c,x,alpha,eps;
	       a    = 0.0f;
	       varp = 0.0f;
	       c    = 0.0f;
	       ch2  = 0.0f;
#if defined __ICC || defined __INTEL_COMPILER
               __assume_aligned(var,64);
	       __assume_aligned(df,64);
#elif defined __GNUC__ && !defined __INTEL_COMPILER
               var = (float*)__builtin_assume_aligned(var,64);
	       df  = (float*)__builtin_assume_aligned(df,64);
#endif
#if defined __ICC || defined __INTEL_COMPILER
#pragma simd reduction(+:a,varp,c,ch2), vecremainder
#elif defined __GNUC__ || !defined __INTEL_COMPILER
#pragma omp simd reduction(+: a,varp,c,ch2)
#endif
               for(int32_t i = 0; i != n; ++i) {
                   a = a + df[i];
		   varp = varp + df[i] * varp[i];
		   c = c + 1.0f/df[i];
		   ch2 = ch2 + df[i]*logf(var[i]);
	       }
	       varp = varp/a;
	       dfp  = a;
	       ch2  = a*ceph_logf(varp)-ch2;
	       a    = 1.0f+(c-1.0f/a)/(3.0f*(float)n-1);
	       x    = 0.5f*ch2;
	       alpha = 0.5f*(float)n-1;
	       eps   = 0.0000001f;
	       // call cdfgam here
        }

      /*
           C
C-----------------------------------------------------------------------
C   CDFBET   WRITTEN BY CHARLES P. REEVE, STATISTICAL ENGINEERING
C            DIVISION, NATIONAL BUREAU OF STANDARDS, GAITHERSBURG,
C            MARYLAND  20899
C
C   FOR: COMPUTING THE CUMULATIVE DISTRIBUTION FUNCTION OF THE BETA
C        DISTRIBUTION (ALSO KNOWN AS THE INCOMPLETE BETA RATIO) TO A
C        SPECIFIED ACCURACY (TRUNCATION ERROR IN THE INFINITE SERIES).
C        THE ALGORITHM, DESCRIBED IN REFERENCE 2, IS A MODIFICATION OF
C        THE ALGORITHM OF REFERENCE 1.  THREE FEATURES HAVE BEEN ADDED: 
C
C        1) A PRECISE METHOD OF MEETING THE TRUNCATION ACCURACY,
C        2) A CONSTANT W USED IN DETERMINING FOR WHICH X VALUES THE
C           RELATION I(X,P,Q) = 1 - I(1-X,Q,P) IS TO BE USED, AND
C        3) A CONSTANT UFLO >= THE UNDERFLOW LIMIT ON THE COMPUTER.
C
C   SUBPROGRAMS CALLED: DGAMLN (LOG OF GAMMA FUNCTION)
C
C   CURRENT VERSION COMPLETED OCTOBER 24, 1986
C
C   REFERENCES: 
C
C   1) MAJUMDER, K.L. AND BHATTACHARJEE, G.P., 'THE INCOMPLETE BETA
C      INTEGRAL', ALGORITHM AS 63, APPLIED STATISTICS, VOL. 22, NO. 3,
C      1973, PP. 409-411.
C
C   2) REEVE, CHARLES P., 'AN ALGORITHM FOR COMPUTING THE BETA C.D.F. 
C      TO A SPECIFIED ACCURACY', STATISTICAL ENGINEERING DIVISION
C      NOTE 86-3, OCTOBER 1986.
C-----------------------------------------------------------------------
C   DEFINITION OF PASSED PARAMETERS: 
C
C      * X = VALUE AT WHICH THE C.D.F. IS TO BE COMPUTED (REAL)
C
C      * P = FIRST PARAMETER OF THE BETA FUNCTION (>0) (REAL)
C
C      * Q = SECOND PARAMETER OF THE BETA FUNCTION (>0) (REAL)
C
C    * EPS =  THE DESIRED ABSOLUTE ACCURACY OF THE C.D.F. (>0) (REAL) 
C
C    IFLAG = ERROR INDICATOR ON OUTPUT (INTEGER)   INTERPRETATION: 
C            0 -> NO ERRORS DETECTED
C            1 -> EITHER P OR Q OR EPS IS <= UFLO 
C            2 -> NUMBER OF TERMS EVALUATED IN THE INFINITE SERIES
C                 EXCEEDS JMAX
C
C     CDFX = THE C.D.F. EVALUATED AT X (REAL)
C
C   * INDICATES PARAMETERS REQUIRING INPUT VALUES 
C-----------------------------------------------------------------------
C
       */

              __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      void cdfbet(float x,
	                  float p,
			  float q,
			  float eps,
			  int32_t &iflag,
			  float &cdfx) {
               constexpr float w = 20.0f;
	       constexpr float uflo = 1.0e-30f;
	       constexpr int32_t jmax = 5000;
	       float xy,yx,pq,qp;
	       float dp,dq,pdfl,u,r,v,yxeps;
	       bool bc1,bc2,bc3,ll;
	       bc1 =   p <= uflo;
	       bc2 =   q <= uflo;
	       bc3 = eps <= uflo;
	       // CHECK FOR VALIDITY OF ARGUMENTS P, Q, AND EPS
	       if(bc1 || bc2 || bc3) {
                  iflag = 1;
		  return;
	       }
	       iflag = 0;
	       if(x <= 0.0f) {
	          return;
	       }
	       ll = false;
	       if(x >= 1.0f) {
                  cdfx = 1.0f;
	       }
	       else {
                // SWITCH ARGUMENTS IF NECESSARY
                  ll = p+w >= (p+q+2.0f*w)*x;
		  if(ll) {
                     xy = x;
		     yx = 1.0f-xy;
		     pq = p;
		     qp = q;
		  }
		  else {
                     yx = x;
		     xy = 1.0f-yx;
		     qp = p;
		     pq = q;
		  }
		  // EVALUATE THE BETA P.D.F. AND CHECK FOR UNDERFLOW
		  dp   = (double)(pq-1.0f)*ceph_log((double)xy)-dgamln(pq);
		  dq   = (double)(qp-1.0f)*ceph_log((double)yx)-dgamln(qp);
		  pdfl = (float)(dgamln(pq+qp)+dp+dq);
		  if(pdfl < log(uflo)) {
                     ; // ?
		  }
		  else {
                     u = ceph_expf(pdfl)*xy/pq;
		     r = xy/yx;
label_10:            if(qp <= 1.0f) goto label_20;
                     // INCREMENT PQ AND DECREMENT QP
		     if(u <= eps*(1.0f-(pq+qp)*xy/(pq+1.0f))) goto label_40;
		     cdfx = cdfx+u;
		     pq   = pq+1.0f;
		     qp   = qp-1.0f;
		     u    = qp*r*u/pq;
		     goto label_10;
label_20:            v    = yx*u;
                     yxeps = yx*eps;
		     //  INCREMENT PQ

		     for(int32_t j = 0; i != jmax; ++j) {
                         if(v <= yxeps) goto label_40;
			 cdfx = cdfx+v;
			 pq   = pq+1.0f;
			 v    = (pq+qp-1.0f)*xy*v/pq;
		     }
		     iflag = 2;
		  }
label_40:         if(!ll) cdfx = 1.0f-cdfx;
	       }
      }

      /*
          C-----------------------------------------------------------------------
C   CDFDNF   WRITTEN BY CHARLES P. REEVE, STATISTICAL ENGINEERING
C            DIVISION, NATIONAL BUREAU OF STANDARDS, GAITHERSBURG,
C            MARYLAND  20899
C
C   FOR: COMPUTING THE CUMULATIVE DISTRIBUTION FUNCTION OF THE DOUBLY 
C        NONCENTRAL F DISTRIBUTION TO A SPECIFIED ACCURACY (TRUNCATION
C        ERROR IN THE INFINITE SERIES REPRESENTATION GIVEN BY EQUATION
C        2.2 IN REFERENCE 1 BELOW).  THE BETA C.D.F. ROUTINE IS CALLED
C        AT MOST TWO TIMES.  FURTHER VALUES OF THE BETA C.D.F. ARE
C        OBTAINED FROM RECURRENCE RELATIONS GIVEN IN REFERENCE 2.
C        REFERENCE 3 GIVES A DETAILED DESCRIPTION OF THE ALGORITHM
C        HEREIN.
C
C        THIS PROGRAM MAY ALSO BE EFFICIENTLY USED TO COMPUTE THE
C        CUMULATIVE DISTRIBUTION FUNCTIONS OF THE SINGLY NONCENTRAL
C        AND CENTRAL F DISTRIBUTIONS BY SETTING THE APPROPRIATE
C        NONCENTRALITY PARAMETERS EQUAL TO ZERO.
C
C        CHECKS ARE MADE TO ASSURE THAT ALL PASSED PARAMETERS ARE
C        WITHIN VALID RANGES AS GIVEN BELOW.  NO UPPER LIMIT IS SET
C        FOR THE NONCENTRALITY PARAMETERS, BUT VALUES UP TO ABOUT
C        10,000 CAN BE HANDLED WITH THE CURRENT DIMENSION LIMITS.  THE
C        COMPUTED VALUE CDFX IS VALID ONLY IF IFLAG=0 ON RETURN.
C
C   NOTE: IN EQUATION 2.2 OF REFERENCE 1 THE AUTHOR HAS MISTAKENLY
C         REVERSED THE ARGUMENTS OF THE INCOMPLETE BETA FUNCTION.
C         THEY SHOULD READ [(M/2)+R,(N/2+S)] WHERE M AND N ARE THE
C         DEGREES OF FREEDOM ASSOCIATED WITH THE NUMERATOR AND
C         DENOMINATOR RESPECTIVELY OF THE F STATISTIC.  TO FURTHER
C         CONFUSE THE ISSUE, THE AUTHOR HAS REVERSED THE USAGE OF
C         M AND N IN SECTION 1 OF THE PAPER.
C
C   NOTE: IN SUBROUTINE EDGEF THE DOUBLE PRECISION CONSTANT DEUFLO IS 
C         THE EXPONENTIAL UNDERFLOW LIMIT WHOSE CURRENT VALUE IS SET
C         AT -69D0.  ON A COMPUTER WHERE DEXP(-69D0) CAUSES UNDERFLOW 
C         THIS LIMIT SHOULD BE CHANGED. 
C
C   SUBPROGRAMS CALLED: CDFBET (BETA C.D.F.)
C                       DGAMLN (DOUBLE PRECISION LOG OF GAMMA FUNCTION)
C                       POISSF, EDGEF (ATTACHED)
C
C   CURRENT VERSION COMPLETED SEPTEMBER 29, 1988
C
C   REFERENCES: 
C
C   1. BULGREN, W.G., 'ON REPRESENTATIONS OF THE DOUBLY NONCENTRAL F
C      DISTRIBUTION', JOURNAL OF THE AMERICAN STATISTICAL ASSOCIATION,
C      MARCH 1971, VOLUME 66, NO. 333, PP. 184-186.
C
C   2. ABRAMOWITZ, MILTON, AND STEGUN, IRENE A., 'HANDBOOK OF
C      MATHEMATICAL FUNCTIONS', NATIONAL BUREAU OF STANDARDS APPLIED
C      MATHEMATICS SERIES 55, NOVEMBER 1970, P. 944.
C
C   3. REEVE, CHARLES P., 'AN ALGORITHM FOR COMPUTING THE DOUBLY
C      NONCENTRAL F C.D.F. TO A SPECIFIED ACCURACY', STATISTICAL
C      ENGINEERING DIVISION NOTE 86-4, NOVEMBER 1986.
C-----------------------------------------------------------------------
C   DEFINITION OF PASSED PARAMETERS: 
C
C        * X = VALUE (>=0) AT WHICH THE C.D.F. IS TO BE COMPUTED (REAL)
C
C      * DF1 = DEGREES OF FREEDOM (>0) IN THE NUMERATOR (REAL)
C
C      * DF2 = DEGREES OF FREEDOM (>0) IN THE DENOMINATOR (REAL)
C
C   * ALAMB1 = THE NONCENTRALITY PARAMETER (>=0) FOR THE NUMERATOR
C              (REAL) [EQUAL TO ZERO FOR THE CENTRAL F DISTRIBUTION]
C
C   * ALAMB2 = THE NONCENTRALITY PARAMETER (>=0) FOR THE DENOMINATOR
C              (REAL) [EQUAL TO ZERO FOR THE SINGLY NONCENTRAL F AND
C              CENTRAL F DISTRIBUTIONS] 
C
C      * EPS = THE DESIRED ABSOLUTE ACCURACY OF THE C.D.F. (REAL)
C              [1 >= EPS >= 10**(-10)]
C
C      IFLAG = ERROR INDICATOR ON OUTPUT (INTEGER)   INTERPRETATION:  
C                0 -> NO ERRORS DETECTED
C              1,2 -> ERROR FLAGS FROM SUBROUTINE CDFBET
C                3 -> EITHER ALAMB1 OR ALAMB2 IS < 0
C                4 -> EITHER DF1 OR DF2 IS <= 0
C                5 -> EPS IS OUTSIDE THE RANGE [10**(-10),1]
C                6 -> VECTOR DIMENSIONS ARE TOO SMALL - INCREASE NX
C
C       CDFX = THE DOUBLY NONCENTRAL F C.D.F. EVALUATED AT X (REAL)
C
C   * INDICATES PARAMETERS REQUIRING INPUT VALUES 
C-----------------------------------------------------------------------
C
        */

#include "GMS_simd_memops.h"


              __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      void cdfdnf(float x,
	                  float df1,
			  float df2,
			  float alamb1,
			  float alamb2,
			  float eps,
			  int32_t &iflag,
			  float &cdfx) {
               using namespace gms::common;
	       // CHECK VALIDITY OF ARGUMENTS
	       if(alamb1<0.0f || alamb2<0.0f) {
	          iflag = 3;
		  return;
	       }
	       if(df1<=0.0f || df2<=0.0f) {
                  iflag = 4;
		  return;
	       }
	       if(eps>1.0f || eps<1.0e-10f) {
                  iflag = 5;
		  return;
	       }
	       iflag = 0;
               constexpr int32_t nx = 1008;
#if defined __AVX512F__
	       __attribute__((aligned(64))) float bfi[nx];
	       __attribute__((aligned(64))) float bfj[nx];
	       __attribute__((aligned(64))) float poi[nx];
	       __attribute__((aligned(64))) float poj[nx];
#else
               __attribute__((aligned(32))) float bfi[nx];
	       __attribute__((aligned(32))) float bfj[nx];
	       __attribute__((aligned(32))) float poi[nx];
	       __attribute__((aligned(32))) float poj[nx];
#endif
	       float eps3,fa,ga,fb,gb,fc,xx,yy;
	       int32_t imin,ni,jmin,nj;
	       // SET ERROR CRITERION FOR THE BETA C.D.F. (PECULIAR TO CDFBET)
	       eps3 = 0.001f*eps;
	       fa   = 0.5f*alamb1;
	       ga   = 0.5f*alamb2;
	       fb   = 0.5f*df1;
	       gb   = 0.5f*df2;
	       yy   = df2/(df2+df1*x);
	       if(yy>=1.0f){
                  return;
	       }
	       xx = 1.0f-yy;
	       if(xx>=1.0f) {
                  cdfx = 1.0f;
		  return;
	       }
	       // COMPUTE POISSON PROBABILITIES IN VECTORS POI AND POJ
#if defined __AVX512F__
               avx512_init_unroll4x_ps(&poi[0],nx,0.0f);
#else
               avx256_init_unroll4x_ps(&poi[0],nx,0.0f);
#endif
               poissf(fa,eps,imin,ni,poi,nx,iflag);
	       if(iflag != 0) {
                  return;
	       }
	       fc = fb+(float)imin;
#if defined __AVX512F__
               avx512_init_unroll4x_ps(&poj[0],nx,0.0f);
#else
               avx256_init_unroll4x_ps(&poj[0],nx,0.0f);
#endif
	       poissf(ga,eps,jmin,nj,poj,nx,iflag);
	       if(iflag != 0) {
                  return;
	       }
	       gc = gb+(float)jmin;
	       // COMPUTE BETA C.D.F. BY RECURRENCE WHEN I=IMIN AND J=JMIN TO JMAX
#if defined __AVX512F__
               avx512_init_unroll4x_ps(&bfj[0],nx,0.0f);
#else
               avx256_init_unroll4x_ps(&bfj[0],nx,0.0f);
#endif
	       edgef(nj,gc,fc,yy,xx,bfj,cdfx,poj,poi,eps3,iflag,1);
	       if(ni<=1 || iflag != 0) {
                  return;
	       }
	       //COMPUTE BETA C.D.F. BY RECURRENCE WHEN J=JMIN AND I=IMIN TO IMAX
#if defined __AVX512F__
               avx512_init_unroll4x_ps(&bfi[0],nx,0.0f);
#else
               avx256_init_unroll4x_ps(&bfi[0],nx,0.0f);
#endif
	       bfi[0] = bfj[0];
	       edgef(ni,fc,gc,xx,yy,bfi,cdfx,poi,poj,eps3,iflag,2);
	       if(nj<=1 || iflag != 0) {
                  return;
	       }
	       // COMPUTE BETA C.D.F. BY RECURRENCE WHEN I>IMIN AND J>JMIN
#if defined __ICC || defined __INTEL_COMPILER
#pragma simd reduction(+:cdfx)
#elif defined __GNUC__ && !defined __INTEL_COMPILER
#pragma omp simd reduction(+:cdfx)
#endif
	       for(int32_t i = 1; i != ni; ++i) {
                   bfj[0] = bfi[i];
		   float tmp = poi[i];
		   for(int32_t j = 1; j != nj; ++j) {
                       bfj[j] = xx*bfj[j]+yy*bfj[j-1];
		       cdfx   = cdfx+tmp+poj[j]*bfj[j];
		   }
	       }
	}



	      __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      void poissf(float alamb,
	                  const float eps,
			  int32_t l,
			  int32_t &nspan,
			  float * __restrict __ATTR_ALIGN__(32) v,
			  const int32_t nv,
			  int32_t &iflag) {
                  double dal,dk,dlimit,dsum;
		  float  pl,pk;
		  int32_t k,nk,nl,inc;
		  dlimit = 1.0-0.5*(double)eps;
		  k = (int32_t)alamb;
		  l = k+1;
		  if(alamb==0.0) {
                     pl = 1.0f;
		  }
		  else {
                     dal = (double)alamb;
		     dk  = (double)k;
		     pl  = (float)(ceph_exp(dk*log(dal)-dal-dgamln((float)k+1)));
		  }
	}

#include <limits>
#include <algorithm>

              __ATTR_PURE__
	      __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      double dgamln(const float x) {
	        if(x<=0.0) {
                   return std::numeric_limits<double>::signaling_NaN();
		}
		double c,dx,q,r,xmin,xn,result;
                constexpr double xmin  = 6.894;
		constexpr double absac = 1.0e-15;
		constexpr double C     = 0.918938533204672741780329736;
		constexpr double B1    = 0.833333333333333333333333333E-1;
		constexpr double B2    = -0.277777777777777777777777778E-2;
		constexpr double B3    = 0.793650793650793650793650794E-3;
		constexpr double B4    = -0.595238095238095238095238095E-3;
		constexpr double B5    = 0.841750841750841750841750842E-3;
		constexpr double B6    = -0.191752691752691752691752692E-2;
		constexpr double B7    = 0.641025641025641025641025641E-2;
		constexpr double B8    = -0.295506535947712418300653595E-1;
		int32_t n;
		dx = (double)x;
		n  = std::max(0,(int32_t)(xmin-dx+1.0));
		xn = dx+(double)n;
		r  = 1.0/xn;
		q  = r*r;
		result = 0.0;
		result = r*(b1+q*(b2+q*(b3+q*(b4+q*(b5+q*(b6+q*(b7+q*b8)))))))+c
		         +(xn-0.5)*log(xn)-xn;
		// USE RECURRENCE RELATION WHEN N>0 (X<XMIN)
		if(n>0) {
                   q = 1.0;
		   for(int32_t i = 0; i != n-1; ++i) {
                       q = q*(dx+(double)i)
		   }
		   result -= ceph_log(q);
		}
		return (result);
	}

	 /*
             PURPOSE--THIS SUBROUTINE COMPUTES THE
C              SAMPLE AUTOCORRELATION COEFFICIENT 
C              OF THE DATA IN THE INPUT VECTOR X. 
C              THE SAMPLE AUTOCORRELATION COEFFICIENT =  THE CORRELATION
C              BETWEEN X(I) AND X(I+1) OVER THE ENTIRE SAMPLE.
C              THE AUTOCORRELATION COEFFICIENT COEFFICIENT WILL BE A
C              SINGLE PRECISION VALUE BETWEEN -1.0 AND 1.0
C              (INCLUSIVELY). 
C     INPUT  ARGUMENTS--X      = THE SINGLE PRECISION VECTOR OF
C                                (UNSORTED) OBSERVATIONS.
C                     --N      = THE INTEGER NUMBER OF OBSERVATIONS
C                                IN THE VECTOR X. 
C                     --IWRITE = AN INTEGER FLAG CODE WHICH 
C                                (IF SET TO 0) WILL SUPPRESS
C                                THE PRINTING OF THE
C                                SAMPLE AUTOCORRELATION COEFFICIENT
C                                AS IT IS COMPUTED;
C                                OR (IF SET TO SOME INTEGER 
C                                VALUE NOT EQUAL TO 0),
C                                LIKE, SAY, 1) WILL CAUSE
C                                THE PRINTING OF THE
C                                SAMPLE AUTOCORRELATION COEFFICIENT
C                                AT THE TIME IT IS COMPUTED.
C     OUTPUT ARGUMENTS--XAUTOC = THE SINGLE PRECISION VALUE OF THE
C                                COMPUTED SAMPLE AUTOCORRELATION
C                                COEFFICIENT.
C                                THIS SINGLE PRECISION VALUE
C                                WILL BE BETWEEN -1.0 AND 1.0
C                                (INCLUSIVELY).
C     OUTPUT--THE COMPUTED SINGLE PRECISION VALUE OF THE
C             SAMPLE AUTOCORRELATION COEFFICIENT. 
           */
	      
              __ATTR_ALWAYS_INLINE__
	      __ATTR_HOT__
              __ATTR_ALIGN__(32)
	      static inline
	      float autoco(float * __restrict __ATTR_ALIGN__(64) x,
	                   const int32_t n) {
                    float xautoco = 0.0f;
		    register float xbar,xbar1,xbar2,sum1,sum2,sum3;
		    float an;
		    int32_t nm1;
		    register int32_t ip1;
		    an = (float)n;
		    xbar = 0.0f;
#if defined __INTEL_COMPILER
                    __assume_aligned(x,64);
#elif defined __GNUC__ && !defined __INTEL_COMPILER
                    x = (float*)__builtin_assume_aligned(x,64);
#endif
#if defined __ICC || defined __INTEL_COMPILER
#pragma loop_count min(2),avg(1000),max(5000)
#pragma simd reduction(+:xbar)
#elif defined __GNUC__ && !defined __INTEL_COMPILER
#pragma omp simd reduction(+:xbar)
#endif
                    for(int32_t i = 0; i != n; ++i) {
                        xbar = xbar+x[i];
		    }
		    xbar1 = xbar-x[n-1];
		    sum1 = 0.0f;
		    xbar1 = xbar1/(an-1.0f);
		    sum2 = 0.0f;
		    xbar2 = xbar-x[0];
		    sum3 = 0.0f;
		    xbar2 = xbar2/(an-1.0f);
		    nm1 = n-1;
#if defined __ICC || defined __INTEL_COMPILER
#pragma loop_count min(2),avg(1000),max(5000)
#pragma simd reduction(+:sum1,sum2,sum3)
#elif defined __GNUC__ && !defined __INTEL_COMPILER
#pragma omp simd reduction(+:sum1,sum2,sum3)
#endif
                    for(int32_t i = 0; i != nm1; ++i) {
		        ip1 = i+1;
                        register float txi  = x[i];
			register float tip1 = x[ip1];
			sum1 = sum1+(txi-xbar1)*(tip1-xbar2);
			sum2 = sum2+(txi-xbar1)*(txi-xbar);
			sum3 = sum3+(tip1-xbar2)*(tip1-xbar2);
		    }
		    xautoc = sum1/(ceph_sqrtf(sum2*sum3));
		    return (xautoc);
	      }
} //math




} //gms


#endif /*__GMS_DESCRIPTIVE_STATISTICS_H__*/
