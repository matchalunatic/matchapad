/**
# MIDI handling

## `void manage_midi_clock(void)`

If matchapad_state.MIDI_MODE is set to MASTER, emit clock messages
Else if matchapad_state.MIDI_MODE is set to LISTEN, listen to clock
messages and adjust the internal clock to follow them.
Else don't handle MIDI clock at all

## `void save_config_with_sysex(void)`

Our Manufacturer ID is defined by default to 7D (private/experimenting).

Emit the following sequence of data over sysex (F0 xxx F7) so that
Patch Librarian can consume them:

- 0x23
- matchapad state
- 0x07
- matchapad sequencer A (8)
- 0xAC
- matchapad sequencer B (16)
- 0xAB
- matchapad sequencer C (32)
- 0x19
- matchapad sequencer D (64)
- 0x05
- matchapad pad config
- 0x23

## `void load_config_from_sysex(void)`

Restore `save_config_with_sysex` style dump to internal state.

## SysEx codes reference

*Manufacturer ID: 0x7D (private)*

Enter load config mode: 0x7D304050
Enter save config mode: 0x7D504030

Sequencer state payload:
7D: manufacturer code
00|01|02: clock mode

**/
#include QMK_KEYBOARD_H
#include "matchapad_types.h"
#include "matchapad_utils.h"
#include "sysex_tools.h"

extern MidiDevice midi_device;
extern matchapad_state_t matchapad_state;
extern matchapad_internal_state_t matchapad_internal_state;


int compare_uint16(const void * a, const void * b) {
  const uint16_t * c = a;
  const uint16_t * d = b;
  return (*c > *d) - (*c < *d);
}

void matchapad_update_qn_interval(uint16_t bpm, uint8_t collec_len, uint16_t* bpms) {
  // insert this to the head
  memmove(bpms + 1, bpms, (collec_len - 1) * sizeof(uint16_t));
  bpms[0] = bpm;
}

uint16_t get_array_median(const uint16_t * ary, uint8_t len) {
  uint16_t * st = malloc(sizeof(uint16_t) * len);
  uint16_t median;
  memcpy(st, ary, len * sizeof(uint16_t));

  // now sort things
  qsort(st, len, sizeof(uint16_t), &compare_uint16);
  // and return the middle value
  median = st[len / 2];
  free(st);
  return median;
}

void manage_midi_clock(void) {
  switch (matchapad_state.midi_mode) {
    case MODE_MASTER:
      break;
    case MODE_LISTEN:
      break;
    case MODE_UNSET:
    default:
      break;
  }
}

void dumpstuff(char * prefix, void* pointer, int len) {
  int i = 0;
  uprintf("%s>0x", prefix);
  for (; i < len; i++) {
    uprintf("%02x", ((uint8_t *)pointer)[i]);
  }
  uprintln("<");
}

void dumpstuff2(char * prefix, void* pointer, int len) {
  int i = 0;
  dprintf("%s>0x", prefix);
  for (; i < len; i++) {
    uprintf("%02x", ((uint8_t *)pointer)[i]);
  }
  uprintln("<");
}

void save_config_with_sysex(void) {
  uint16_t size_to_send;
  uint8_t* sysex_message;
  uint8_t* config_message;
  size_t mesgsize = sizeof(uint8_t) + sizeof(matchapad_state_t);
  config_message = malloc(mesgsize);
  config_message[0] = 0x7D;
  memcpy(config_message + 1, (uint8_t*) &matchapad_state, sizeof(matchapad_state_t));
  sysex_message = malloc(sysex_encoded_length(mesgsize));
  uprintln("save_config_with_sysex");
  size_to_send = sysex_encode(sysex_message, config_message, mesgsize);
  // do the send
  midi_send_array(&midi_device, 4, (uint8_t[]){0xF0, 0x7D, 0x23, 0xF7});
  midi_send_array(&midi_device, size_to_send, sysex_message);
  dumpstuff("save_config_with_sysex:sent", sysex_message, size_to_send);
  midi_send_array(&midi_device, 4, (uint8_t[]){0xF0, 0x7D, 0x07, 0xF7});
  // TODO: send more data (sequencer data)
}

