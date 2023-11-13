   #if 0
   pcal.c 0.2.2 (Sat 19 Dec 1998)
   Adam M. Costello <amc @ cs.berkeley.edu>

   This is public domain example code for computing
   the mappings defined for the PNG pCAL chunk.

   #endif
   #if __STDC__ != 1
   #error This code relies on ANSI C conformance.
   #endif


   #include <limits.h>
   #include <math.h>
   #include <stdio.h>
   #include <stdlib.h>


   /* In this program a type named uintN denotes an unsigned    */
   /* type that handles at least all values 0 through (2^N)-1.  */
   /* A type named intN denotes a signed type that handles at   */
   /* least all values 1-2^(N-1) through 2^(N-1)-1.  It is not  */
   /* necessarily the smallest such type; we are more concerned */
   /* with speed.                                               */

   typedef unsigned int uint16;

   #if UINT_MAX >= 0xffffffff
     typedef unsigned int uint32;
   #else
     typedef unsigned long uint32;
   #endif

   #if INT_MAX >= 0x7fffffff && INT_MIN + 0x7fffffff <= 0
     typedef int int32;
   #else
     typedef long int32;
   #endif

   /* Testing for 48-bit integers is tricky because we cannot */
   /* safely use constants greater than 0xffffffff.  Also,    */
   /* shifting by the entire width of a type is undefined, so */
   /* for unsigned int, which might be only 16 bits wide, we  */
   /* must shift in two steps.                                */

   #if (UINT_MAX - 0xffff) >> 8 >> 8 >= 0xffffffff
     typedef unsigned int uint48;
     #define HAVE_UINT48 1
   #elif (ULONG_MAX - 0xffff) >> 16 >= 0xffffffff
     typedef unsigned long uint48;
     #define HAVE_UINT48 1
   #elif defined(ULLONG_MAX)
     #if (ULLONG_MAX - 0xffff) >> 16 >= 0xffffffff
       typedef unsigned long long uint48;
       #define HAVE_UINT48 1
     #endif
   #else
     #define HAVE_UINT48 0
   #endif


   /*******************/
   /* Program failure */

   void
   fail(const char *msg)
   {
     fputs(msg,stderr);
     fputc('\n', stderr);
     exit(EXIT_FAILURE);
   }


   /*************************/
   /* Check max, x0, and x1 */

   int
   samp_params_ok(uint16 max, int32 x0, int32 x1)

   /* Returns 1 if max, x0, and x1 have */
   /* allowed values, 0 otherwise.      */

   {
     const int32 xlimit = 0x7fffffff;

     return    max >  0      && max <=  0xffff
            &&  x0 <= xlimit &&  x0 >= -xlimit
            &&  x1 <= xlimit &&  x1 >= -xlimit
            &&  x0 != x1;
   }


   /***********************************************/
   /* Map from stored samples to original samples */

   int32
   stored_to_orig(uint16 stored, uint16 max, int32 x0, int32 x1)

   #if 0

     Returns the original sample corresponding to the given stored
     sample, which must be <= max.  The parameters max, x0, and x1
     must have been approved by samp_params_ok().

     The pCAL spec says:

         orig = (stored * (x1-x0) + max/2) / max + x0           [1]

     Equivalently:

         orig = (stored * (x1-x0) + max/2) / max
                + (x0-x1) - (x0-x1) + x0
         orig = (stored * (x1-x0) + max * (x0-x1) + max/2) / max
                - (x0-x1) + x0
         orig = ((max - stored) * (x0-x1) + max/2) / max + x1

     So we can check whether x0 < x1 and coerce the formula so that
     the numerators and denominators are always nonnegative:

         orig = (offset * xspan + max/2) / max + xbottom        [2]

     This will come in handy later.

     But the multiplication and the subtraction can overflow, so we
     have to be trickier.  For the subtraction, we can convert to
     unsigned integers.  For the multiplication, we can use 48-bit
     integers if we have them, otherwise observe that:

            b    =   (b/c)*c +     b%c
          a*b    = a*(b/c)*c +  a*(b%c)  ; let d = a*(b%c)
         (a*b)/c = a*(b/c)   +     d/c   remainder d%c          [3]

     These are true no matter which way the division rounds.  If
     (a*b)/c is in-range, a*(b/c) is guaranteed to be in-range if
     b/c rounds toward zero.  Here is another observation:

         sum{x_i} / c = sum{x_i / c} + sum{x_i % c} / c         [4]

     This one also avoids overflow if the division rounds toward
     zero.  The pCAL spec requires rounding toward -infinity.  ANSI
     C leaves the rounding direction implementation-defined except
     when both the numerator and denominator are nonnegative, in
     which case it rounds downward.  So if we arrange for all
     numerators and denominators to be nonnegative, everything
     works.  Starting with equation 2 and applying identity 4, then
     3, we obtain the final formula:

               d = offset * (xspan % max)
         xoffset = offset * (xspan / max) + d/max
                   + (d%max + max/2) / max
            orig = xoffset + xbottom

   #endif

   {
     uint16 offset;
     uint32 xspan, q, r, d, xoffset;
     int32 xbottom;

     if (stored > max) fail("stored_to_orig: stored > max");

     if (x1 >= x0) {
       xbottom = x0;
       xspan = (uint32)x1 - (uint32)x0;
       offset = stored;
     }
     else {
       xbottom = x1;
       xspan = (uint32)x0 - (uint32)x1;
       offset = max - stored;
     }

     /* We knew xspan would fit in a uint32, but we needed to   */
     /* cast x0 and x1 before subtracting because otherwise the */
     /* subtraction could overflow, and ANSI doesn't say what   */
     /* the result will be in that case.                        */

     /* Let's optimize two common simple cases */
     /* before handling the general case:      */

     if (xspan == max) {
       xoffset = offset;
     }
     else if (xspan <= 0xffff) {
       /* Equation 2 won't overflow and does only one division. */
       xoffset = (offset * xspan + (max>>1)) / max;
     }
     else {
       #if HAVE_UINT48
         /* We can use equation 2 and do one uint48     */
         /* division instead of three uint32 divisions. */
         xoffset = (offset * (uint48)xspan + (max>>1)) / max;
       #else
         q = xspan / max;
         r = xspan % max;
         /* Hopefully those were compiled into one instruction. */
         d = offset * r;
         xoffset = offset * q + d/max + (d%max + (max>>1)) / max;
       #endif
     }

     /* xoffset might not fit in an int32, but we know the sum */
     /* xbottom + xoffset will, so we can do the addition on   */
     /* unsigned integers and then cast.                       */

     return (int32)((uint32)xbottom + xoffset);
   }


   /***********************************************/
   /* Map from original samples to stored samples */

   uint16
   orig_to_stored(int32 orig, uint16 max, int32 x0, int32 x1)

   #if 0

     Returns the stored sample corresponding to the given original
     sample.  The parameters max, x0, and x1 must have been
     approved by samp_params_ok().

     The pCAL spec says:

     stored = ((orig - x0) * max + (x1-x0)/2) / (x1-x0)
              clipped to the range 0..max

     Notice that all three terms are nonnegative, or else all
     are nonpositive.  Just as in stored_to_orig(), we can avoid
     overflow and rounding problems by transforming the equation to
     use unsigned quantities:

     stored = (xoffset * max + xspan/2) / xspan

   #endif

   {
     uint32 xoffset, xspan;

     if (x0 < x1) {
       if (orig < x0) return 0;
       if (orig > x1) return max;
       xspan = (uint32)x1 - (uint32)x0;
       xoffset = (uint32)orig - (uint32)x0;
     }
     else {
       if (orig < x1) return 0;
       if (orig > x0) return max;
       xspan = (uint32)x0 - (uint32)x1;
       xoffset = (uint32)x0 - (uint32)orig;
     }

     /* For 16-bit xspan the calculation is straightforward: */

     if (xspan <= 0xffff)
       return (xoffset * max + (xspan>>1)) / xspan;

     /* Otherwise, the numerator is more than 32 bits and the   */
     /* denominator is more than 16 bits.  The tricks we played */
     /* in stored_to_orig() depended on the denominator being   */
     /* 16-bit, so they won't help us here.                     */

     #if HAVE_UINT48
       return ((uint48)xoffset * max + (xspan>>1)) / xspan;
     #else

       /* Doing the exact integer calculation with 32-bit         */
       /* arithmetic would be very difficult.  But xspan > 0xffff */
       /* implies xspan > max, in which case the pCAL spec says   */
       /* "there can be no lossless reversible mapping, but the   */
       /* functions provide the best integer approximations to    */
       /* floating-point affine transformations."  So why insist  */
       /* on using the integer calculation?  Let's just use       */
       /* floating-point.                                         */

       return ((double)xoffset * max + (xspan>>1)) / xspan;

     #endif
   }


   /*********************************************/
   /* Check x0, x1, eqtype, n, and p[0]..p[n-1] */

   int
   phys_params_ok(int32 x0, int32 x1, int eqtype, int n, double *p)

   /* Returns 1 if x0, x1, eqtype, n, and p[0]..p[n-1] */
   /* have allowed values, 0 otherwise.                */

   {
     if (!samp_params_ok(1,x0,x1)) return 0;

     switch (eqtype) {
       case 0: return n == 2;
       case 1: return n == 3;
       case 2: break;
       case 3: return n == 4;
     }

     /* eqtype is 2, check for pow() domain error: */

     if (p[2] > 0) return 1;
     if (p[2] < 0) return 0;
     return (x0 <= x1) ? (x0 > 0 && x1 > 0) : (x0 < 0 && x1 < 0);
   }


   /************************************************/
   /* Map from original samples to physical values */

   double
   orig_to_phys(int32 orig, int32 x0, int32 x1,
                int eqtype, double *p)

   /* Returns the physical value corresponding to the given    */
   /* original sample.  The parameters x0, x1, eqtype, and p[] */
   /* must have been approved by phys_params_ok().  The array  */
   /* p[] must hold enough parameters for the equation type.   */

   {
     double xdiff, f;

     xdiff = (double)x1 - x0;

     switch (eqtype) {
       case 0:  f = orig / xdiff;
                break;
       case 1:  f = exp(p[2] * orig / xdiff);
                break;
       case 2:  f = pow(p[2], orig / xdiff);
                break;
       case 3:  f = sinh(p[2] * (orig - p[3]) / xdiff);
                break;
       default: fail("orig_to_phys: unknown equation type");
     }

     return p[0] + p[1] * f;
   }