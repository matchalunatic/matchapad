Layouts and manual for matchapad.

# General principles

## Layout

Instrument lines are all the same length. Layout for instrument selection is vertical.
Notes are horizontally grouped in 4 quarter-note sections. Colors indicate features.
## SysEx

### Assigning CC codes

This is done through SysEx. By default the CC codes are as follows:

CCO1: CC 20
CCO2: CC 21
CCO3: CC 22
CCO4: CC 23
CCTg1: CC 24
CCTg2: CC 25
CCTg3: CC 26
CCTg4: CC 27
CTmp1: CC 28
CTmp2: CC 29
CTmp3: CC 30
CTmp4: CC 31

In order to assign other CC codes (temporarily for that session) you need to send the following CC sequence:

```midiCC
0xF0  // sysex start
0x7D  // manufacturer code (experimental)
0xCC  // sysex command: set CC values
0x??  // set CCO1 code (0x00 ... 0x8a)
0x??  // set CCO2 code
0x??  // set CCO3 code
0x??  // set CCO4 code
0x??  // set CCTg1 code
0x??  // set CCTg2 code
0x??  // set CCTg3 code
0x??  // set CCTg4 code
0x??  // set CTmp1 code
0x??  // set CTmp2 code
0x??  // set CTmp3 code
0x??  // set CTmp4 code (0x00 ... 0x8a)
0xF7  // sysex end
```

Code 0x8a will cause a CC to switch back to its default value.

You can use a MIDI Librarian to manage the settings.

### Assigning key types

This is done through SysEx. You can change the type of the keys to suit your needs. This is the CC sequence you need to send to change types:

```midiSysex
0xF0  // sysex start
0x7D  // manufacturer code (experimental)
0xCD  // sysex command: set CC note types
0x??  // CCO1: type (0x00 ... 0x05)
0x??  // CCO2: type
0x??  // CCO3: type
0x??  // CCO4: type
0x??  // CCTg1: type
0x??  // CCTg2: type
0x??  // CCTg3: type
0x??  // CCTg4: type
0x??  // CTmp1: type
0x??  // CTmp2: type
0x??  // CTmp3: type
0x??  // CTmp4: type (0x00 ... 0x05)
0xF7  // sysex end
```

Possible CC types:
- 0x00: default type for this button. Will make all CC go back to their initial types
- 0x01: oscillator CC
- 0x02: toggle CC
- 0x03: temporary CC
- 0x04: ramp CC (go up and down between 0 and 127 by steps of 16 by 8th-note)
- 0x05: disabled (turn this CC off)


### Assigning key functions

matchapad wants to be a hackable control surface.

So in all modes, you can override the default function of any key (except the switch mode key).

In the annex is a code table of all keys encountered, their hexadecimal designation when addressing
them through SysEx, their QMK keycode when relevant and their intent. Some keys are grouped in
continuous ranges.

### Dumping matchapad full config

In order to dump matchapad config, send the following sysex sequence

```midiSysex
0xF0            // sysex start
0x7D            // manufacturer code (experimental)
0x50 0x40 0x30  // sysex command: emit matchapad full config.
0xF7            // sysex end
```

You will then receive a replayable sysex sequence. See the following section
for details.


### Loading matchapad full config

You can directly set matchapad config by sending the following sysex sequence.
It represents the direct internal state of the `matchapad_state_t` structure
as a series of bytes. It's subject to change with major versions.

This code sets the matchapad in LISTEN mode.
```midiSysex
0xF0            // sysex start
0x7D            // manufacturer code (experimental)
0x30 0x40 0x50  // sysex command: enter LISTEN mode
0xF7            // sysex end
```

### Dumping matchapad transportable config

This sysex command will dump a transportable version of a matchapad config. You can use it to
transfer your setup to new hardware and even between a 4x12 and a 5x5 and across versions.

However, you will not be able to save and restore with it things like direct button assignments.

```midiSysex
0xF0            // sysex start
0x7D            // manufacturer code (experimental)
0x51 0x41 0x31  // sysex command: emit matchapad transportable config.
0xF7            // sysex end
```

# Pad


### 4x12 with space bar

```
                                                                                     
 ,-----------------------------------------------------------------------------------.
 | CCO1 | CCO2 |CCTg1 |CCTg2 |CTmp1 |SwType|ClkMod| CTmp2|CCTG3 |CCTG4 | CCO3 | CCO4 |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | C3   | C#3  | D3   | D#3  | E3   | F3   | F#3  | G3   | G#3  | A4   | A#4  | B4   |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | C2   | C#2  | D2   | D#2  | E2   | F2   | F#2  | G2   | G#2  | A3   | A#3  | B3   |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Oct+1| Oct-1|      |      |CTmp3 | Switch Mode |CTmp4 |      |      |      |      |
  -----------------------------------------------------------------------------------'
   Channel 1 notes                                  Channel 1 notes
```