void load_config_from_sysex(void) {
  matchapad_internal_state.load_sysex = 0xFF;
}

int compare_sysex_buf(uint8_t size, ...) {
  va_list args;
  va_start(args, size);
  int outcome;
  uint8_t * toCompare = malloc((2 + size) * sizeof(uint8_t));
  toCompare[0] = 0xF0;
  for (outcome = 0; outcome < size; outcome++) {
    toCompare[outcome + 1] = va_arg(args, int);
  }
  toCompare[outcome + 1] = 0xF7;
  va_end(args);
  outcome = memcmp(toCompare, matchapad_internal_state.sysex_guard, size + 2);
  free(toCompare);
return outcome;
}

void matchapad_midi_tick(void) {
  // handle a MIDI tick
  t_matchapad_tick_h * th = matchapad_internal_state.driver->tick_handler;
  (*th)(matchapad_internal_state.midi_clock_interval_walker);
}

#define _CHECK_SYSEX_GUARD(val) (compare_sysex_buf(1, (val)) == 0)
#define _CHECK_SYSEX_GUARD_2(val1, val2) (compare_sysex_buf(2, (val1), (val2)) == 0)
#define _CHECK_SYSEX_GUARD_3(val1, val2, val3) (compare_sysex_buf(3, (val1), (val2), (val3)) == 0)
#define _CHECK_SYSEX_GUARD_4(val1, val2, val3, val4) (compare_sysex_buf(4, (val1), (val2), (val3), (val4)) == 0)
__attribute__((optimize("unroll-loops")))
void matchapad_process_realtime(MidiDevice *device, uint8_t byte) {
  uint32_t average_interval = 0;
  uint8_t i;
  switch (matchapad_state.midi_mode) {
    case MODE_LISTEN:
      switch (byte) {
        case 0xFA: // MIDI start
          matchapad_internal_state.midi_play = true;
          matchapad_internal_state.midi_clock_interval_walker = 0; // force recount clock
          break;
        case 0xFC: // MIDI stop
          matchapad_internal_state.midi_play = false;
          break;
        case 0xF8: // MIDI clock
          switch (matchapad_internal_state.midi_clock_interval_walker) {
            case 0:
              matchapad_internal_state.midi_clock_timer = timer_read32();
              // matchapad_internal_state.midi_clock_interval[0] = 0;
              matchapad_internal_state.prev_read_clock = 0;
              matchapad_internal_state.midi_clock_interval_walker++;
              break;
            case 1 ... BPM_ONE_MEASURE_COUNT - 1:
              // populate the walker array with intervals
              uint8_t * w = &(matchapad_internal_state.midi_clock_interval_walker);
              uint32_t *prev_value = &(matchapad_internal_state.prev_read_clock);
              uint32_t elapsed = timer_elapsed32(matchapad_internal_state.midi_clock_timer);

              matchapad_internal_state.midi_clock_interval[*w - 1] = elapsed - *prev_value;
              (*w)++;
              *prev_value = elapsed;
              break;
            case BPM_ONE_MEASURE_COUNT:
              for (i = 0; i < BPM_ONE_MEASURE_COUNT - 1; i++) {
                average_interval += matchapad_internal_state.midi_clock_interval[i];
              }
              // now offset by 16 bits 
              average_interval <<= 16;
              // BPM_ONE_MEASURE_COUNT samplings: we measure one less interval
              average_interval /= (BPM_ONE_MEASURE_COUNT - 1);
              // now unshift to get back to our actual number
              average_interval >>= 16;
              // now let's get the one-quarter-note interval = average_interval * ppqn
              average_interval = average_interval * matchapad_state.midi_ppqn;
              matchapad_update_qn_interval((uint16_t)average_interval, BPM_SAMPLE_COUNT, matchapad_internal_state.computed_qn_intervals);
              matchapad_internal_state.computed_bpm = 60000 / get_array_median(matchapad_internal_state.computed_qn_intervals, BPM_SAMPLE_COUNT);
              dprintf("computed BPM: %d PPQN %d\n", matchapad_internal_state.computed_bpm, matchapad_state.midi_ppqn);
              matchapad_internal_state.midi_clock_interval_walker = 0;
              break;
            default:
              uprintln("bad clock management");
              break;
          } // switch midi_clock_interval_walker
          // now issue a tick if we are playing
          if (matchapad_internal_state.midi_play) {
            matchapad_midi_tick();
          }
          break;
        default:
          uprintf("unimplemented realtime byte 0x%02X\n", byte);
          break;
      } // switch byte

      break;
    case MODE_MASTER:
    case MODE_UNSET:
      break;
    default:
      uprintln("incorrect matchapad_state.midi_mode");
  }
}

