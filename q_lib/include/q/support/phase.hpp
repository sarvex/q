/*=============================================================================
   Copyright (c) 2014-2023 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_PHASE_HPP_MAY_2018)
#define CYCFI_Q_PHASE_HPP_MAY_2018

#include <q/support/base.hpp>
#include <q/support/literals.hpp>
#include <infra/assert.hpp>

namespace cycfi::q
{
   ////////////////////////////////////////////////////////////////////////////
   // Type safe representation of phase: the relationship in timing between a
   // periodic signal relative to a reference periodic signal of the same
   // frequency. Phase values run from 0 to 2π,  suitable for oscillators.
   // `phase` is represented as fixed point 1.31 format where 31 bits are
   // fractional.
   //
   // The turn, also cycle, full circle, revolution, and rotation, is a
   // complete circular movement or measure (as to return to the same point)
   // with circle or ellipse. A turn is abbreviated τ, cyc, rev, or rot
   // depending on the application. The symbol τ can also be used as a
   // mathematical constant to represent 2π radians.
   //
   //    https://en.wikipedia.org/wiki/Angular_unit
   //
   ////////////////////////////////////////////////////////////////////////////
   struct phase : value<std::uint32_t, phase>
   {
      using base_type = value<std::uint32_t, phase>;
      using base_type::base_type;

      constexpr static auto one_cyc = int_max<std::uint32_t>();
      constexpr static auto bits = sizeof(std::uint32_t) * 8;

      constexpr explicit            phase(value_type val = 0);
      constexpr explicit            phase(float frac);
      constexpr explicit            phase(double frac);
      constexpr explicit            phase(long double frac);
      constexpr                     phase(frequency freq, float sps);

      [[deprecated("Use as_float(db) instead of float(db)")]]
      constexpr explicit operator   float() const;

      [[deprecated("Use as_double(db) instead of double(db)")]]
      constexpr explicit operator   double() const;

      constexpr static phase        begin()     { return phase{}; }
      constexpr static phase        end()       { return phase(one_cyc); }
      constexpr static phase        middle()    { return phase(one_cyc/2); }
   };

   // Free functions
   constexpr double  as_double(phase d);
   constexpr float   as_float(phase d);

   ////////////////////////////////////////////////////////////////////////////
   // phase_iterator: iterates over the phase with an interval specified by
   // the supplied frequency.
   ////////////////////////////////////////////////////////////////////////////
   struct phase_iterator
   {
      constexpr                     phase_iterator();
      constexpr                     phase_iterator(frequency freq, float sps);

      constexpr phase_iterator      operator++(int);
      constexpr phase_iterator&     operator++();
      constexpr phase_iterator      operator--(int);
      constexpr phase_iterator&     operator--();

      constexpr phase_iterator&     operator=(phase rhs);
      constexpr phase_iterator&     operator=(phase_iterator const& rhs) = default;

      constexpr void                set(frequency freq, float sps);

      constexpr bool                first() const;
      constexpr bool                last() const;
      constexpr phase_iterator      begin() const;
      constexpr phase_iterator      end() const;
      constexpr phase_iterator      middle() const;

      phase                         _phase, _step;
   };

   ////////////////////////////////////////////////////////////////////////////
   // one_shot_phase_iterator: A variant of the phase_iterator that does not
   // wrap around when incrementing at the end or when decremented at the
   // beginning.
   //
   // Note: Branchfree Saturating Arithmetic using
   // http://locklessinc.com/articles/sat_arithmetic/
   //
   ////////////////////////////////////////////////////////////////////////////
   struct one_shot_phase_iterator : phase_iterator
   {
      using phase_iterator::phase_iterator;
      using phase_iterator::operator=;

      constexpr one_shot_phase_iterator      operator++(int);
      constexpr one_shot_phase_iterator&     operator++();
      constexpr one_shot_phase_iterator      operator--(int);
      constexpr one_shot_phase_iterator&     operator--();
   };

   ////////////////////////////////////////////////////////////////////////////
   // Implementation
   ////////////////////////////////////////////////////////////////////////////
   constexpr phase::phase(value_type val)
      : base_type{val}
   {}

   namespace detail
   {
      constexpr phase::value_type frac_phase(long double frac)
      {
         CYCFI_ASSERT(frac >= 0.0,
            "Frac should be greater than 0"
         );
         return (frac >= 1.0)?
            phase::end().rep :
            pow2<long double>(phase::bits) * frac;
      }

      constexpr phase::value_type frac_phase(double frac)
      {
         CYCFI_ASSERT(frac >= 0.0,
            "Frac should be greater than 0"
         );
         return (frac >= 1.0)?
            phase::end().rep :
            pow2<double>(phase::bits) * frac;
      }

      constexpr phase::value_type frac_phase(float frac)
      {
         CYCFI_ASSERT(frac >= 0.0f,
            "Frac should be greater than 0"
         );
         return (frac >= 1.0f)?
            phase::end().rep :
            pow2<float>(phase::bits) * frac;
      }
   }

   constexpr phase::phase(long double frac)
    : base_type{detail::frac_phase(frac)}
   {}

   constexpr phase::phase(double frac)
    : base_type{detail::frac_phase(frac)}
   {}

   constexpr phase::phase(float frac)
    : base_type{detail::frac_phase(frac)}
   {}

   constexpr phase::phase(frequency freq, float sps)
    : base_type((pow2<double>(bits) * as_double(freq)) / sps)
   {}

   constexpr double as_double(phase p)
   {
      constexpr auto denom = pow2<double>(p.bits);
      return p.rep / denom;
   }

   constexpr float as_float(phase p)
   {
      constexpr auto denom = pow2<float>(p.bits);
      return p.rep / denom;
   }

   constexpr phase::operator float() const
   {
      return as_float(*this);
   }

   constexpr phase::operator double() const
   {
      return as_double(*this);
   }

   constexpr phase_iterator::phase_iterator()
    : _phase{}
    , _step{}
   {}

   constexpr phase_iterator::phase_iterator(frequency freq, float sps)
    : _phase{}
    , _step{freq, sps}
   {}

   constexpr phase_iterator phase_iterator::operator++(int)
   {
      phase_iterator r = *this;
      _phase += _step;
      return r;
   }

   constexpr phase_iterator& phase_iterator::operator++()
   {
      _phase += _step;
      return *this;
   }

   constexpr phase_iterator phase_iterator::operator--(int)
   {
      phase_iterator r = *this;
      _phase -= _step;
      return r;
   }

   constexpr phase_iterator& phase_iterator::operator--()
   {
      _phase -= _step;
      return *this;
   }

   constexpr phase_iterator& phase_iterator::operator=(phase rhs)
   {
      _step = rhs;
      return *this;
   }

   constexpr void phase_iterator::set(frequency freq, float sps)
   {
      _step = {freq, sps};
   }

   constexpr bool phase_iterator::first() const
   {
      return _phase < _step;
   }

   constexpr bool phase_iterator::last() const
   {
      return (phase::end()-_phase) < _step;
   }

   constexpr phase_iterator phase_iterator::begin() const
   {
      auto r = *this;
      r._phase = phase::begin();
      return r;
   }

   constexpr phase_iterator phase_iterator::end() const
   {
      auto r = *this;
      r._phase = phase::end();
      return r;
   }

   constexpr phase_iterator phase_iterator::middle() const
   {
      auto r = *this;
      r._phase = phase::middle();
      return r;
   }

   constexpr one_shot_phase_iterator one_shot_phase_iterator::operator++(int)
   {
      one_shot_phase_iterator r = *this;
      ++(*this);
      return r;
   }

   constexpr one_shot_phase_iterator& one_shot_phase_iterator::operator++()
   {
      auto res = _phase.rep + _step.rep;
      res |= -(res < _phase.rep);
      _phase.rep = res;
      return *this;
   }

   constexpr one_shot_phase_iterator one_shot_phase_iterator::operator--(int)
   {
      one_shot_phase_iterator r = *this;
      --(*this);
      return r;
   }

   constexpr one_shot_phase_iterator& one_shot_phase_iterator::operator--()
   {
	   auto res = _phase.rep - _step.rep;
	   res &= -(res <= _phase.rep);
      _phase.rep = res;
      return *this;
   }
}

#endif
