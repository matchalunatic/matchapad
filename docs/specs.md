# matchapad sequencer

## What's WIP

Planck EZ support
MIDI sysex
Manual
Seq8 mode

## What's next

Build a 5x5
more seq modes
pad mode

## Layouts

SAVE: save config
LOAD: load config
MODE: cycle through modes

### Planck EZ

```
                                                           
____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
                                                           
____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
                                                           
____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____ ____
                                                           
____ ____ ____ ____ ____ _________ ____ ____ ____ ____ ____
```

### Matcha Pad

```
                        
____ ____ ____ ____ ____
                        
____ ____ ____ ____ ____
                        
____ ____ ____ ____ ____
                        
____ ____ ____ ____ ____
                        
____ ____ ____ ____ ____

```

## General considerations

Numbers are using a special braille encoding (no number prefix, see manual).

color indicators keys serve another purpose in general (save and load).

The following flashing colors indicate these functions:

flashing red: decrement
flashing green: increment
flashing blue: change mode of last pressed key
flashing yellow: switch feature
constant yellow: switch mode from performance to edition
constant blue: switch mode from edition to performance

UNSURE

operation of the matchapad is mode-based. you pick a mode, you enter the edit mode where you will
enable and disable stuff as needed. then you exit edit mode and enter performance mode where you
will just be able to start and stop some things but not to alter programs.

## Features

### Several features

Rotate through:
- pad feature
- launcher feature
- sequencer feature (8 steps, 12 instruments)
- sequencer feature (16 steps, 12 instruments)
- sequencer feature (32 steps, 8 instruments)
- sequencer feature (64 steps, 8 instruments)


### Acceptable MIDI support

- Consume external clock.
- Emit MIDI clock.
- Send and load config using SysEx.
- Save config to EEPROM/Flash

### Pad feature

Launch MIDI notes and CC by pressing/releasing.

Yellow keys are MIDI notes and depend on their position
Blue keys are MIDI CC (127/0 toggle)
Green keys are MIDI CC (temporary toggle to 127, 0 on release)
Pink keys are MIDI CC (macro, oscillate from 0 to 127 in a beat, from 127 to 0 in the next beat)

### Launcher feature

Load sequencer programs by loading them. See them advance in the note section.

### Sequencer feature

A sequencer with 8, 16, 32 or 64 steps.

## MIDI considerations

### How to send notes

NoteOn with velocity set to 127
NoteOff will not be sent: we will use NoteOn with velocity 0


