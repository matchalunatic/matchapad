#include QMK_KEYBOARD_H
#include "eeprom.h"
#include "keymap_color.h"
#include "matchapad_utils.h"
#include "matchapad_midi.h"
#include "matchapad_seq.h"
#include "keymap_us_international.h"
#define DEV_RGB(n) (n).r, (n).g, (n).b
#define DEV_RGBP(n) (n)->r, (n)->g, (n)->b
#define DEV_fRGB(n) (int)(255 * (n).r), (int)(255 * (n).g), (int)(255 * (n).b)
#define DEV_fRGBP(n) (int)(255 *(n)->r), (int)(255 * (n)->g, (int)(255 * (n)->b)


enum planck_keycodes {
  fRGB_SLD = EZ_SAFE_RANGE,
};

enum planck_layers {
  _BASE,
  _SEQ8,
  _SEQ16,
  _SEQ32,
  _SEQ64,
  _PAD,
};

#define INS1 KC_F1
#define INS2 KC_F2
#define INS3 KC_F3
#define INS4 KC_F4
#define INS5 KC_F5
#define INS6 KC_F6
#define INS7 KC_F7
#define INS8 KC_F8
#define INS9 KC_F9
#define INSA KC_F10
#define INSB KC_F11
#define INSC KC_F12
// works for ~80 notes
#define ZNOTE(n) ((n) < 53 ? KC_A + (n) : KC_PRINT_SCREEN + (n) - 53)
#define Z8(n,m) ZNOTE((n - 1) * 8 + (m - 1))
#define Z16(n,m) ZNOTE((n - 1) * 16 + (m - 1))
#define Z32(n,m) ZNOTE((n - 1) * 32 + (m - 1))
#define Z64(n,m) ZNOTE((n - 1) * 64 + (m - 1))
#define FUN1 KC_F13
#define FUN2 KC_F14
#define FUN3 KC_F15
#define FUN4 KC_F16
#define FUN5 KC_F17
#define FUN6 KC_F18
#define FUN7 KC_F19
#define FUN8 KC_F20
#define FUN9 KC_F21
#define FUNA KC_F22
#define FUNB KC_F23
#define FUNC KC_F24

extern rgb_config_t rgb_matrix_config;
extern rgb_config_t rgb_matrix_config;
extern sequencer_config_t sequencer_config;
extern MidiDevice midi_device;
// add a RESET key on the trident so I can dev
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
  [_BASE] = LAYOUT_planck_grid(
    KC_A,    KC_C, KC_F, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    KC_B,    KC_D, RESET, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    DEBUG,   KC_E, RESET, RESET, RESET, DEBUG, DEBUG, RESET, RESET, RESET, RESET, KC_NO,
    RESET,   RESET, RESET, RESET, RESET, FUN6, KC_NO, RESET, RESET, RESET, RESET, KC_NO
  ),
  [_SEQ8] = LAYOUT_planck_grid(
    INS1, INS4, INS7,  INSA,   Z8(1,1), Z8(1,2), Z8(1,3), Z8(1,4), Z8(1,5), Z8(1,6), Z8(1,7), Z8(1,8),
    INS2, INS5, INS8,  INSB,   Z8(2,1), Z8(2,2), Z8(2,3), Z8(2,4), Z8(2,5), Z8(2,6), Z8(2,7), Z8(2,8),
    INS3, INS6, INS9,  INSC,   Z8(3,1), Z8(3,2), Z8(3,3), Z8(3,4), Z8(3,5), Z8(3,6), Z8(3,7), Z8(3,8),
    FUN1, FUN2, FUN3,  FUN4,   FUN5,    FUN6,    KC_NO,   FUN7,    FUN8,    FUN9,    FUNA,    FUNB
  ),
};

