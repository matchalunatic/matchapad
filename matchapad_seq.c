#include "matchapad_types.h"
#include "matchapad_seq.h"
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;
extern MidiDevice midi_device;
// root note for SEQ8 is C3
#define SEQ8_ROOT_NOTE 48
#define PROCESS_NOTE(d, note) (((d).i##note##_on) ? midi_send_noteon(&midi_device, 0, (SEQ8_ROOT_NOTE + (note)), 127) : NULL)
#define PROCESS_NOTEOFF(d, note) (((d).i##note##_on) ? midi_send_noteon(&midi_device, 0, (SEQ8_ROOT_NOTE + (note)), 0) : NULL)
// let's assume our maximum is 6000 intervals / minute for starters
#define MIDI_MIN_STEP 10
/* zero the seq8 program given */

void matchapad_seq8_zero_program(matchapad_seq8_program * program) {
  int i = 0;
  matchapad_seq8_step* progptr = (matchapad_seq8_step *)(program);
  for (; i < 8; i++) {
    progptr[i] = (matchapad_seq8_step){.mask = 0x000};
  }
}

void matchapad_seq8_handle_tick(uint8_t position) {
  switch (position) {
    case 0:
      matchapad_seq8_issue_notes(matchapad_seq8_next_step());
      break;
    default:
      if (position == matchapad_state.midi_ppqn) {
        matchapad_seq8_issue_note_off();
      }
      
    break;
  }
}

void matchapad_seq8_dummy_h(uint8_t) {
  dprintln("dummy handler");
}

const matchapad_driver_t matchapad_seq8_driver = {
  .tick_handler = &matchapad_seq8_handle_tick,
  .noteplay_handler = &matchapad_seq8_dummy_h,
  .noterelease_handler = &matchapad_seq8_dummy_h,
};

/* initialize seq8 */
void matchapad_seq8_init(void) {
  uprintln("matchapad_seq8_init");
  if (matchapad_internal_state.seq8_active_program == NULL) {
    matchapad_internal_state.seq8_active_program = malloc(sizeof(matchapad_seq8_program));
    matchapad_seq8_zero_program(matchapad_internal_state.seq8_active_program);
  }
  matchapad_internal_state.driver = &matchapad_seq8_driver;
  uprintln("matchapad_seq8_init: done");
}

/* set a step into the given seq8 program */
void matchapad_seq8_set_step(matchapad_step step, matchapad_seq8_program * program, matchapad_seq8_step stepmap) {
  ((matchapad_seq8_step *)(program))[step] = stepmap;
}


/* load a test program into the seq8 active sequence */
void matchapad_seq8_load_test_program(void) {
  uprintln("matchapad_seq8_load_test_program");
  matchapad_seq8_program * prog = matchapad_internal_state.seq8_active_program;
  matchapad_seq8_zero_program(prog);
  uprintln("matchapad_seq8_load_test_program: zero");
  matchapad_seq8_set_step(0, prog, (matchapad_seq8_step){.mask=0b100000000000});
  matchapad_seq8_set_step(1, prog, (matchapad_seq8_step){.mask=0b100000000000});
  matchapad_seq8_set_step(2, prog, (matchapad_seq8_step){.mask=0b100000000000});
  matchapad_seq8_set_step(3, prog, (matchapad_seq8_step){.mask=0b100000000000});
  matchapad_seq8_set_step(4, prog, (matchapad_seq8_step){.mask=0b100000000000});
  matchapad_seq8_set_step(5, prog, (matchapad_seq8_step){.mask=0b101111111111});
  matchapad_seq8_set_step(6, prog, (matchapad_seq8_step){.mask=0b011111111111});
  matchapad_seq8_set_step(7, prog, (matchapad_seq8_step){.mask=0b111111111111});
  //matchapad_seq8_set_step(1, prog, (matchapad_seq8_step){{1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0}});
  //matchapad_seq8_set_step(2, prog, (matchapad_seq8_step){{1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0});
  //matchapad_seq8_set_step(3, prog, (matchapad_seq8_step){{0, 1, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0});
  //matchapad_seq8_set_step(4, prog, (matchapad_seq8_step){{1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0});
  //matchapad_seq8_set_step(5, prog, (matchapad_seq8_step){{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0});
  //matchapad_seq8_set_step(6, prog, (matchapad_seq8_step){{0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0});
  //matchapad_seq8_set_step(7, prog, (matchapad_seq8_step){{0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1});
  uprintln("matchapad_seq8_load_test_program: loaded");
}

