/*=============================================================================
   Copyright (c) 2014-2023 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_SIN_COS_GEN_HPP_APRIL_26_2023)
#define CYCFI_Q_SIN_COS_GEN_HPP_APRIL_26_2023

#include <q/support/base.hpp>
#include <q/support/literals.hpp>
#include <cmath>

namespace cycfi::q
{
   ////////////////////////////////////////////////////////////////////////////
   // sin_cos_gen: Generates sine and cosine waves simultaneously. Generates
   // sine and cosine waves at the same time. The Chamberlin state variable
   // filter is optimized for infinite Q oscillation.
   //
   // 1. Suitable as a low frequency sine wave oscillator. The upper
   //    frequency limit for stability is roughly 1/6 sps.
   // 2. At low frequencies, the waveform symmetry is very pure, but it
   //    becomes skewed as you approach the upper limit.
   // 3. For low frequencies, we can reduce the calculation of the
   //    coefficient to 2*Pi*freq/sps.
   // 4. When utilized with fixed point or IEEE floating point, the
   //    oscallator should run forever without instability.
   //
   // Source:
   // https://www.earlevel.com/main/2003/03/02/the-digital-state-variable-filter/
   ////////////////////////////////////////////////////////////////////////////
   struct sin_cos_gen
   {
      sin_cos_gen(frequency f, float sps)
       : a(2.0 * faster_sin(1_pi * as_float(f) / sps))
      {
      }

      std::pair<float, float> operator()()
      {
         s[0] = s[0] - a*s[1];
         s[1] = s[1] + a*s[0];
         return {s[1], s[0]};
      }

      void config(frequency f, float sps)
      {
         a = 2.0 * faster_sin(1.0_pi * as_float(f) / sps);
      }

      double a;
      float s[2] = {1.0f, 0.0f};
   };
}

#endif