typedef struct {
  union {
    uint16_t funmask : 16;
    struct {
      bool fun1_state : 1;
      bool fun2_state : 1;
      bool fun3_state : 1;
      bool fun4_state : 1;
      bool fun5_state : 1;
      bool fun6_state : 1;
      bool fun7_state : 1;
      bool fun8_state : 1;
      bool fun9_state : 1;
      bool funa_state : 1;
      bool funb_state : 1;
      bool func_state : 1;
      bool fund_state : 1;
      bool fune_state : 1;
      bool funf_state : 1;
      bool fun0_state : 1;
    };
  };
} matchapad_keymap_state_t;

// struct that holds the wanted colors in HSV for configuration
// note: HSV = 24 bits = 3 bytes
typedef struct {
  union {
    struct {
      HSV row_1_color_on;
      HSV row_2_color_on;
      HSV row_3_color_on;
      HSV row_4_color_on;
      HSV row_5_color_on;
      HSV row_6_color_on;
      HSV row_7_color_on;
      HSV row_8_color_on;
    };
    HSV row_colors[8];
  };
  union {
    struct {
      HSV inst_1_color;
      HSV inst_2_color;
      HSV inst_3_color;
      HSV inst_4_color;
      HSV inst_5_color;
      HSV inst_6_color;
      HSV inst_7_color;
      HSV inst_8_color;
      HSV inst_9_color;
      HSV inst_10_color;
      HSV inst_11_color;
      HSV inst_12_color;
    };
    HSV inst_colors[12];
  };
} matchapad_lightboard_state_t;

matchapad_keymap_state_t matchapad_keymap_state = {
  .funmask = 0x0,
};

matchapad_lightboard_state_t matchapad_lightboard_state = {
  .row_colors = {
    {CR_1},
    {CR_2},
    {CR_3},
    {CR_4},
    {CR_5},
    {CR_6},
    {CR_7},
    {CR_8},
  },
  .inst_colors = {
    {C8_1},
    {C8_2},
    {C8_3},
    {C8_4},
    {C8_5},
    {C8_6},
    {C8_7},
    {C8_8},
    {C8_9},
    {C8_A},
    {C8_B},
    {C8_C},

  },
};
/**
Seq8 keyboard usage
FUN1 + Z(1,x) -> set note for INS1
FUN1 + Z(2,x) -> set note for INS2
FUN1 + Z(3,x) -> set note for INS3
FUN2 + Z(1,x) -> set note for INS4
FUN2 + Z(2,x) -> set note for INS5
FUN2 + Z(3,x) -> set note for INS6
... for FUN3 and FUN4 too

FUN5: unused.
INS1...INSC: mute/unmute track
FUN7 + INSX: zero track INSX
**/

inline HSV dim_color(const HSV c) {
  return (HSV){
    .h = c.h,
    .s = c.s,
    .v =c.v / 2
  };
}

#define MIN(a,b) ((a) > (b) ? (b) : (a))
inline HSV up_color(const HSV c) {
  return (HSV){
    .h = c.h,
    .s = c.s,
    .v =MIN(c.v * 2, 1)
  };
}

// use that to count the number of bits set in a mask, useful for rotators
uint8_t count_set_bits(uint16_t n) {
  uint8_t count = 0;
  while (n) {
    n &= (n - 1);
    count++;
  }
  return count;
}

void matchapad_init_keymap(void) {
}

void keyboard_post_init_user(void) {
  rgb_matrix_enable();
  matchapad_init_keymap();
  matchapad_init_midi();
}

void km_matchapad_rotate_mode(void) {
  matchapad_rotate_mode();
  switch (matchapad_state.active_mode) {
    case MODE_SEQ8:
      uprintln("switching to MODE_SEQ8");
      layer_move(_SEQ8);
    break;
    default:
      uprintf("unimplemented mode %d\n", matchapad_state.active_mode);
      layer_move(_BASE);
      break;
  }
}

