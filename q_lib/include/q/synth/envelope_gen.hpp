/*=============================================================================
   Copyright (c) 2014-2023 Joel de Guzman. All rights reserved.

   Distributed under the MIT License [ https://opensource.org/licenses/MIT ]
=============================================================================*/
#if !defined(CYCFI_Q_SYNTH_ENVELOPE_HPP_MAY_17_2018)
#define CYCFI_Q_SYNTH_ENVELOPE_HPP_MAY_17_2018

#include <q/support/literals.hpp>
#include <algorithm>

namespace cycfi::q
{
   ////////////////////////////////////////////////////////////////////////////
   // envelope: Generates ADSR envelopes. attack_rate, decay_rate,
   // sustain_level, sustain_rate and release_rate determine the envelope
   // shape. The trigger member functions start the attack. The envelope can
   // be retriggered multiple times. The release member function starts the
   // release.
   ////////////////////////////////////////////////////////////////////////////
   class envelope_gen
   {
   public:

      static constexpr float hysteresis = 0.0001; // -80dB

      enum state_enum
      {
         note_off_state       = 0
       , attack_state         = 6
       , legato_state         = 5
       , decay_state          = 4
       , sustain_state        = 3
       , release_state        = 2
       , note_release_state   = 1
      };

      struct config
      {
         // Default settings

         duration             attack_rate    = 30_ms;
         duration             decay_rate     = 70_ms;
         decibel              sustain_level  = -6_dB;
         duration             sustain_rate   = 50_s;
         duration             release_rate   = 100_ms;
      };

                              envelope_gen(config const& config_, float sps);
                              envelope_gen(float sps);

      float                   operator()();
      float                   current() const { return _y; }
      void                    trigger(float level, int auto_decay = 1);
      void                    legato();
      void                    decay();
      void                    release();
      state_enum              state() const;

      void                    attack_rate(duration rate, float sps);
      void                    decay_rate(duration rate, float sps);
      void                    sustain_level(float level);
      void                    sustain_rate(duration rate, float sps);
      void                    release_rate(duration rate, float sps);
      void                    release_rate(float rate);
      void                    note_off_level(float level);

      float                   velocity() const        { return _level; }
      float                   sustain_level() const   { return _sustain_level; }

   private:

      void                    update_legato();
      void                    update_attack();
      void                    update_decay();
      void                    update_sustain();
      void                    update_release();

      state_enum              _state = note_off_state;
      float                   _y = 0.0f;
      float                   _attack_rate;
      float                   _level;
      float                   _decay_rate;
      float                   _legato_level;
      float                   _sustain_level;
      float                   _start_sustain_level;
      float                   _sustain_rate;
      float                   _release_rate;
      float                   _note_off_level = 0.0f;
      float                   _end_note_off_level = 0.0f;
      int                     _auto_decay = 1;
   };

   ////////////////////////////////////////////////////////////////////////////
   // envelope implementation
   ////////////////////////////////////////////////////////////////////////////
   inline envelope_gen::envelope_gen(config const& config_, float sps)
    : _attack_rate(fast_exp3(-2.0f / (sps * as_double(config_.attack_rate))))
    , _decay_rate(fast_exp3(-2.0f / (sps * as_double(config_.decay_rate))))
    , _sustain_level(as_float(config_.sustain_level))
    , _sustain_rate(fast_exp3(-2.0f / (sps * as_double(config_.sustain_rate))))
    , _release_rate(fast_exp3(-2.0f / (sps * as_double(config_.release_rate))))
   {}

   inline envelope_gen::envelope_gen(float sps)
    : envelope_gen(config{}, sps)
   {}

   inline void envelope_gen::attack_rate(duration rate, float sps)
   {
      _attack_rate = fast_exp3(-2.0f / (sps * as_double(rate)));
   }

   inline void envelope_gen::decay_rate(duration rate, float sps)
   {
      _decay_rate = fast_exp3(-2.0f / (sps * as_double(rate)));
   }

   inline void envelope_gen::sustain_level(float level)
   {
      _sustain_level = level;
   }

   inline void envelope_gen::sustain_rate(duration rate, float sps)
   {
      _sustain_rate = fast_exp3(-2.0f / (sps * as_double(rate)));
   }

   inline void envelope_gen::release_rate(duration rate, float sps)
   {
      _release_rate = fast_exp3(-2.0f / (sps * as_double(rate)));
   }

   inline void envelope_gen::release_rate(float rate)
   {
      if (rate < 1.0f)
         _release_rate = rate;
   }

   inline void envelope_gen::note_off_level(float level)
   {
      if (level < _y)
      {
         _note_off_level = level;
         _end_note_off_level = level * 0.1;
      }
   }

   inline float envelope_gen::operator()()
   {
      switch (_state)
      {
         case note_off_state:
            return 0.0f;

         case legato_state:
            update_legato();
            break;

         case attack_state:
            update_attack();
            break;

         case decay_state:
            update_decay();
            break;

         case sustain_state:
            update_sustain();
            break;

         case note_release_state:
         case release_state:
            update_release();
            break;
      }

      return _y;
   }

   inline void envelope_gen::trigger(float level, int auto_decay)
   {
      if (_y < level)
      {
         _auto_decay = auto_decay;
         _level = level;
         _state = attack_state;
      }
   }

   inline void envelope_gen::legato()
   {
      if (_state == sustain_state && _y < _start_sustain_level)
      {
         _auto_decay = -1;
         _legato_level = _start_sustain_level;
         _state = legato_state;
      }
   }

   inline void envelope_gen::decay()
   {
      _auto_decay = 1; // auto decay after attack
   }

   inline void envelope_gen::update_legato()
   {
      _y = _legato_level + _attack_rate * (_y - _legato_level);
      if (_y < _legato_level + hysteresis)
      {
         _y = _legato_level;
         _state = sustain_state;
      }
   }

   inline void envelope_gen::update_attack()
   {
      _y = 1.6f + _attack_rate * (_y - 1.6f);
      if (_y > _level)
      {
         _y = _level;
         switch (_auto_decay)
         {
            case 1: _state = decay_state; break;
            case -1: _state = sustain_state; break;
         }
      }
   }

   inline void envelope_gen::update_decay()
   {
      auto level = _level * _sustain_level;
      _y = level + _decay_rate * (_y - level);
      if (_y < level + hysteresis)
      {
         _y = level;
         _start_sustain_level = level;
         _state = sustain_state;
      }
   }

   inline void envelope_gen::update_sustain()
   {
      _y *= _sustain_rate;
   }

   inline void envelope_gen::release()
   {
      if (_state != note_off_state)
         _state = release_state;
   }

   inline void envelope_gen::update_release()
   {
      _y = _note_off_level + _release_rate * (_y - _note_off_level);
      if (_y < _note_off_level + hysteresis)
      {
         _y = _note_off_level;
         if (_y < _end_note_off_level)
            _state = note_release_state;
         else if (_y < hysteresis)
            _state = note_off_state;
      }
   }

   inline envelope_gen::state_enum envelope_gen::state() const
   {
      return _state;
   }
}

#endif