void matchapad_process_sysex(MidiDevice *device, uint16_t start_byte, uint8_t data_length, uint8_t *data) {
  void * matchapad_state_pointer = &matchapad_state;
  dumpstuff2("matchapad_process_sysex called", data, data_length);
  
  // first copy the sysex message to the sysex buffer
  memcpy(matchapad_internal_state.sysex_guard + start_byte, data, data_length);
  if (matchapad_internal_state.sysex_guard[start_byte + data_length - 1] != 0xF7) {
    uprintf("s %d l %d v %02X\n", start_byte, data_length, matchapad_internal_state.sysex_guard[start_byte + data_length]);
    uprintln("sysex in progress");
    return;
  }
  switch (matchapad_internal_state.load_sysex) {
    case 0xFF: // initial
      if (_CHECK_SYSEX_GUARD_2(0x7D, 0x23)) {
        dprintln("matchapad_process_sysex: got sysex 0x23 -- starting");
        matchapad_internal_state.load_sysex = 0x01;
      }
    break;
    case 0x01: // step 1 - receive matchapad_state
      dprintf("matchapad_process_sysex: writing matchapad_state[%d:%d]\n", start_byte, data_length - 1);
      matchapad_state_dump();
      memcpy(matchapad_state_pointer, matchapad_internal_state.sysex_guard + 2, sizeof(matchapad_state_t));
      dprintln("matchapad_process_sysex: done writing matchapad_state");
      matchapad_internal_state.load_sysex = 0x02;
    break;
    case 0x02: // step 2 - 
      if (_CHECK_SYSEX_GUARD_2(0x7D, 0x07)) {
        dprintln("matchapad_process_sysex: got sysex 0x07 -- continuing");
        matchapad_internal_state.load_sysex = 0x03;
      }
    break;
    case 0x03: // step 3
      matchapad_internal_state.load_sysex = 0;
      // TODO: more steps
    break;
    case 0x00: // generic mode
      dprintln("generic sysex handler");
      if (_CHECK_SYSEX_GUARD_4(0x7D, 0x30, 0x40, 0x50)) {
        dprintln("magic sysex received, switching to load mode");
        matchapad_internal_state.load_sysex = 0xFF;
      } else if (_CHECK_SYSEX_GUARD_4(0x7D, 0x50, 0x40, 0x30)) {
        dprintln("magic sysex received, switching to dump mode");
        save_config_with_sysex();
      } else {
        dumpstuff("meh ", data, data_length);
      }
      
    break;
    default:
      uprintln("abnormal sysex state");
  }
  matchapad_state_dump();
}

void matchapad_init_midi(void) {
  matchapad_internal_state.sysex_guard = malloc(SYSEX_BUF_SIZE * sizeof(uint8_t));
  midi_register_sysex_callback(&midi_device, &matchapad_process_sysex);
  midi_register_realtime_callback(&midi_device, &matchapad_process_realtime);
}