void matchapad_kp_toggle_notes_seq8(const matchapad_keymap_state_t * state, int row, int col) {
  uprintln("matchapad_kp_toggle_notes_seq8");
  // fetch the step
  matchapad_seq8_step orig_step = matchapad_seq8_get_step_by_number(col - 1);
  // identify which tracks are set
  matchapad_seq8_step flip_steps = {
    .i1_on = state->fun1_state && row == 1,
    .i2_on = state->fun1_state && row == 2,
    .i3_on = state->fun1_state && row == 3,
    .i4_on = state->fun2_state && row == 1,
    .i5_on = state->fun2_state && row == 2,
    .i6_on = state->fun2_state && row == 3,
    .i7_on = state->fun3_state && row == 1,
    .i8_on = state->fun3_state && row == 2,
    .i9_on = state->fun3_state && row == 3,
    .i10_on = state->fun4_state && row == 1,
    .i11_on = state->fun4_state && row == 2,
    .i12_on = state->fun4_state && row == 3,
  };
  // now let's negate the set bits for our selected columns.
  orig_step.mask ^= flip_steps.mask;
  matchapad_seq8_set_step_by_number(col - 1, orig_step);

  
}

void matchapad_kp_toggle_notes(const matchapad_keymap_state_t * state, int row, int col) {
  // using fmask, note row and note column, set notes (1-based)
  switch (matchapad_state.active_mode) {
    case MODE_SEQ8:
      return matchapad_kp_toggle_notes_seq8(state, row, col);
    default:
      uprintln("matchapad_kp_toggle_notes: not implemented");
  }
}

void matchapad_kb_seq8_rotate_colors(void);
bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  matchapad_kb_seq8_rotate_colors();
  switch (keycode) {
    case FUN1...FUNC:
      if (record->event.pressed) {
        matchapad_keymap_state.funmask |= 1 << (keycode - FUN1);
      }
      else {
        matchapad_keymap_state.funmask &= ~(1 << (keycode - FUN1));
      }
    break;
    default:
    break;
  }
  if (!record->event.pressed) {
    return true;
  }
  switch (keycode) {
    case Z8(1,1) ... Z8(1,8):
      matchapad_kp_toggle_notes(&matchapad_keymap_state, 1, keycode - Z8(1,1) + 1);
      return false;
      break;
    case Z8(2,1) ... Z8(2,8):
      matchapad_kp_toggle_notes(&matchapad_keymap_state, 2, keycode - Z8(2,1) + 1);
      return false;
      break;
    case Z8(3,1) ... Z8(3,8):
      matchapad_kp_toggle_notes(&matchapad_keymap_state, 3, keycode - Z8(3,1) + 1);
      return false;
      break;
    case FUN6:
      dprintln("matchapad: rotating mode");
      km_matchapad_rotate_mode();
      return false;
      break;
    case FUN5:
      uprintln ("save config with sysex");
      save_config_with_sysex();
      return false;
      break;
    case FUN7:
      uprintln ("undef");
      return false;
    case FUN8:
      matchapad_seq8_show_step(matchapad_seq8_next_step());
      matchapad_internal_state_dump();
      return false;
    case FUN9:
      matchapad_seq8_play_next_step();
      matchapad_internal_state_dump();
      return false;
    case FUNA:
      uprintln ("load config from sysex");
      load_config_from_sysex();
      return false;
    case FUNB:
      matchapad_seq8_debug();
      return false;
  }
  return true;
}


#define NOFF .h = 0, .s = 0, .v =0
const int PROGMEM seq8_inst_button_map[] = {
  0, 12, 24, 1, 13, 25, 2, 14, 26, 3, 15, 27
};


typedef struct {
  union {
    struct {
      HSV inst1;
      HSV inst2;
      HSV inst3;
      HSV inst4;
      HSV inst5;
      HSV inst6;
      HSV inst7;
      HSV inst8;
      HSV inst9;
      HSV inst10;
      HSV inst11;
      HSV inst12;
    };
    HSV insts[12];
  };
} seq8_color_library_t;