/* debug seq8 contents */
void matchapad_seq8_debug(void) {
  if (matchapad_state.active_mode != MODE_SEQ8) {
    return;
  }
  matchapad_seq8_program * prog = matchapad_internal_state.seq8_active_program;
  matchapad_seq8_step* progptr = (matchapad_seq8_step *)(prog);
  int i = 0;
  for (; i < 8; i++) {
    matchapad_seq8_show_step(*progptr);
    progptr++;
  } 
}

/* utility to show a note mask */
void matchapad_seq8_show_step(matchapad_seq8_step step) {
    uprintf("%b %b %b %b %b %b %b %b %b %b %b %b\n",
      step.i1_on, 
      step.i2_on, 
      step.i3_on, 
      step.i4_on, 
      step.i5_on, 
      step.i6_on, 
      step.i7_on, 
      step.i8_on, 
      step.i9_on, 
      step.i10_on, 
      step.i11_on, 
      step.i12_on);
}

/* get a step note mask for the given step number of the given seq8 program */
matchapad_seq8_step matchapad_seq8_get_step(matchapad_step step, matchapad_seq8_program * prog) {
  if (step > 7) {
    return (matchapad_seq8_step){.mask = 0xfff};
  }
  matchapad_seq8_step* progptr = (matchapad_seq8_step *)(prog);
  return progptr[step];
}

matchapad_seq8_step matchapad_seq8_get_step_by_number(matchapad_step step) {
  return matchapad_seq8_get_step(step, matchapad_internal_state.seq8_active_program);
}

void matchapad_seq8_set_step_by_number(matchapad_step step, matchapad_seq8_step notes) {
  matchapad_seq8_set_step(step, matchapad_internal_state.seq8_active_program, notes);
}

/* advance one step forward in the seq8 active program and return the set notes */
matchapad_seq8_step matchapad_seq8_next_step(void) {
  if (matchapad_internal_state.current_step > 7) {
    matchapad_internal_state.current_step = 0;
  }
  return matchapad_seq8_get_step(matchapad_internal_state.current_step++, matchapad_internal_state.seq8_active_program);
}
void matchapad_seq8_issue_notes(matchapad_seq8_step step) {
  PROCESS_NOTE(step, 1);
  PROCESS_NOTE(step, 2);
  PROCESS_NOTE(step, 3);
  PROCESS_NOTE(step, 4);
  PROCESS_NOTE(step, 5);
  PROCESS_NOTE(step, 6);
  PROCESS_NOTE(step, 7);
  PROCESS_NOTE(step, 8);
  PROCESS_NOTE(step, 9);
  PROCESS_NOTE(step, 10);
  PROCESS_NOTE(step, 11);
  PROCESS_NOTE(step, 12);
  matchapad_internal_state.seq8_last_step = step;
  // this is very QMK
  // defer_exec(MIDI_MIN_STEP, matchapad_seq8_issue_note_off_cb, NULL);
}

void matchapad_seq8_play_next_step(void) {
  matchapad_seq8_issue_notes(matchapad_seq8_next_step());
}

// this is quite specific to QMK
uint32_t matchapad_seq8_issue_note_off_cb(uint32_t trigger_time, void *cb_arg) {
  matchapad_seq8_issue_note_off();
  return 0;
}

void matchapad_seq8_issue_note_off(void) {
  matchapad_seq8_step * s = &matchapad_internal_state.seq8_last_step;
  PROCESS_NOTEOFF(*s, 1);
  PROCESS_NOTEOFF(*s, 2);
  PROCESS_NOTEOFF(*s, 3);
  PROCESS_NOTEOFF(*s, 4);
  PROCESS_NOTEOFF(*s, 5);
  PROCESS_NOTEOFF(*s, 6);
  PROCESS_NOTEOFF(*s, 7);
  PROCESS_NOTEOFF(*s, 8);
  PROCESS_NOTEOFF(*s, 9);
  PROCESS_NOTEOFF(*s, 10);
  PROCESS_NOTEOFF(*s, 11);
  PROCESS_NOTEOFF(*s, 12);
}

