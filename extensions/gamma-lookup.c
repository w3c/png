   #if 0
   gamma-lookup.c 0.1.4 (Sat 19 Dec 1998)
   by Adam M. Costello <amc @ cs.berkeley.edu>

   This is public domain example code for computing gamma
   correction lookup tables using integer arithmetic.

   #endif
   #if __STDC__ != 1
   #error This code relies on ANSI C conformance.
   #endif


   #include <limits.h>
   #include <math.h>


   /* In this program a type named uintN denotes the  */
   /* smallest unsigned type we can find that handles */
   /* at least all values 0 through (2^N)-1.          */

   typedef unsigned char uint8;

   #if UCHAR_MAX >= 0xffff
     typedef unsigned char uint16;
   #else
     typedef unsigned short uint16;
   #endif

   #if UCHAR_MAX >= 0xffffffff
     typedef unsigned char uint32;
   #elif USHRT_MAX >= 0xffffffff
     typedef unsigned short uint32;
   #elif UINT_MAX >= 0xffffffff
     typedef unsigned int uint32;
   #else
     typedef unsigned long uint32;
   #endif


   /*********************/
   /* 16-bit arithmetic */

   void
   precompute16(uint16 L[511])

   /* Precomputes the log table (this requires floating point). */

   {
     int j;
     double f;

     /* L[j] will hold an integer representation of          */
     /* -log(j / 510.0).  Knowing that L[1] (the largest) is */
     /* 0xfe00 will help avoid overflow later, so we set the */
     /* scale factor accordingly.                            */

     f = 0xfe00 / log(1 / 510.0);

     for (j = 1;  j <= 510;  ++j)
       L[j] = log(j / 510.0) * f + 0.5;
   }


   void
   gamma16(uint16 L[511], uint8 G[256], uint16 g)

   /* Makes a 256-entry gamma correction lookup table G[] with */
   /* exponent g/pow(2,14), where g must not exceed 0xffff.    */

   {
     int i, j;
     uint16 x, y, xhi, ghi, xlo, glo;

     j = 1;
     G[0] = 0;

     for (i = 1;  i <= 255;  ++i) {
       x = L[i << 1];
       xhi = x >> 8;
       ghi = g >> 8;
       y = xhi * ghi;

       if (y > 0x3f80) {
         /* We could have overflowed later. */
         /* But now we know y << 2 > L[1].  */
         G[i] = 0;
         continue;
       }

       xlo = x & 0xff;
       glo = g & 0xff;
       y = (y << 2) + ((xhi * glo) >> 6) + ((xlo * ghi) >> 6);
       while (L[j] > y) ++j;
       G[i] = j >> 1;
     }
   }


   /*********************/
   /* 32-bit arithmetic */

   void
   precompute32(uint32 L[511])

   /* Precomputes the log table (this requires floating point). */

   {
     int j;
     double f;

     /* L[j] will hold an integer representation of         */
     /* -log(j / 510.0).  Knowing that L[1] (the largest)   */
     /* is 0x3ffffff will help avoid overflow later, so we  */
     /* set the scale factor accordingly.                   */

     f = 0x3fffffff / log(1 / 510.0);

     for (j = 1;  j <= 510;  ++j)
       L[j] = log(j / 510.0) * f + 0.5;
   }


   void
   gamma32(uint32 L[511], uint8 G[256], uint16 g)

   /* Makes a 256-entry gamma correction lookup table G[] with */
   /* exponent g/pow(2,14), where g must not exceed 0xffff.    */

   {
     int i, j;
     uint32 x, y;

     j = 1;
     G[0] = 0;

     for (i = 1;  i <= 255;  ++i) {
       x = L[i << 1];
       y = (x >> 14) * g;
       while (L[j] > y) ++j;
       G[i] = j >> 1;
     }
   }


   /**********************************************/
   /* Floating-point arithmetic (for comparison) */

   void
   gamma_fp(uint8 G[256], double g)

   /* Makes a 256-entry gamma correction */
   /* lookup table G[i] with exponent g. */

   {
     int i;

     G[0] = 0;

     for (i = 1;  i <= 255;  ++i)
       G[i] = pow(i/255.0, g) * 255 + 0.5;
   }