typedef struct {
  union {
    struct {
      HSV fun0;
      HSV fun1;
      HSV fun2;
      HSV fun3;
      HSV fun4;
      HSV fun5;
      HSV fun6;
      HSV fun7;
      HSV fun8;
      HSV fun9;
      HSV funa;
      HSV funb;
      HSV func;
      HSV fund;
      HSV fune;
      HSV funf;
    };
    HSV functions[16];
  };
  HSV note_off;
  HSV note_on;
} matchapad_kb_color_library_t;

const seq8_color_library_t PROGMEM seq8_color_library = {
  .insts = {
    {C8_1},
    {C8_2},
    {C8_3},
    {C8_4},
    {C8_5},
    {C8_6},
    {C8_7},
    {C8_8},
    {C8_9},
    {C8_A},
    {C8_B},
    {C8_C},
  },
};

const matchapad_kb_color_library_t PROGMEM matchapad_kb_color_library = {
  .functions = {
    {CFN0},
    {CFN1},
    {CFN2},
    {CFN3},
    {CFN4},
    {CFN5},
    {CFN6},
    {CFN7},
    {CFN8},
    {CFN9},
    {CFNA},
    {CFNB},
    {CFNC},
    {CFND},
    {CFNE},
    {CFNF},
  },
  .note_off = {NOFF},
  .note_on = {.h = .3, .s = 1, .v =1},
};
#define INL(n) &(seq8_color_library.insts[(n)])
#define FNL(n) &(matchapad_kb_color_library.functions[(n)])
#define NOFP &(matchapad_kb_color_library.note_off)
const HSV * PROGMEM seq8_base_layout[] = {
  INL(1), INL(4), INL(7), INL(10), NOFP,   NOFP, NOFP, NOFP, NOFP, NOFP, NOFP, NOFP,
  INL(2), INL(5), INL(8), INL(11), NOFP,   NOFP, NOFP, NOFP, NOFP, NOFP, NOFP, NOFP,
  INL(3), INL(6), INL(9), INL(12), NOFP,   NOFP, NOFP, NOFP, NOFP, NOFP, NOFP, NOFP,
  FNL(1), FNL(2), FNL(3), FNL(4),  FNL(5), FNL(6),         FNL(7), FNL(8), FNL(9), FNL(10), FNL(11)
};
/**
    INS1, INS4, INS7,  INSA,   Z(1,1), Z(1,2), Z(1,3), Z(1,4), Z(1,5), Z(1,6), Z(1,7), Z(1,8),
    INS2, INS5, INS8,  INSB,   Z(2,1), Z(2,2), Z(2,3), Z(2,4), Z(2,5), Z(2,6), Z(2,7), Z(2,8),
    INS3, INS6, INS9,  INSC,   Z(3,1), Z(3,2), Z(3,3), Z(3,4), Z(3,5), Z(3,6), Z(3,7), Z(3,8),
    FUN1, FUN2, FUN3,  FUN4,     FUN5,   FUN6,  KC_NO, FUN7,   FUN8,   FUN9,   FUNA,   FUNB
**/

const int PROGMEM instrument_led_map_seq8[12] = {0, 12, 24, 1, 13, 25, 2, 14, 26, 3, 15, 27};
const HSV PROGMEM color_off = {.h = 0, .s = 0, .v = 0};
const HSV PROGMEM color_note_on = {.h = 0, .s = 255, .v = 255};  // FIXME


