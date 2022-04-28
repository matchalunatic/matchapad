#define MIDI_BASIC
#define MIDI_ADVANCED
#define ENCODER_RESOLUTION 4

/*
  Set any config.h overrides for your specific keymap here.
  See config.h options at https://docs.qmk.fm/#/config_options?id=the-configh-file
*/
#define USB_SUSPEND_WAKEUP_DELAY 0
#define RGB_MATRIX_STARTUP_SPD 60

#define SEQUENCER_STEPS 64
#define SEQUENCER_TRACKS 6

#undef ORYX_CONFIGURATOR
#define WS2812_BYTE_ORDER WS2812_BYTE_ORDER_GBR
