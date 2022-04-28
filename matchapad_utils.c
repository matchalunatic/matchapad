#include QMK_KEYBOARD_H
#include "matchapad_types.h"
#include "matchapad_seq.h"
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;

void matchapad_state_dump(void) {
  uprintln("*** matchapad state ***");
  uprintf("clock mode: %d\n", matchapad_state.midi_mode);
  uprintf("clock PPQN: %d\n", matchapad_state.midi_ppqn);
  uprintf("active mode: %d\n", matchapad_state.active_mode);
}

void matchapad_internal_state_dump(void) {
  int i;
  uprintln("*** matchapad internal state ***");
  uprintf("load_sysex? %d\n", matchapad_internal_state.load_sysex);
  uprintf("sysex_guard: 0x");
  for (i = 0; i < SYSEX_BUF_SIZE; i++) {
    uprintf("%02X", matchapad_internal_state.sysex_guard[i]);
  }
  uprintln(".");
  uprintln("QN intervals:");
  for (i = 0; i < BPM_SAMPLE_COUNT; i++) {
    uprintf("%d: %d, ", i, matchapad_internal_state.computed_qn_intervals[i]);
  }
  uprintln(".");
  uprintf("Computed BPM: %d\n", matchapad_internal_state.computed_bpm);
  uprintf("Current step: %d\n", matchapad_internal_state.current_step);
  uprintf("MIDI play: %b\n", matchapad_internal_state.midi_play);
}

void matchapad_switch_mode(matchapad_mode mode) {
  matchapad_state.active_mode = mode;
  switch (mode) {
    case MODE_SEQ8:
      uprintln("matchapad_switch_mode: seq8");
      matchapad_seq8_init();
    break;
    default:
      uprintln("mode not implemented yet");
  }
}

void matchapad_rotate_mode(void) {
  switch (matchapad_state.active_mode) {
    case MODE_SEQ8:
      matchapad_switch_mode(MODE_SEQ16);
      break;
    case MODE_SEQ16:
      matchapad_switch_mode(MODE_SEQ32);
      break;
    case MODE_SEQ32:
      matchapad_switch_mode(MODE_SEQ64);
      break;
    case MODE_SEQ64:
      matchapad_switch_mode(MODE_PAD);
      break;
    case MODE_PAD:
      matchapad_switch_mode(MODE_LAUNCHER);
      break;
    case MODE_LAUNCHER:
      matchapad_switch_mode(MODE_PIANOKEYS);
      break;
    case MODE_PIANOKEYS:
      matchapad_switch_mode(MODE_SEQ8);
      break;
    default:
      dprintln("incorrect mode, bug in matchapad");
  }
}