void matchapad_kb_seq8_rotate_colors(void) {
  // this is a generic rotate colors function, it's only useful for the demos as it
  // rotates colors for all instruments irrespective of which are active
  int row_to_instruments[3][4] = {
    [0] = {0, 3, 6, 9},
    [1] = {1, 4, 7, 10},
    [2] = {2, 5, 8, 11},
  };
  int i, j, k, l;
  HSV * row_color, * inst_color;
  // rotate through colors
  for (i = 0; i < 3; i++) {
    for (j = 0; j < 3; j++) {
      k = row_to_instruments[i][j];
      l = row_to_instruments[i][j+1];
      row_color = &matchapad_lightboard_state.row_colors[i];
      inst_color = &matchapad_lightboard_state.inst_colors[k];
      if (memcmp(row_color, inst_color, sizeof(HSV)) == 0) {
        matchapad_lightboard_state.row_colors[i] = matchapad_lightboard_state.inst_colors[l];
        break;
      }
    }
    if (j == 3) {
        matchapad_lightboard_state.row_colors[i] = matchapad_lightboard_state.inst_colors[row_to_instruments[i][0]];
        uprintln("rollover");
    }
    uprintf("rotated from %d\n", j);
  }
}

void seq8_kb_light_notes(HSV * colors[47], const matchapad_seq8_program * program, matchapad_step step) {
  // notes for instruments 1, 4, 7, A:  4 ... 11
  // notes for instruments 2, 5, 8, B: 16 ... 23
  // notes for instruments 3, 5, 8, C: 28 ... 35

  // notes for instrument buttons
  // 0,  1,  2,  3,
  // 12, 13, 14, 15,
  // 24, 25, 26, 27,
  uint8_t step_counter, inst_counter;
  bool row1_on, row2_on, row3_on;
  for (step_counter = 0; step_counter < 8; step_counter++) {
    for (inst_counter = 0; inst_counter < 12; inst_counter++) {
      row1_on = count_(program->steps[step_counter].mask & 0b001001001001) > 0;
      row2_on = (program->steps[step_counter].mask & 0b010010010010) > 0;
      row3_on = (program->steps[step_counter].mask & 0b100100100100) > 0;
      if (row1_on) {
        colors[4 + step_counter] = &matchapad_lightboard_state.row_1_color_on;
      }
      else {
        colors[4 + step_counter] = (HSV*)&matchapad_kb_color_library.note_off;
      }
      if (row2_on) {
        colors[16 + step_counter] = &matchapad_lightboard_state.row_2_color_on;
      }
      else {
        colors[16 + step_counter] = (HSV*)&matchapad_kb_color_library.note_off;
      }
      if (row3_on) {
        colors[28 + step_counter] = &matchapad_lightboard_state.row_3_color_on;
      }
      else {
        colors[28 + step_counter] = (HSV*)&matchapad_kb_color_library.note_off;
      }
    }
  }
  for (inst_counter = 0; inst_counter < 12; inst_counter++) {
    bool note_on = (program->steps[step].mask & (1 << inst_counter)) != 0;
    if (note_on) {
      colors[instrument_led_map_seq8[inst_counter]] = (HSV*)&matchapad_kb_color_library.note_on;
    }
  }

}

void rgb_matrix_seq8_renderfull(void) {
  // first, prepare base layout
  HSV * colors[47];
  memcpy(colors, seq8_base_layout, sizeof(seq8_base_layout));
  // then set adequate instrument colors
  for (int i = 0; i < 12; i++) {
    colors[seq8_inst_button_map[i]] = &matchapad_lightboard_state.inst_colors[i];
  }
  // then override based on set notes and play state
  seq8_kb_light_notes(colors, matchapad_internal_state.seq8_active_program, matchapad_internal_state.current_step);
  // then render
  RGB color;
  for (int i = 0; i < 47; i++) {
    color = hsv_to_rgb(*colors[i]);
    //uprintf("color: (%d,%d,%d)\n", DEV_fRGB(color));
    rgb_matrix_set_color(i, DEV_RGB(color));
  }
}

void rgb_matrix_seq8(void) {
  // TODO: optimize render to do only what's necessary
  rgb_matrix_seq8_renderfull();
}

void rgb_matrix_indicators_user(void) {
  switch (matchapad_state.active_mode) {
    case MODE_SEQ8:
      return rgb_matrix_seq8();
    break;
    default:
      dprintln("not implemented");
    break;
  }
}
