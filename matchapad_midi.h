#pragma once
void manage_midi_clock(void);
void save_config_with_sysex(void);
void load_config_from_sysex(void);
void matchapad_process_sysex(MidiDevice *device, uint16_t start_byte, uint8_t data_length, uint8_t *data);
void matchapad_init_midi(void);
