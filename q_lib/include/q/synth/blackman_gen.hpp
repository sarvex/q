/*=============================================================================
   Copyright (c) 2014-2023 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_BLACKMAN_GEN_HPP_APRIL_26_2023)
#define CYCFI_Q_BLACKMAN_GEN_HPP_APRIL_26_2023

#include <q/synth/sin_cos_gen.hpp>

namespace cycfi::q
{
   ////////////////////////////////////////////////////////////////////////////
   // Blackman window taper generator.
   //
   // The Blackman window is a type of window function that gradually tapers
   // the amplitude of a signal towards the window's edges. It is often used
   // in digital signal processing to smooth a signal (smoothing
   // discontinuities at the edges of sampled signals) or to reduce spectral
   // leakage. The Blackman window has a wider main lobe and lower level side
   // lobes than other windows. It is named after Robert Blackman, who first
   // described it in 1958.
   //
   // The formula for the Blackman window is:
   //    w(n) = 0.42 - 0.5 * cos(2pi*n/(N-1)) + 0.08cos(4pi*n/(N-1))
   //
   ////////////////////////////////////////////////////////////////////////////
   struct blackman_gen
   {
      blackman_gen(duration width, float sps)
       : cos1{frequency(1.0f / as_float(width)), sps}
       , cos2{frequency(1.0f / as_float(width/2)), sps}
      {
      }

      float operator()()
      {
         return 0.42f - 0.5f * cos1().second + 0.08f * cos2().second;
      }

      void config(duration width, float sps)
      {
         cos1.config(frequency(1.0f / as_float(width)), sps);
         cos2.config(frequency(1.0f / as_float(width/2)), sps);
      }

      sin_cos_gen cos1, cos2;
   };
}

#endif
