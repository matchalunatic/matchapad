/**
  This defines the base stuff for the matcha pad
**/
#include QMK_KEYBOARD_H
#include "matchapad_types.h"

matchapad_state_t matchapad_state = {
  .midi_mode = MODE_LISTEN,
  .midi_ppqn = PPQN_24,
};

matchapad_internal_state_t matchapad_internal_state = {
  .load_sysex = 0,
  .midi_clock_interval_walker = 0,
  .computed_qn_intervals = {25}, // sane default
  .prev_read_clock = 0,
  .midi_play = true,
};