### 5x5 ortho
 
```
 ,----------------------------------.
 | CCO1 |      |      |      |      |
 |------+------+------+------+------|
 | CCO2 |      |      |      |      |
 |------+------+------+------+------|
 | CTmp1|      |      |      |      |
 |------+------+------+------+------|
 | CTmp2|      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      | OctT |
  ----------------------------------'

```

## Types of keys

CCOx are CC oscillators: on their first quarter note of activation they switch to 127, on the next one, they go back to 0.
CCTgx are CC toggles: they are set to 127 when first pressed and switch back to 0 on the next press.
CTmpx are CC tmps: they are set to 127 when pressed and switch to 0 when released.

Notes are MIDI notes.
Oct+1 / Oct-1 shift the MIDI notes but not the CC codes. Color codes indicate where you are:
  When both are off, it means you are in .
  Oct-1 will go green -> orange -> red as you get to lower octaves.
  Oct+1 will go green -> orange -> red as you get to higher octaves.

# Seq8

```
 ,-----------------------------------------------------------------------------------.
 | TgI1 | TgI4 | TgI7 | TgI10| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 1
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI2 | TgI5 | TgI8 | TgI11| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 2
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI3 | TgI6 | TgI9 | TgI12| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 3
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Col1 | Col2 | Col3 | Col4 |      | Switch Mode |      |      |      |      |      | CONTROL LINE
  -----------------------------------------------------------------------------------'
 
 ,----------------------------------.
 | IB1  | IBIn |      |      | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 | NxtP | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```

## With a 4 x 12
To toggle a step for an instrument:
- Press and hold one of Col1...Col4 on the Control Line
- Press one of the StpX keys on either of LINE 1, LINE 2, LINE 3.
- The instrument at the crossing of the column and the line will be set for the given step.

Example: I want to set Stp4 for instrument 9.
- Instrument 9 is on line 3 (TgI9: toggle I9)
- Instrument 9 is on column 4.
- I press and holdt the Col4 button and then press the Stp4 button on line 3.

To toggle an instrument track: press the button corresponding to that instrument.

To see which instruments are set for a given key: the colors indicate which instruments are set.
If a given step on a line has more than one instrument set, its color will alternate between the
colors of instrument tracks it is set for.

## With a 5 x 5

## To address an instrument line

### Select an instrument by buttons

Press IBIn two times in a row to enter instrument select mode. The instruments line will turn deep blue.

Then input an instrument code to select an instrument.

Codes:

- I1: IB1 x 1
- I2: IB1 x 2
- I3: IB1 x 3
- I4: IB2 x 1
- I5: IB2 x 2
- I6: IB2 x 3
- I7: IB3 x 1
- I8: IB3 x 2
- I9: IB3 x 3
- I10 IB4 x 3
- I11 IB4 x 3
- I12 IB4 x 3

Each time you press an IB button, the IB line color will change:
- from deep blue to deep red
- from deep red to green
- from green to purple.

The only the selected instrument button will remain.

To exit selection, press any other Instrument button after starting. Otherwise, after 1 second, the code you selected will be final.

To switch to another instrument, press the IBIn button again and input another code.

To mute the active instrument, press its button. Note that all instruments on the same button will be muted so you can use this to
switch off entire layers of your rhythm at once.

### Select an instrument using MIDI CC

