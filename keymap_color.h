#pragma once

// this is my chosen equivalent of \0 for colors.
#define COLOR_END .h = 254, .s = 254, .v = 255

#ifdef _COMPUTATIONAL_COLORS
// use these defines with numbers from 1 to 12
// HSV wheel of 12
#define COLOR_INS_SEQ8(n) .h = 255 / 4 * (n), .s = 255, .v =255
// HSV wheel of 12, offset by ~15 deg
#define COLOR_INS_SEQ16(n) .h = 255 / 12 * (n) - 15, .s = 255, .v =255
// HSV wheel of 8
#define COLOR_INS_SEQ32(n) .h = 255 / 8 * (n), .s = 255, .v =255
// HSV wheel of 8, offset by ~22 deg
#define COLOR_INS_SEQ64(n) .h = 255 / 8 * (n) - 16, .s = 255, .v =255
// HSV wheel of 4 instruments, no offset (count from 0)
#define COLOR_NOTEON_ROW1_INST_SEQ8(inst) .h = 255 / 4 * (n), .s = 255, .v =255
// HSV wheel of 4 instruments, offset by 30 deg
#define COLOR_NOTEON_ROW2_INST_SEQ8(inst) .h = 255 / 4 * (n) + 30, .s = 255, .v =255
// HSV wheel of 4 instruments, offset by 60 deg
#define COLOR_NOTEON_ROW3_INST_SEQ8(inst) .h = 255 / 4 * (n) + 60, .s = 255, .v =255

// arbitrary division with fixed value offset
#define COLOR_DIVISION(sectors,index,offset) .h = 255 / (sectors) * (index) - (offset), .s = 255, .v =255


#define C8_1 COLOR_INS_SEQ8(1)
#define C8_2 COLOR_INS_SEQ8(1)
#define C8_3 COLOR_INS_SEQ8(1)
#define C8_4 COLOR_INS_SEQ8(2)
#define C8_5 COLOR_INS_SEQ8(2)
#define C8_6 COLOR_INS_SEQ8(2)
#define C8_7 COLOR_INS_SEQ8(3)
#define C8_8 COLOR_INS_SEQ8(3)
#define C8_9 COLOR_INS_SEQ8(3)
#define C8_A COLOR_INS_SEQ8(4)
#define C8_B COLOR_INS_SEQ8(4)
#define C8_C COLOR_INS_SEQ8(4)

#define C16_1 COLOR_INS_SEQ16(1)
#define C16_2 COLOR_INS_SEQ16(2)
#define C16_3 COLOR_INS_SEQ16(3)
#define C16_4 COLOR_INS_SEQ16(4)
#define C16_5 COLOR_INS_SEQ16(5)
#define C16_6 COLOR_INS_SEQ16(6)
#define C16_7 COLOR_INS_SEQ16(7)
#define C16_8 COLOR_INS_SEQ16(8)
#define C16_9 COLOR_INS_SEQ16(9)
#define C16_A COLOR_INS_SEQ16(10)
#define C16_B COLOR_INS_SEQ16(11)
#define C16_C COLOR_INS_SEQ16(12)

#define C32_1 COLOR_INS_SEQ32(1)
#define C32_2 COLOR_INS_SEQ32(2)
#define C32_3 COLOR_INS_SEQ32(3)
#define C32_4 COLOR_INS_SEQ32(4)
#define C32_5 COLOR_INS_SEQ32(5)
#define C32_6 COLOR_INS_SEQ32(6)
#define C32_7 COLOR_INS_SEQ32(7)
#define C32_8 COLOR_INS_SEQ32(8)

#define C64_1 COLOR_INS_SEQ64(1)
#define C64_2 COLOR_INS_SEQ64(2)
#define C64_3 COLOR_INS_SEQ64(3)
#define C64_4 COLOR_INS_SEQ64(4)
#define C64_5 COLOR_INS_SEQ64(5)
#define C64_6 COLOR_INS_SEQ64(6)
#define C64_7 COLOR_INS_SEQ64(7)
#define C64_8 COLOR_INS_SEQ64(8)
#else 

// hand-picked HSV

// instruments 1, 2 and 3 are here
#define C8_1 .h = 0,   .s = 240, .v = 240
#define C8_2 .h = 0,   .s = 240, .v = 240
#define C8_3 .h = 0,   .s = 240, .v = 240
#define C8_4 .h = 50,  .s = 240, .v = 240
#define C8_5 .h = 50,  .s = 240, .v = 240
#define C8_6 .h = 50,  .s = 240, .v = 240
#define C8_7 .h = 100, .s = 240, .v = 240
#define C8_8 .h = 100, .s = 240, .v = 240
#define C8_9 .h = 100, .s = 240, .v = 240
#define C8_A .h = 150, .s = 240, .v = 240
#define C8_B .h = 150, .s = 240, .v = 240
#define C8_C .h = 150, .s = 240, .v = 240

// row colors

#define CR_1 .h = 30,  .s = 120, .v = 240
#define CR_2 .h = 80,  .s = 120, .v = 240
#define CR_3 .h = 130, .s = 120, .v = 240
#define CR_4 .h = 180, .s = 120, .v = 240
#define CR_5 .h = 30,  .s = 240, .v = 240
#define CR_6 .h = 80,  .s = 240, .v = 240
#define CR_7 .h = 130, .s = 240, .v = 240
#define CR_8 .h = 180, .s = 240, .v = 240


#endif
#define CFN0 .h = 0, .s = 255, .v =255
#define CFN1 .h = 0, .s = 255, .v =255
#define CFN2 .h = 0, .s = 255, .v =255
#define CFN3 .h = 0, .s = 255, .v =255
#define CFN4 .h = 0, .s = 255, .v =255
#define CFN5 .h = 0, .s = 255, .v =255
#define CFN6 .h = 0, .s = 255, .v =255
#define CFN7 .h = 0, .s = 255, .v =255
#define CFN8 .h = 0, .s = 255, .v =255
#define CFN9 .h = 0, .s = 255, .v =255
#define CFNA .h = 0, .s = 255, .v =255
#define CFNB .h = 0, .s = 255, .v =255
#define CFNC .h = 0, .s = 255, .v =255
#define CFND .h = 0, .s = 255, .v =255
#define CFNE .h = 0, .s = 255, .v =255
#define CFNF .h = 0, .s = 255, .v =255

typedef struct {
  union {
    struct {
      double h;
      double s;
      double l;
    };
    double components[3];
  };
} HSLuv;

typedef struct {
  union {
    struct {
      double r;
      double g;
      double b;
    };
    double components[3];
  };
} fRGB;