Send MIDI CC 0 with the instrument number (starting from 0x01 for I1 to 0x0C for I12

This instrument remains selected until you select another instrument.

## Seq16

```
 ,-----------------------------------------------------------------------------------.
 | TgI1 | TgI4 | TgI7 | TgI10| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 1
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI2 | TgI5 | TgI8 | TgI11| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 2
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | TgI3 | TgI6 | TgI9 | TgI12| Stp1 | Stp2 | Stp3 | Stp4 | Stp5 | Stp6 | Stp7 | Stp8 | LINE 3
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Col1 | Col2 | Col3 | Col4 | NxtP | Switch Mode |      |      |      |      |      | CONTROL LINE
  -----------------------------------------------------------------------------------'
 

```

Usage is the same as for Seq8 with one difference: the NxtP button.


### Set steps 9-16
To toggle a step for an instrument, it's the same thing as for Seq8. But in order to set steps
9 to 16, you need to press and hold NextP and ColX before pressing one of the Stp buttons.

### Toggle to/from Page 2

Press NxtP twice in a row, within one second.
Or send Midi CC 3 with value 0x02 to switch to page 2, and 0x01 to switch to page 1.

The NxtP button will change color from green to orange.

# Seq32

32 steps, 8 instruments

Instrument selection is similar to Seq16 but with the following sequences
and max 8 instruments:

Press IBIn two times in a row to enter instrument select mode. The instruments line will turn deep red.

Then input an instrument code to select an instrument.

Codes:

- I1: IB1 x 1
- I2: IB1 x 2
- I3: IB2 x 1
- I4: IB2 x 2
- I5: IB3 x 1
- I6: IB3 x 2
- I7: IB4 x 1
- I8: IB4 x 2

## Change pages and copy content

Not needed on 4x12. On 5x5, press PgB1 to switch to page 1, PgB2 to switch to page 2,
PgB1 + PgB2 to copy the current page to the other page.


```
 ,-----------------------------------------------------------------------------------.
 | Stp1 | Stp2 | Stp3 | Stp4 | Stp17| Stp18| Stp19| Stp20| Stp25| Stp26| Stp27| Stp28|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp5 | Stp6 | Stp7 | Stp8 | Stp21| Stp22| Stp23| Stp24| Stp29| Stp30| Stp31| Stp32|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp9 | Stp10| Stp11| Stp12|      |      |      | PgB1 | Inst1| Inst3| Inst5| Inst7|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp13| Stp14| Stp15| Stp16|      | Switch Mode | PgB2 | Inst2| Inst4| Inst6| Inst8|
  -----------------------------------------------------------------------------------'

 ,----------------------------------.
 | IB1  | IBIn | PgB1 | PgB2 | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 | NxtP | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```



# Seq64

64 steps, 8 instruments. Similar use to Seq32 except that there is twice as much pages.

## Change page and copy contents

### On a 4x12

Use PgB1 to switch to page 1  (steps 1-32)
Use PgB2 to switch to page 2 (steps 33-64)
Use PgB1 + PgB2 to copy the active page to the other page.

When on Page1, the PgB1 button will be lighted up in green.
When on Page2, the PgB2 button will be lighted up in green.

When pattern copy happens, the PgB1 + PgB2 button will flash in orange for 2 seconds.

### On a 5x5

5x5 has 4 pages of 16 steps each.
Use PgB1 to cycle through pages leftward (4->3->2->1->4)
Use PgB2 to cycle through pages righttward (4->3->2->1->4)
Use PgB1 + PgB2 to copy active page to all the other pages.

To know on which page you are, look at the lights:
Page 1: PgB1 in green, PgB2 off
Page 2: PgB1 in red, PgB2 in off
Page 3: PgB1 off, PgB2 in red
Page 4: PgB1 off, PgB2 in green

When copying happens, the PgB1 + PgB2 buttons will flash in orange for 2 seconds.

```
 ,-----------------------------------------------------------------------------------.
 | Stp1 | Stp2 | Stp3 | Stp4 | Stp17| Stp18| Stp19| Stp20| Stp25| Stp26| Stp27| Stp28|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp5 | Stp6 | Stp7 | Stp8 | Stp21| Stp22| Stp23| Stp24| Stp29| Stp30| Stp31| Stp32|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp9 | Stp10| Stp11| Stp12|      |      |      | PgB1 | Inst1| Inst2| Inst3| Inst4|
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | Stp13| Stp14| Stp15| Stp16|      | Switch Mode | PgB2 | Inst5| Inst6| Inst7| Inst8|
  -----------------------------------------------------------------------------------'

 ,----------------------------------.
 | IB1  | IBIn | PgB1 | PgB2 | SwMod|
 |------+------+------+------+------|
 | IB2  | Stp1 | Stp2 | Stp3 | Stp4 |
 |------+------+------+------+------|
 | IB3  | Stp5 | Stp6 | Stp7 | Stp8 |
 |------+------+------+------+------|
 | IB4  | Stp9 | Stp10| Stp11| Stp12|
 |------+------+------+------+------|
 |      | Stp13| Stp14| Stp15| Stp16|
  ----------------------------------'

```

# Annex

## matchapad keys

This is a code table of all keys encountered, their hexadecimal designation when addressing
them through SysEx, their QMK keycode when relevant and their original intent. Some keys are
grouped in continuous ranges because they form functional blocks.

We re

|Symbol  |SysEx  |QMK kc    |Intent                                        |
|--------|------:|---------:|:--------------------------------------------:|
|INS1 ...|0x10   |KC_F1...  |Toggle track 1 ...                            |
|INS12   |0x1c   |KC_F12    |Toggle track 12.                              |
|FUN1 ...|0x20   |KC_F13... |Activate Function 1 ...                       |
|FUN12   |0x2c   |KC_F24    |Activate Function 12.                         |
|STP1 ...|0x40   |KC_A      |Toggle step 1 of the sequencer ...            |
|STP52   |0x74   |KC_SLASH  |Toggle step 52 of the sequencer.              |
|STP53...|0x75   |KC_PRINTSC|Toggle step 53 of the sequencer ...           |
|        |       |REEN      |                                              |
|STP64   |0x80   |KC_DOWN   |Toggle step 64 of the sequencer.              |
|CCO1... |0x90   |KC_KP_PLUS|Trigger CCO1 ...                              |
|CC04    |0x93   |KC_KP_2   |Trigger CCO4.                                 |
|CCTg1...|0x94   |KC_KP_3   |Trigger CCTg1 ...                             |
|CCTg4   |0x97   |KC_KP_6   |Trigger CCTg4.                                |
|CTmp1...|0x98   |KC_KP_7   |Trigger CCTmp1 ...                            |
|CTmp4   |0x9b   |KC_KP_0   |Trigger CCTmp4.                               |
|        |       |          |                                              |
|        |       |          |                                              |
|        |       |          |                                              |
|        |       |          |                                              |
|        |       |          |                                              |
|        |       |          |                                              |

### Special keys

FUN6 is *always* SwitchMode.

### Direct key addressing

We reserve in these semantics all sysex designations space from 0xA0 to 0xEF for
direct 1-byte addressing of keys (79 entries). To address keys directly without
consideration for their previous functions, use the reference that follows.

#### 4x12 with space bar

Line 1 has top-4 bits set to 0xA 
Line 2 has top-4 bits set to 0xB
Line 3 has top-4 bits set to 0xC
Line 4, first half, has top-5 bits set to `11010`
Line 4, second half, has top-5 bits set to `11011`
Space bar is 0xEF.

```
 ,-----------------------------------------------------------------------------------.
 | 0xA0 |      |      |      |      |      |      |      |      |      |      | 0xAC |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | 0xB0 |      |      |      |      |      |      |      |      |      |      | 0xBC |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | 0xC0 |      |      |      |      |      |      |      |      |      |      | 0xCC |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 | 0xD0 |      |      |      | 0xD4 |    0xEF     | 0xDA |      |      |      | 0xDE |
  -----------------------------------------------------------------------------------'
 
```

#### 5x5 ortho

Columns are numbered from A to E and are the top 4 bits.
Lines are numbered from 0 to 4 and are the bottom 4 bits.

Each part of the grid can be read as coordinates.
```
 ,----------------------------------.
 | 0xA0 | 0xB0 | 0xC0 | 0xD0 | 0xE0 |
 |------+------+------+------+------|
 | 0xA1 | 0xB1 | 0xC1 | 0xD1 | 0xE1 |
 |------+------+------+------+------|
 | 0xA2 | 0xB2 | 0xC2 | 0xD2 | 0xE2 |
 |------+------+------+------+------|
 | 0xA3 | 0xB3 | 0xC3 | 0xD3 | 0xE3 |
 |------+------+------+------+------|
 | 0xA4 | 0xB4 | 0xC4 | 0xD4 | 0xE4 |
  ----------------------------------'

```

## Blank layouts

### 4x12 with space bar

```
 ,-----------------------------------------------------------------------------------.
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |      |      |      |      |      |      |      |
 |------+------+------+------+------+------+------+------+------+------+------+------|
 |      |      |      |      |      |             |      |      |      |      |      |
  -----------------------------------------------------------------------------------'
 
```

### 5x5 ortho
 
```
 ,----------------------------------.
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
 |------+------+------+------+------|
 |      |      |      |      |      |
  ----------------------------------'

```

## Numbers representation on a 5x5

base-10 and base-16 numbers, encoded this way. two numbers supported: units in the 4th column
(from left to right) and tens in the 2nd column.

Hundreds are represented by a change in color: if the top LED is red, you are in the
100s. If it is green, you are in the 200s. No other hundreds are supported.

We are using braille range from g to p to represent 0 - 9 and a - f to represent A-F

Hex characters are blue. Decimal characters are yellow. 

⠛ ⠓ ⠊ ⠚ ⠅ ⠇ ⠍ ⠝ ⠕ ⠏ ⠁ ⠃ ⠉ ⠙ ⠑ ⠋
0 1 2 3 4 5 6 7 8 9 A B C D E F


```
  Examples
                 TEN                                   TWENTY-FOUR                                FIFTY-SEVEN                 
 ,----------------------------------.      ,----------------------------------.      ,----------------------------------.      
 |      |   X  |      |   X  |   X  |      |      |      |   X  |   X  |      |      |      |   X  |      |  X   |  X   |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |   X  | X    |   X  |   X  |      |      |   X  |      |      |      |      |      |   X  |      |      |  X   |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |      |      |      |      |      |      |   X  |      |      |      |   X  |      |  X   |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      
 |------+------+------+------+------|      |------+------+------+------+------|      |------+------+------+------+------|      
 |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      |      
  ----------------------------------'       ----------------------------------'       ----------------------------------'      
```
