/*
  ____________________

  Welcome to Ivee (IV7) - A Powerful Programable RPN Calculator based on FORTH
  Version 1.0, (c) 2020 by deetee/zooxo
  This software is covered by the 3-clause BSD license.
  See also: https://youtu.be/uZYgRxXX5ro
  ____________________

  ____________________

   PREAMBLE
  ____________________

  IV7 is a port of IVEE (see https://github.com/zooxo/iv) to the QYF-TM1638
  development board. The most obvious difference to IVEE and its matrix display
  is the old fashioned 8 digit 7 segment display of the QYF-Board. So no plots
  are possible, there is no business calculator and the representation of text
  (ie sorted FORTH commands) is very limited. On the other hand there was space
  left to implement a convenient matrix mode.
  Nevertheless IV7 offers nearly the same range of functionality like IVEE:
  - 120 intrinsic functions based on FORTH
  - Programming: Handle up to 40 user definable programs
                 (Edit, New, Rename, Move, Delete, Send, Load)
  - Dictionary of all commands, words and programs
  - Fast user definable menu (longpressed F-key)
  - Calculations with complex numbers for nearly every function
  - Calculus (f(x), solve, integrate)
  - Basic math operations in number systems with selectable BASE
  - Save up to 10 numbers/constants permanently
  - Statistics and line best fit
  - Matrix mode to calculate (complex) matrices
  - Many unit conversions
  - Clock function
  - Basic system functions (Battery voltage, Brightness, Screensaver)

  Have fun!
  deetee


  ____________________

   KEYBOARD, CODES
  ____________________

    F(G)[OFF]{MENU}  7(SUM+)[SUMCLR]  8(PRG)[FREE]    9(/)[MAT]
    E(SWAP)[ROT]     4(DICT)[USR]     5(FX)[BASE]     6(*)[TXT]
    N(OVER)[PICK]    1(RCL)[STO]      2(FSOLV)[FINT]  3(-)[LIT-]
    X(CLR)[TOFF]     0(CPLX)[]        .(CVIEW)[CLK]   #(+)[LIT+]

    PRG Shortcuts:                         BASE/HEX Shortcuts:
      E UP    5 OUT  6 UP                     E EXIT  4 E  5 F
      N DOWN  1 NEW  2 IN   3 DOWN                    1 B  2 C    3 D
      X EXIT  0 REN  . DEL  D EDIT            X CLR   0 A  . A~F

    MENU/DICT/REN Navigation:
             A   B   C   D    A: NUMERIC, MATH
      UP     E   4   5   6    B: TRIG/HYP, STAT/LR, CPLX, BASE, FN, LOGIC, SYSTM
      DOWN   N   1   2   3    C: STACK, PRG, CONVERT
             PREV0   .NEXT    D: USERPRGS

    BUTTONCODES (used by GETKEY):   SUM/STAT/L.R. Storage Registers:
      F(15)  7(07)  8(08)  9(09)      7 SN  8 SX   9 SY
      E(14)  4(04)  5(05)  6(06)            5 SXX  6 SXY
      N(13)  1(01)  2(02)  3(03)
      X(12)  0(00)  .(10)  #(11)

    MATRIX
      F(OP)                7[1|1]           8[1|2]        9[1|3](INV)
      E[A<->B](SWAP)       4[2|1]           5[2|2]        6[2|3](A*B)
      N[POP<->VIEW](COPY)  1[3|1]           2[3|2]        3[3|3](A-B)
      X[EXIT](CLR)         0[FLUSH](TRANS)  .[LOAD](DET)  #[PUSH](A+B)

  ____________________

   DISPLAY, INDICATORS
  ____________________

     _              _    Prompt (regular and base mode)
     _

          _     .    |.  Numeric mode (F-key, G-key, number input, stack floated)


    |      |             Complex mode (rectangular, polar)
     _
    |      |
              | |  |_    Complex view (real, imaginary, absolute, angle)
     _    _
    |_|  |_
    |    |_              Program mode (select, edit)

    | |
         | |    .        Matrix mode (A, B, view)

  ____________________

   COMPLEX VIEW
  ____________________

    Use CVIEW and the cursor keys "E" or "0" respective "N" or "." to view the
    components of a complex number (real part, imaginary part, absolute value
    or angle). Note that the complex indicator indicates the viewed component:
       _
      |  (real), | (imaginary),  | | (absolute value) or |_ (angle)

    If you want to make ongoing calculations with one component press enter to
    copy the desired component to the stack. Note that the original complex
    number will be removed from the stack.
    Escape the complex viewing mode with "X".
    Like in IVEE the function CPLX is used to convert the stack to a complex
    number and vice versa. If the rectangular or polar mode is used shows the
    complex indicator (left or right comma). The used mode can be set by
    escaping the complex view mode either when viewing the rectangular
    (real/imaginary) or when viewing the polar (absolute/angle) components.

  ____________________

   MATRIX
  ____________________

    Despite of the simple display IV7 offers many functions for convenient
    dealing with (complex) 3x3-matrices. Possible operations are adding,
    substracting or multiplying matrices, calculating the inverse, transpose or
    determinant or moving data from the stack to a matrix and vice versa.

    After entering the matrix mode with "MAT" (F-9) you can toggle between
    matrix A and B (indicated with high or low quotation marks) with E as well
    as toggle between view mode (indicated with a dot) or STO-mode. When in
    store mode the displayed stack value can be stored to the appropriate
    position in the matrix with 789, 456 or 123.
    For instance to store 1.5 to row 2 and column 3 of matrix B enter 1.5 to
    the stack, enter the matrix mode (F-9), toggle to matrix B ("E", indicates
    low quotation marks) and press 6 (row 2 and column 3).
    To view matrix values toggle with "N" to view mode ("."-indicator) and
    select the appropriate location of the matrix element (ie "7" for the
    0|0-location).
    Please note that it is possible to load the whole matrix from stack
    (9 "reverse" entered values) with "." or to flush all matrix values to stack
    with "0". Loading an empty stack clears the whole matrix (like F-X).
    To copy a matrix (A to B or vice versa) press F-N.
    To examine one value of the matrix (ie for viewing the imaginary part) view
    it and push it with "#" to the stack.
    Operations like A=A+B can be done with F-#.
    Please note that matrix functions are neither programable nor selectable
    from the dictionary.
    To exit the matrix mode press "X".

  ____________________

   PROGRAMMING
  ____________________

    Example program to query button code (inclusive displaying text):
      tC                                     // Clear text register
      127 tP 28 tP 120 tP 120 tP 92 tP 84 tP // Write "Button" to text register
      tS                                     // Show text register
      1 PS                                   // Pause for 1 quarter second
      KY                                     // Query button code

  ____________________

   DICTIONARY
  ____________________

    Numerics (cursor keys E and N)
       _                                 _                _
      | |                |               _|               _|
      |_|      0         |      1       |_       2        _|      3
                        _                _                _
      |_|              |_               |_                 |
        |      4        _|      5       |_|      6         |      7
       _                _
      |_|              |_|                                _
      |_|      8        _|      9          .     .                NEGATE
       _   _            _                                 _
      |_  |_           |_| |             _   _           |_  |_
      |_  |_   EE      |   |    PI      |   |_   RCL      _| |_   STO

    Mathematics
       _                _                _
      |_|  _|  ADD     |_  |_   SUB         | |  MULT     _|      DIV
      | | |_|  +        _| |_|  -       | | |_|  *       |_| |    /
       _
           _|          | |               _                    _
      | | |_|  MOD     | |      ABS        .     INT        .     FRAC
       _                                 _   _                _
      |_               |    _           |_| |_|          |   |
      |_       EXP     |_  | |  LN      |    _   POW10   |_  |_|  LOG
       _                                 _                    _
      | |              |                |_|              |   |    LN
      |        SQRT    |        INV     |        POW     |_  |    (GAMMA)
       _   _            _   _
      |   | |          |_| |_            _|               _|  _|  DELTA
      |_  |_|  COMB    |   |_   PERM    |        %       |_| |    %
       _                                 _   _
      |_| |_|           _   _|          |_| |_            _|  _
      |   |    PV      | | |_|  ND        | |_   QE      |_|.|    DEG.RAD

    Trigonometrics/Hyperbolics (cursor keys 4 and 1)
       _                _                                 _
      |_               |                |_               |_
       _|      SIN     |_       COS     |_       TAN      _|.     ASIN
       _                                     _                _
      |                |_               |_  |_   HYP     |_  |    HYP
      |_ .     ACOS    |_ .     ATAN    | |  _|  SIN     | | |_   COS
                            _                _
      |_  |_   HYP     |_  |_   AHYP    |_  |    AHYP    |_  |_   AHYP
      | | |_   TAN     | |  _|. SIN     | | |_ . COS     | | |_   TAN

    Statistics. Linear Regression
       _                _   _            _   _
      |_   _           |_  |_|          |_  |_
       _| |_   SUMCLR   _| | |  SUM+     _|  _|  SUM-
       _
       _               |    _
      |_|      STAT    |_  |    L.R.

    Complex, Base
       _                _   _                _                _
      |                |   |_|  COMPLEX |_  |_|          |_  |_
      |_       COMPLEX |_       VIEW    |_| | |  BASE    | | |_   HEX

    Function
       _                _   _            _
      |_   _           |_  |_   FN      |_  |    FN
      |   | |  FN      |    _|  SOLVE   |   |    INT

    Logics
       _                                 _
               BINARY   _  |_   LOGIC   |_|  _   LOGIC    _   _   LOGIC
               NOT     | | |_   NOT     | | | |  AND     |_| |    OR

    System
                            _                _
       _|  _               |_            _  |_           |_   _   OFF
      |_| |_   DICT    |_|  _|  USER    |_| |    OFF     |_  |_|  TIME
                                                          _   _
      |_  |_           |        LIT     |    _|  LIT     |   |_
      |_| |_   BATT    |_  |_|  UP      |_  |_|  DOWN    |_  | |  CLOCK

    Stack (cursor keys 5 and 2)
                                                          _
       _  |    CLEAR    _|               _|  _           |_  | |
      |_  |_   ALL     |_| |_|  DUP     |_| |    DROP     _|  _   SWAP
                                         _                _   _
       _   _            _  |_|          |_|  _           |_| |_   PRG
      |   |_|  ROT     |_| |    OVER    |   |_   PICK    |   |    FREE

    Programming
       _                _
      |_|  _            _|      @             |  !            _|
      |   |    PRG     |_|      POKE           . PEEK             EXE
           _            _   _
      |_  |_|  ADD     |_| |_
      |_  | |  TIME    |    _|  PAUSE
       _                    _                _                _
      |_  |_|  GET     |_  |    TEXT    |_  |_|  TEXT    |_  |_   TEXT
      | |  _|  KEY     |_  |_   CLEAR   |_  |    PRINT   |_   _|  SHOW

               LESS     _                _       NOT              GREATER
      |_       THAN     _       EQUAL    _ .     EQUAL    _|      THAN
           _                _            _
       _   _|  IS          |_           |_  |            |_  |_
      |   |    REAL?     | |    IF      |_  |_   ELSE    |_  | |  THEN
           _
      |_  |_                _           |_   _
      |_| |_   BEGIN   |_| | |  UNTIL   |_| |    BREAK

    Conversions
                                         _                    _
       _  |_           |_   _           |_  |            |   |
          | |  HMS->H  | |      H->HMS  | |.|_   KG-LBS  |_ .|_|  L-GAL
       _   _            _   _            _   _
      |   |_           |_                   |_            _
      |_ .|    C-F     | |.| |  KM-MI   | |.|    M-FT    |_ .|    CM-IN


  ____________________

   7-SEGMENT DISPLAY
  ____________________

     --a--    hgfe-dcba 8421-8421
    |     |
    f     b   Example code to display "3": abcdg = 0400-8421 = 0x4f = 79
    |--g--|
    e     c
    |     |
     --d--  (h)



    DEC       0   1   2   3   4   5   6   7   8   9  10  11  12  13  14  15
        HEX   0   1   2   3   4   5   6   7   8   9   a   b   c   d   e   f
                  _       _       _       _       _       _       _       _
                       |   |           |   |           |   |           |   |
    000 00                     |   |   |   |  _   _   _   _   _|  _|  _|  _|
                  _       _       _       _       _       _       _       _
                       |   |           |   |           |   |           |   |
    016 10   |   |   |   |   | | | | | | | | |_  |_  |_  |_  |_| |_| |_| |_|
                  _       _       _       _       _       _       _       _
             |   |   | | | | |   |   | | | | |   |   | | | | |   |   | | | |
    032 20                     |   |   |   |  _   _   _   _   _|  _|  _|  _|
                  _       _       _       _       _       _       _       _
             |   |   | | | | |   |   | | | | |   |   | | | | |   |   | | | |
    048 30   |   |   |   |   | | | | | | | | |_  |_  |_  |_  |_| |_| |_| |_|
                  _       _       _       _       _       _       _       _
              _   _   _|  _|  _   _   _|  _|  _   _   _|  _|  _   _   _|  _|
    064 40                     |   |   |   |  _   _   _   _   _|  _|  _|  _|
                  _       _       _       _       _       _       _       _
              _   _   _|  _|  _   _   _|  _|  _   _   _|  _|  _   _   _|  _|
    080 50   |   |   |   |   | | | | | | | | |_  |_  |_  |_  |_| |_| |_| |_|
                  _       _       _       _       _       _               _
             |_  |_  |_| |_| |_  |_  |_| |_| |_  |_  |_| |_| |_  |_  |_| |_|
    096 60                     |   |   |   |  _   _   _   _   _|  _|  _|  _|
                  _       _       _       _       _       _       _       _
             |_  |_  |_| |_| |_  |_  |_| |_| |_  |_  |_| |_| |_  |_  |_| |_|
    112 70   |   |   |   |   | | | | | | | | |_  |_  |_  |_  |_| |_| |_| |_|


  ____________________

   CIRCUIT DIAGRAM
  ____________________

              _______/ _______
             |  OnOff-Switch  |
             |   __________   |                         _________________
             |  |          |  |        _USB_           | 8.8.8.8.8.8.8.8.|
        +----+--|BATT+  IN+|--+---VCC-|     |-GND--GND-|GND              |
       _|_      |          |          |    3|----------|DIO   *   *   *  |
        =       |          |          |    6|----------|CLK   *   *   *  |
        +-------|BATT-  IN-|------GND-|    9|----------|STB   *   *   *  |
                |__________|          |_____|-VCC--VCC-|VCC   *   *   *  |
      LIPO      LIPO Battery          Arduino          |_________________|
      Battery   Charger              Pro Micro             QYF-TM1638
                (TP4056)



*/

// INCLUDES
#include <EEPROM.h>
#include <avr/power.h> // Needed for power management
#include <avr/sleep.h> // Needed for sleeping

// PINS
#define STROBE 9 // Strobe pin // Arduino/Genuino Micro
//#define STROBE 11 // Strobe pin // Mini
#define CLOCK  6 // Clock pin
#define DATA   3 // Data pin

// COMMANDS (QYF-TM1638-board)
#define PRINTCMD 0x40 // Command code for display printing
#define READCMD  0x42 // Command code for key reading

// POWER SAVING
static byte eachframemillis, thisframestart, lastframedurationms; // Framing times
static boolean justrendered; // True if frame was just rendered
static void idle(void) { // Idle, while waiting for next frame
  SMCR = _BV(SE); // Select idle mode and enable sleeping
  sleep_cpu();
  SMCR = 0; // Disable sleeping
}
static bool nextFrame(void) { // Wait (idle) for next frame
  byte now = (byte) millis(), framedurationms = now - thisframestart;
  if (justrendered) {
    lastframedurationms = framedurationms;
    justrendered = false;
    return false;
  }
  else if (framedurationms < eachframemillis) {
    if (++framedurationms < eachframemillis) idle();
    return false;
  }
  justrendered = true;
  thisframestart = now;
  return true;
}

// CONSTANTS
#define DIGITS         8 // Number of digits
#define SEGMENTS       8 // Number of segments per digit
#define KEYS          16 // Number of keys
#define FRAMERATE     15 // Maximal number of screen refreshes per second (>3)
#define EEBRIGHTNESS   0 // EEPROM address for brightness
#define END          255 // Escape key
#define DARKTIMEMIN    2 // Minimal time for auto display off in 10 x s

// KEY VALUES
#define _KF 15    //  key number:   getkey():     keycast[]:    symbol:
#define _K7 7     //   1  2  3  4    1  3  5  7   15  7  8  9   F  7  8  9
#define _K8 8     //   5  6  7  8    9 11 13 15   14  4  5  6   E  4  5  6
#define _K9 9     //   9 10 11 12   16  2  4  6   13  1  2  3   N  1  2  3
#define _KE 14    //  13 14 15 16    8 10 12 14   12  0 10 11   X  0  D  P
#define _K4 4
#define _K5 5
#define _K6 6
#define _KN 13
#define _K1 1
#define _K2 2
#define _K3 3
#define _KX 12
#define _K0 0
#define _KD 10
#define _KP 11

// SIGN
#define _70 0x3f //  --a--    hgfe-dcba 8421-8421
#define _71 0x06 // |     |
#define _72 0x5b // f     b
#define _73 0x4f // |--g--|
#define _74 0x66 // e     c
#define _75 0x6d // |     |
#define _76 0x7d //  --d--  (h)
#define _77 0x07
#define _78 0x7f
#define _79 0x6f
#define _7Q 0x3b
#define _7A 0x77
#define _7b 0x7c
#define _7C 0x39
#define _7c 0x58
#define _7d 0x5e
#define _7E 0x79
#define _7F 0x71
#define _7G 0x3d
#define _7h 0x74
#define _7H 0x76
#define _7i 0x04
#define _7I 0x30
#define _7J 0x0e
#define _7j 0x0d
#define _7K 0x75
#define _7L 0x38
#define _7N 0x37
#define _7n 0x54
#define _7M 0x15
#define _7o 0x5c
#define _7P 0x73
#define _7Q 0x3b
#define _7q 0x67
#define _7R 0x31
#define _7r 0x50
#define _7S 0x6d
#define _7t 0x78
#define _7u 0x1c
#define _7U 0x3e
#define _7V 0x72
#define _7W 0x2a
#define _7y 0x6e
#define _7ABSOLUTE 0x36
#define _7APOSTROPHELEFT 0x20
#define _7APOSTROPHERIGHT 0x02
#define _7AT 0x5f
#define _7COMMALEFT 0x10
#define _7COMMARIGHT 0x04
#define _7Cabs 0x14
#define _7Cre 0x21 // Imaginary indicators
#define _7Cim 0x02
#define _7Cangle 0x18
#define _7DEGREE 0x63
#define _7DOT 0x80
#define _7DOWN 0x0c
#define _7ENTER 0x42
#define _7EQUAL 0x48
#define _7GAMMA 0x31
#define _7GRAD 0x63
#define _7GREATERTHAN 0x0c
#define _7HIGHEQUALSIGN 0x41
#define _7HIGHLINE 0x01
#define _7LEFTBRACKET 0x39
#define _7LESSTHAN 0x18
#define _7LOWLINE 0x08
#define _7LOWHIGHLINE 0x09
#define _7MATA 0x22
#define _7MATB 0x14
#define _7MINUS 0x40
#define _7QUESTIONMARK 0x53
#define _7QUOTATIONMARK 0x22
#define _7PARALLEL 0x36
#define _7RECORD 0x6b
#define _7REVERSESOLIDUS 0x64
#define _7RIGHTBRACKET 0x0f
#define _7SOLIDUS 0x52
#define _7SPACE 0x00
#define _7SQRT 0x33
#define _7STATISTIC 0x5d
#define _7SWAP 0x1b
#define _7TEN 0x6b
#define _7THREELINES 0x49
#define _7UP 0x03

static const byte digit[] = // Digits
{ _70, _71, _72, _73, _74, _75, _76, _77, _78, _79, _7A, _7b, _7c, _7d, _7E, _7F};

#define MAXCMDI  76 // Number of commands of intrinsic functions
#define MAXCMDB 120 //120 End of builtin commands
#define MAXCMDU 160 //160 End of user commands

static byte msgnr = 0; // MESSAGES
#define MSGASK   0
#define MSGERROR 1
const char m0[] PROGMEM = {_7QUESTIONMARK, NULL};
const char m1[] PROGMEM = {_7E, _7r, _7r, _7o, _7r, NULL};
const char* const msg[] PROGMEM = {m0, m1};

// Command code defines
#define _0 0 // Intrinsic commands
#define _1 1
#define _2 2
#define _3 3
#define _4 4
#define _5 5
#define _6 6
#define _7 7
#define _8 8
#define _9 9
#define _DOT 10
#define _DUP 11
#define _DROP 12
#define _NEG 13
#define _E 14
#define _CMPLX 16
#define _RCL 17
#define _SUB 19
#define _MULT 22
#define _DIV 25
#define _ADD 27
#define _SWAP 30
#define _STO 33
#define _BASE 37
#define _PICK 45
#define _ROT 46
#define _LT 48
#define _EQ 49
#define _NE 50
#define _GT 51
#define _NAND 52
#define _ADDDUR 53
#define _PAUSE 54
#define _INT 55
#define _BEGIN 58
#define _UNTIL 59
#define _IF 61
#define _ELSE 62
#define _THEN 63
#define _ISREAL 68
#define _PI 71
#define _INV 72
#define _SIN 73
#define _EXP 74
#define _LN 75
#define _MOD MAXCMDI+0 // Builtin commands (mem)
#define _HEX MAXCMDI+1
#define _AND MAXCMDI+2
#define _NOT MAXCMDI+3
#define _OR MAXCMDI+4
#define _OVER MAXCMDI+5
#define _ABS MAXCMDI+6
#define _SQRT MAXCMDI+7
#define _COS MAXCMDI+8
#define _TAN MAXCMDI+9
#define _POW MAXCMDI+10
#define _PWR10 MAXCMDI+11
#define _LOG MAXCMDI+12
#define _ASIN MAXCMDI+13
#define _ACOS MAXCMDI+14
#define _ATAN MAXCMDI+15
#define _SINH MAXCMDI+16
#define _COSH MAXCMDI+17
#define _TANH MAXCMDI+18
#define _ASINH MAXCMDI+19
#define _ACOSH MAXCMDI+20
#define _ATANH MAXCMDI+21
#define _GAMMALN MAXCMDI+22
#define _HMS2H MAXCMDI+23
#define _H2HMS MAXCMDI+24
#define _PV MAXCMDI+25
#define _ND MAXCMDI+26
#define _QE MAXCMDI+27
#define _CLOCK MAXCMDI+28
#define _CLRSUM MAXCMDI+29
#define _SUM MAXCMDI+30
#define _SUMSUB MAXCMDI+31
#define _STAT MAXCMDI+32
#define _LR MAXCMDI+33
#define _PERCENT MAXCMDI+34
#define _PERCENTDELTA MAXCMDI+35
#define _FRAC MAXCMDI+36
#define _DEGRAD MAXCMDI+37
#define _CF MAXCMDI+38
#define _KMMI MAXCMDI+39
#define _MFT MAXCMDI+40
#define _CMIN MAXCMDI+41
#define _KGLBS MAXCMDI+42
#define _LGAL MAXCMDI+43
#define _PREEND 254 // Function delimiter
#define _END 255 // Function delimiter

// Builtin functions (mem)
const byte mem[] PROGMEM = {
  _END, // Necessary to prevent function starting with mp = 0
  //_1, _0, _BASE, _END, //0 BUSINESS
  _SWAP, _DUP, _ROT, _DUP, _ROT, _SWAP, _DIV, _INT, _MULT, _SUB, _END, //0 MODULO
  _1, _6, _BASE, _END, //1 HEX
  _NAND, _DUP, _NAND, _END, //2 AND
  _DUP, _NAND, _END, //3 NOT
  _DUP, _NAND, _SWAP, _DUP, _NAND, _NAND, _END, //4 OR
  _SWAP, _DUP, _ROT, _ROT, _END, //5 OVER
  _DUP, _0, _LT, _IF, _NEG, _THEN, _END, //6 ABS
  _DUP, _0, _EQ, _IF, _DROP, _0, _ELSE, _LN, _2, _DIV, _EXP, _THEN, _END, //7 SQRT sqrt(z)=exp(2*ln(z))
  _ISREAL, _1, _EQ, _IF, _9, _0, _SWAP, _SUB, _ELSE, _PI, _2, _DIV, _ADD, _THEN, _SIN, _END, //8 COS cos(x)=sqrt(1-sin(z)*sin(z)) cos(x)=sin(90-x)
  _DUP, _SIN, _SWAP, _COS, _DIV, _END, //9 TAN tan(z)=sin(z)/cos(z)
  _SWAP, _LN, _MULT, _EXP, _END, //10 POW a^b=exp(b*ln(a))
  _1, _SWAP, _E, _END, //11 POW10
  _LN, _1, _0, _LN, _DIV, _END, //12 LOG log(z)=ln(z)/ln(10)

  _DUP, _DUP, _DUP, _MULT, _NEG, _1, _ADD, _SQRT, //13 ASIN: asin(z) = -i*ln(i*z+sqrt(1-z*z))
  _SWAP, _0, _DUP, _1, _CMPLX, _MULT, _ADD, _LN, _0, _DUP, _1, _CMPLX, _NEG, _MULT,
  _SWAP, _ISREAL, _1, _EQ, _IF, _DROP, _1, _8, _0, _MULT, _PI, _DIV, // *RAD - if input was real
  _ISREAL, _1, _EQ, _IF, _ELSE, _CMPLX, _DROP, _THEN, // if result is imaginary - set im to 0
  _ELSE, _DROP, _THEN, _END,

  _DUP, _ASIN, _NEG, //14 ACOS acos(z)=PI/2-asin(z)
  _SWAP, _ISREAL, _1, _EQ, _IF, _DROP, _9, _0, _ADD, _ELSE, // +90 - if input was real
  _DROP, _PI, _2, _DIV, _ADD, _THEN, _END, // +PI/2 - if input was complex

  _DUP, _DUP, _MULT, _1, _ADD, _SQRT, _INV, _MULT, _ASIN, _END, //15 ATAN: atan(z)=asin(z/(sqrt(1+z*z)))

  _EXP, _DUP, _INV, _NEG, _ADD, _2, _DIV, _END, //16 SINH sinh(z)=(exp(z)-exp(-z))/2
  _EXP, _DUP, _INV, _ADD, _2, _DIV, _END, //17 COSH cosh(z)=(exp(z)+exp(-z))/2
  _DUP, _SINH, _SWAP, _COSH, _DIV, _END, //18 TANH tanh(z)=sinh(z)/cosh(z)
  _DUP, _DUP, _MULT, _1, _ADD, _SQRT, _ADD, _LN, _END, //19 ASINH asinh(x)=ln(x+sqrt(x*x+1))
  _DUP, _DUP, _MULT, _1, _SUB, _SQRT, _ADD, _LN, _END, //20 ACOSH acosh(z)=ln(z+sqrt(z*z-1))
  _DUP, _1, _ADD, _SWAP, _NEG, _1, _ADD, _DIV, _SQRT, _LN, _END, //21 ATANH atanh(z)=ln(sqrt((1+z)/(1-z)))

  _1, _ADD, _DUP, _DUP, _DUP, _DUP, _1, _2, _MULT, //22 GAMMALN: ln!=(ln(2*PI)-ln(z))/2+z*(ln(z+1/(12*z-1/10/z))-1)
  _SWAP, _1, _0, _MULT, _INV, _SUB, _INV, _ADD, _LN, _1, _SUB, _MULT,
  _SWAP, _LN, _NEG, _2, _PI, _MULT, _LN, _ADD, _2, _DIV, _ADD, _END,

  _DOT, _0, _0, _0, _0, _0, _1, _ADD,//23 HMS2H - round up to prevent leaps
  _DUP, _DUP, _INT, _SWAP, _OVER, _SUB, _1, _0, _0, _MULT, _INT, // hh mm
  _ROT, _3, _PICK, _SUB, _1, _0, _0, _MULT, _OVER, _SUB, _1, _0, _0, _MULT, // ss
  _3, _6, _0, _0, _DIV, _SWAP, _6, _0, _DIV, _ADD, _ADD, _END,// ->s ->h

  _DUP, _3, _6, _0, _0, _MULT, _DUP, _ROT, _INT, //24 H2HMS - h->s
  _SWAP, _OVER, _3, _6, _0, _0, _MULT, _SUB, _6, _0, _DIV, _INT, // hh mm
  _ROT, _OVER, _6, _0, _MULT, _SUB, _3, _PICK, _3, _6, _0, _0, _MULT, _SUB, // ss
  _1, _0, _0, _0, _0, _DIV, _SWAP, _1, _0, _0, _DIV, _ADD, _ADD, _END, // hh.mmss

  _OVER, _1, _ADD, _SWAP, _POW, _DUP, _1, _SUB, _SWAP, _DIV, _SWAP, _DIV, _END, //25 PV PV(i,n)=((1+i)^n-1)/(1+i)^n/i

  _DUP, _DUP, _DUP, _DUP, _MULT, _MULT, _DOT, _0, _7, _MULT, //26 ND
  _SWAP, _1, _DOT, _6, _MULT, _NEG, _ADD, _EXP, _1, _ADD, _INV, _SWAP,
  _DUP, _MULT, _NEG, _2, _DIV, _EXP, _2, _PI, _MULT, _SQRT, _INV, _MULT, _END,

  _OVER, _2, _DIV, _DUP, _MULT, _SWAP, _SUB, _SQRT, _SWAP, _2, _DIV, _NEG, _SWAP, //27 QE x12=-p/2+-sqrt(p*p/4-q)
  _OVER, _OVER, _SUB, _ROT, _ROT, _ADD, _END,

  _DUP, _ADDDUR, _DROP, //28 CLOCK - Reset duration
  _BEGIN, _HMS2H, _3, _6, _0, _0, _0, _0, _0, _MULT, _ADDDUR, // hms to ms and add duration
  _1, _SUB, // Time correction (in ms)
  _3, _6, _0, _0, _0, _0, _0, _DIV, _H2HMS, // Convert ms to hms
  _DUP, _1, _0, _0, _0, _0, _MULT, _INT, _1, _0, _0, _DIV, // Display hm.s
  //_DUP, _1, _0, _0, _0, _0, _MULT, _INT, _1, _0, _0, _0, _0, _DIV, // Display h.ms
  //_DUP, _1, _0, _0, _MULT, _INT, _1, _0, _0, _DIV, // Display h.m
  //_DUP, _1, _0, _0, _MULT, _INT, // Display hm.
  _2, _PAUSE, _DROP, _0, _UNTIL, _END, // Print and delete display copies

  _0, _DUP, _DUP, _DUP, _DUP, _DUP, //29 CLRSUM
  _5, _STO, _6, _STO, _7, _STO, _8, _STO, _9, _STO, _END,

  _7, _RCL, _1, _ADD, _7, _STO, //30 SUM+ - n
  _DUP, _8, _RCL, _ADD, _8, _STO, // X
  _DUP, _DUP, _MULT, _5, _RCL, _ADD, _5, _STO, // XX
  _OVER, _MULT, _6, _RCL, _ADD, _6, _STO, // XY
  _9, _RCL, _ADD, _9, _STO, _7, _RCL, _END, // Y push(n)

  _7, _RCL, _1, _SUB, _7, _STO, //30 SUM- - n
  _DUP, _8, _RCL, _SWAP, _SUB, _8, _STO, // X
  _DUP, _DUP, _MULT, _5, _RCL, _SWAP, _SUB, _5, _STO, // XX
  _OVER, _MULT, _6, _RCL, _SWAP, _SUB, _6, _STO, // XY
  _9, _RCL, _SWAP, _SUB, _9, _STO, _7, _RCL, _END, // Y

  _8, _RCL, _7, _RCL, _DIV, //32 STAT - mean (X/n)
  _DUP, _DUP, _MULT, _7, _RCL, _MULT, _NEG, _5, _RCL, _ADD, // stddev (XX-n*m^2)/(n-1)
  _7, _RCL, _1, _SUB, _DIV, _SQRT, _SWAP, _END,

  _6, _RCL, _7, _RCL, _MULT, _8, _RCL, _9, _RCL, _MULT, _SUB, //33 L.R. - a
  _5, _RCL, _7, _RCL, _MULT, _8, _RCL, _DUP, _MULT, _SUB, _DIV,
  _DUP, _8, _RCL, _MULT, _NEG, _9, _RCL, _ADD, _7, _RCL, _DIV, _SWAP, _END, // b

  _OVER, _DIV, _1, _0, _0, _MULT, _END, //34 % x/B*100%
  _OVER, _SUB, _OVER, _DIV, _1, _0, _0, _MULT, _END, //35 DELTA% (x-B)/B*100%
  _DUP, _INT, _SUB, _END, //36 FRAC

  _DUP, _PI, _MULT, _1, _8, _0, _DIV, //37 DEG<>RAD
  _SWAP, _1, _8, _0, _MULT, _PI, _DIV, _END,

  _DUP, _1, _DOT, _8, _MULT, _3, _2, _ADD, //38 C<>F
  _SWAP, _3, _2, _SUB, _1, _DOT, _8, _DIV, _END,

  _DUP, _1, _DOT, _6, _0, _9, _3, _4, _4, _DUP, _DUP, //39 KM<>MI
  _ROT, _SWAP, _DIV, _ROT, _ROT, _MULT, _END,

  _DUP, _3, _DOT, _3, _7, _0, _0, _7, _9, _DUP, _DUP, //40 M<>FT
  _ROT, _MULT, _ROT, _ROT, _DIV, _END,


  _DUP, _2, _DOT, _5, _4, _DUP, _DUP, _ROT, _SWAP, _DIV, _ROT, _ROT, _MULT, _END, //41 CM<>IN

  _DUP, _2, _DOT, _2, _0, _4, _6, _2, _3, _DUP, _DUP, //42 KG<>LBS
  //_ROT, _SWAP, _DIV, _ROT, _ROT, _MULT, _END,
  _ROT, _MULT, _ROT, _ROT, _DIV, _END,

  _DUP, _3, _DOT, _7, _8, _5, _4, _1, _2, _DUP, _DUP, //43 L<>GAL
  _ROT, _SWAP, _DIV, _ROT, _ROT, _MULT, _END,

};

// Command code defines
#define _0 0 // Intrinsic commands
#define _1 1
#define _2 2
#define _3 3
#define _4 4
#define _5 5
#define _6 6
#define _7 7
#define _8 8
#define _9 9

#define ALMOSTZERO   1e-37 // Limits to decide if sci or fix
#define OVERFLOW        36 // Max power of 10 before overflow
#define OVERFLOWEXP     87 // Max power of e before overflow
#define FIXMIN        1e-3 // Limits for fix display guarantee maximal
#define FIXMAX         1e7 //             number of significant digits
#define FIXSCI           3 // Display 3 decimals in scientific view
//#define FIXMANT          3 // FIX in 10th-exponent
#define ISF              1 // F-key demanded
#define ISG              2 // G-key demanded
#define RAD  ((180.0)/(PI)) // 180/PI
#define VIEWMAX 5
#define VIEWRE    1 // Number view mode (0=re 1=im 2=abs 3=angle)
#define VIEWIM    2
#define VIEWABS   3
#define VIEWANGLE 4 // Time in ms to display next animation step
#define PRGNAMEMAX       2 // Maximal number of characters of program name
#define PRGSIZEMAX     128 // Max size of prg, prgptr, prgbuf
#define FLONGPRESSTIME 350 // Time in ms when f longpress starts
#define DELTAX        1E-4 // Delta for solver
#define INTSTRIPES      10 // Number of Simpson stripes for integrating


// EEPROM addresses
#define EEDARKTIME 1 // EEPROM address for display off time
#define EEBASE     2 // Store base
#define MEMNR     10 // Maximal number of constant memories in EEPROM (slots)
#define EEMEM      3 // Starting EE-address for constants (2xMEMNRx4)
#define EEMEMB    83 // Starting EE-address for base number (8)
#define MENUITEMS 32 // Number of selectable user menu items
#define EEMENU    91 // Start of user menu in EEPROM (MENUITEMS bytes)
#define EEUSTART 123 // EEPROM address of user programs
#define EEUEND   EEPROM.length()
#define EEU (EEUEND-EEUSTART) // Available user memory

// GLOBAL VARIABLES
static byte dbuf[DIGITS]; // Display buffer
static byte brightness = 0; // Brightness

struct data { // Structure for numbers
  double r, i; // Real, imaginary
  int64_t b; // Integer, base
};
#define C0 (struct data){NULL, NULL,NULL} // Complex NULL

#define DATASTACKSIZE 26 // DATA STACK
struct data ds[DATASTACKSIZE];
static byte dp = 0;

#define ADDRSTACKSIZE 64 // ADDRESS STACK
static int as[ADDRSTACKSIZE];
static byte ap = 0;

byte cl = 0; // CONDITIONAL LEVEL
static int mp; // MEMPOINTER (builtin and user functions)
static int breakmp; // Holds memory pointer while break

static byte decimals = 0; // Number of decimals entered (input after decimal dot)
static boolean isdot = false; // True if dot was pressed and decimals will be entered
static byte dotpos = DIGITS; // Position of dot for printing (dbuf)
static boolean isnewnumber = true; // True if stack has to be lifted before entering a new number
static boolean isprintscreen = true; // True, if screen should be printed
static byte key = END; // Holds entered key
static byte oldkey = END; // Holds oldkey - to prevent keyrepeat
static byte fgm = 0, setfgm = 0; // F-key variables
static boolean isfloated = false; // True, if data stack was floated
static byte select; // Selected options
static boolean isdict = false; // True for dict browsing
static boolean ismenu = false; // True, if menu was demanded
static boolean issetusr = false; // True if dict is used for setting usr menu
static byte setusrselect; // Stores selected cmd value to store
static boolean ismenusetusr; // True for choosing user menu place

static boolean ispolar = false; // Shows imaginary number in polar format (else rectangular)
static byte view = 0; // Number view (1=re 2=im 3=abs 4=angle)

static boolean isprgdict = false, isprgmenu = false, isprgnew = false;
static byte prgbuflen; // Size of program in program buffer
static byte prgeditstart = 0;
static byte prgbuf[PRGSIZEMAX], oldprgbuflen; // Program buffer for fast editing
static boolean isprgeditfirst = false;
static boolean isprgedit = false; // True, if program is editet
static byte prgselect = 0; // Number of selected program
static boolean isprgselect = false; // Used for selecting program
static int prgaddr; // EEPROM address of actual program
static boolean isprgrename = false; // True if program should be renamed
static byte renamenr = 0; // Stores selected prg value to rename
static byte pause = 0; // Pause time in n * 250 ms

static long timestamp; // Timestamp for f longpress
static long durationtimestamp = millis(); // Timestamp for add-duration-function
static boolean freleased = false; // Used for releasing longpressed f-key
static long powertimestamp = 0; // Needed for timing of power manangement
static byte darktime; // Time of inactivity for dark screen (in 10 x s)
static boolean darkscreen = false; // Shows empty dbuf, if true

static int sou; // Size of user programs
static byte nou; // Number of user programs

static byte base = 0; // BASE
static boolean isAF; //Next keypress is hexA ... hexF
boolean isgetkey = false; // Needed for getkey function
static char alpha[DIGITS] = {NULL}; // String buffer (alpha) for user text output
static boolean isprintalpha = false; // Print alpha if true

static byte cycles; // Solver cycles
static double x0, x1, f0; // Old and new solve-value
static boolean issolve = false, isint = false; // True if solving or integrating is demanded
static double inta, intb, intdelta, intx, intf; // INT variables

static byte anipos = 1; // Position of animated bit (0 ... no animation)
static byte anicount = _END; // Number of animations (_END ... infinite animation)
#define _AEXIT 2 // Number of animations when exiting any mode

#define _MA 0 // Matrix A
#define _MB 1 // Matrix B
#define _MC 2 // Matrix C
static double m[6][3][3]; // Matrices A:0/1 B:2/3 C:4/5
boolean ismat = false, ismatview = false; // Matrix and view mode
static byte mselect = 0; // Selected matrix (A or B)
static double mdet, mdeti; // Determinant of matrix
byte matr = 0, matc = 0; // Row and col of selected matrix slot


// FUNCTION POINTER ARRAY
static void _nop(void) {} // NOP - no operation
static void _n0(void) { // 0
  _numinput(_0);
}
static void _n1(void) { // 1
  _numinput(_1);
}
static void _n2(void) { // 2
  _numinput(_2);
}
static void _n3(void) { // 3
  _numinput(_3);
}
static void _n4(void) { // 4
  _numinput(_4);
}
static void _n5(void) { // 5
  _numinput(_5);
}
static void _n6(void) { // 6
  _numinput(_6);
}
static void _n7(void) { // 7
  _numinput(_7);
}
static void _n8(void) { // 8
  _numinput(_8);
}
static void _n9(void) { // 9
  _numinput(_9);
}
static void _absolute(void) { // ABS
  seekmem(_ABS);
}
static void _acos(void) { // ACOS
  seekmem(_ACOS);
}
static void _acosh(void) { // ACOSH
  seekmem(_ACOSH);
}
static void _add(void) { // ADD + (a+i*b)(c+i*d)=(a+c)+i*(b+d)
  struct data b = dpop(), a = dpop();
  dpush({a.r + b.r, a.i + b.i, a.b + b.b});
}
static void _addduration(void) { // ADD DURATION
  dpushr(dpopr() + (millis() - durationtimestamp));
  durationtimestamp = millis();
}
static void _asin(void) { // ASIN
  seekmem(_ASIN);
}
static void _asinh(void) { // ASINH
  seekmem(_ASINH);
}
static void _atan(void) { // ATAN
  seekmem(_ATAN);
}
static void _atanh(void) { // ATANH
  seekmem(_ATANH);
}
static void _base(void) { // BASE MODE
  if (base) { // Return from base mode
    B2stack(); base = 0;
    anicount = _AEXIT;
  }
  else { // Switch to base mode
    base = dpopr();
    if (base > 1) stack2B(); else base = 0;
  }
  EEPROM[EEBASE] = base;
}
static void _batt(void) { // BATT
  dpushr((double)(1126400L / rawadc()) / 1000.0);
}
static void _begin(void) { // BEGIN
  apush(mp);
}
static void _break(void) { // BREAK
  breakmp = mp; mp = 0;
  isprintscreen = true;
}
static void _ce(void) { // CE
  if (isdot) {
    if (decimals)
      dpushr(((long)(dpopr() * pow10(--decimals)) / pow10(decimals)));
    else isdot = false;
  }
  else {
    if (base) {
      int64_t a = dpopb();
      a = a / base;
      if (!a) isnewnumber = true;
      else dpushb(a);
    }
    else {
      long a = dpopr() / 10.0;
      if (!a) isnewnumber = true;
      else dpushr(a);
    }
  }
}
static void _clock(void) { // CLOCK
  seekmem(_CLOCK);
}
static void _clr(void) { // CLR
  dp = 0;
  _textclear();
}
static void _comb(void) { // COMB nCr=n!/r!/(n-r)!=nPr/r!
  byte k = dpushr(dpopr());
  _perm();
  for (byte i = 1; i <= k; i++) dpushr(dpopr() / i);
}
static void _complex(void) { // COMPLEX
  if (!base) {
    if (isreal()) { // # Stack -> complex
      if (ispolar) { // Polar
        double b = dpopr(), a = dpopr();
        dpush({a * cos(b / RAD), a * sin(b / RAD), 0LL});
      }
      else { // Rectangular
        double b = dpopr();
        dpush({dpopr(), b, 0LL});
      }
    }
    else { // # Complex -> stack
      if (ispolar) { // Polar
        struct data a = dpop();
        dpushr(absolute(a.r, a.i)); dpushr(angle(a.r, a.i));
      }
      else { // Rectangular
        struct data a = dpop();
        dpushr(a.r); dpushr(a.i);
      }
    }
  }
}
static void _condelse(void) { // CONDITION ELSE
  if (!cl) msgnr = MSGERROR; // ELSE without corresponding IF
  else {
    _condseek(); // Seek next THEN
    cl--;
  }
}
static void _condeq(void) { // CONDITION =
  dpushr(dpopr() == dpopr());
}
static void _condgt(void) { // CONDITION >
  dpushr(dpopr() < dpopr());
}
static void _condif(void) { // CONDITION IF
  cl++; // Increment conditional level
  if (!dpopr()) _condseek(); // FALSE-Clause - seek next ELSE or THEN
}
static void _condlt(void) { // CONDITION <
  _condgt();
  dpushr(!dpopr());
}
static void _condne(void) { // CONDITION <>
  _condeq();
  dpushr(!dpopr());
}
static void _condseek(void) { // CONDITION - seek next ELSE or THEN
  boolean isloop = true;
  byte cltmp = 0; // Local conditional level
  while (isloop) {
    byte c = NULL;
    if (mp < sizeof(mem)) c = pgm_read_byte(mem + mp++); // Builtin
    else if (mp < sizeof(mem) + sou) c = EEPROM[mp++ -sizeof(mem) + EEUSTART]; // User
    if (mp >= sizeof(mem) + sou) { // No corresponding ELSE or THEN
      msgnr = MSGERROR;
      isloop = false;
    }
    else if (c == _IF) cltmp++; // Nested IF found
    else if (cltmp && c == _THEN) cltmp--; // Nested IF ended
    else if (!cltmp && (c == _ELSE || c == _THEN)) isloop = false;
  }
}
static void _condthen(void) { // CONDITION THEN
  if (!cl) msgnr = MSGERROR; // THEN without corresponding IF
  else cl--; // Decrement conditional level
}
static void _cvc2f(void) { // CONVERT C>F
  seekmem(_CF);
}
static void _cvcm2in(void) { // CONVERT CM>IN
  seekmem(_CMIN);
}
static void _cvdeg2rad(void) { // CONVERT DEG>RAD
  seekmem(_DEGRAD);
}
static void _cvkg2lbs(void) { // CONVERT KG<>LBS
  seekmem(_KGLBS);
}
static void _cvl2gal(void) { // CONVERT L<>GAL
  seekmem(_LGAL);
}
static void _cvkm2mi(void) { // CONVERT KM>MI
  seekmem(_KMMI);
}
static void _cvm2ft(void) { // CONVERT M>FT
  seekmem(_MFT);
}
static void _cos(void) { // COS
  seekmem(_COS);
}
static void _cosh(void) { // COSH
  seekmem(_COSH);
}
static void _cview(void) { // CVIEW
  if (dp) {
    view = VIEWRE;
  }
}
static void _dict(void) { // DICT
  if (!base) {
    select = 0;
    isdict = true;
  }
}
static void _div(void) { // DIV / a/b=a*inv(b)
  if (base) {
    int64_t b = dpopb();
    if (!b) msgnr = MSGERROR;
    else {
      int64_t m = 1;
      while (b % 10LL == 0) {
        b /= 10LL; m *= 10LL;
      }
      dpushb((dpopb() / b) / m);
    }
  }
  else {
    _inv(); _mul();
  }
}
static void _dot(void) { // DOT .
  if (isnewnumber) {
    dpush(C0); // Start new number with 0
    decimals = 0; isnewnumber = false;
  }
  if (base) isAF = true; // Enter A~F
  else isdot = true;
}
static void _drop(void) { // DROP, X
  if (isnewnumber && breakmp) { // Return from break and continue program
    mp = breakmp; breakmp = 0;
    delayshort(250); // To prevent interrupting the run with C key
  }
  else if (isnewnumber && dp) dp--; // Clear X
  if (!isnewnumber) _ce(); // Clear entry
}
static void _dup(void) { // DUP, #
  if (isnewnumber && dp) dpush(ds[dp - 1]);
}
static void _e(void) { // E
  if (base) _base();
  else {
    dpushr(pow10(dpopr()));
    _mul();
  }
}
static void _exe(void) { // EXE
  mp = dpopr() + sizeof(mem);
}
static void _exp(void) { // EXP exp(a+jb)=exp(a)*(cos(b)+i*sin(b))
  struct data a = dpop();
  if (a.r > OVERFLOWEXP) msgnr = MSGERROR;
  else {
    double tmp = texp(a.r);
    dpush({tmp * cos(a.i), tmp * sin(a.i), a.b});
  }
}
static void _fnfx(void) { // FN F(X)
  if (!base) execute(MAXCMDB);
}
static void _fnintegrate(void) { // FN INTEGRATE
  if (!base) {
    _swap(); inta = dpopr();
    intb = dpushr(dpopr());
    intdelta = (intb - inta) / 2 / INTSTRIPES;
    intf = 0.0;
    select = cycles = 0;
    anicount = _END;
    isint = true;
  }
}
static void _fnsolve(void) { // FN SOLVE
  if (!base) {
    x0 = dpushr(dpopr());
    select = cycles = 0;
    anicount = _END;
    issolve = true;
  }
}
static void _frac(void) { // FRAC
  seekmem(_FRAC);
}
static void _gammaln(void) { // GAMMALN
  seekmem(_GAMMALN);
}
static void _h2hms(void) { // H2HMS
  seekmem(_H2HMS);
}
static void _hex(void) { // HEX MODE
  seekmem(_HEX);
}
static void _hms2h(void) { // HMS2H
  seekmem(_HMS2H);
}
static void _integer(void) { // INTEGER
  dpushr((long)dpopr());
}
static void _inv(void) { // INV inv(a+jb)=a/(a*a+b*b)-i*b/(a*a+b*b)
  struct data a = dpop();
  if (absolute(a.r, a.i) == 0.0) msgnr = MSGERROR;
  dpush({1.0 / (a.r + a.i / a.r * a.i), -1.0 / (a.i + a.r / a.i * a.r), 0LL});
}
static void _isreal(void) { // ISREAL?
  dpushr(isreal());
}
static void _getkey(void) { // KEY
  isgetkey = true;
}
static void _keyf(void) { // KEY-F
  fgm = ISF;
  setfgm = 0;
}
static void _keyg(void) { // KEY-G
  fgm = ISG;
  setfgm = 0;
}
static void _keygoff(void) { // KEY-G OFF
  darkscreen = true;
}
static void _litdown(void) { // LIT-
  set_brightness(-1);
}
static void _litup(void) { // LIT+
  set_brightness(1);
}
static void _ln(void) { // LN ln(a+i*b)=ln(r*exp(i*f))=ln(r)+i*f r=|a+i*b| f=atan(b/a)
  struct data a = dpop();
  if (absolute(a.r, a.i) == 0.0) msgnr = MSGERROR;
  else dpush({log(absolute(a.r, a.i)), angle(a.r, a.i) / RAD, 0LL});
}
static void _log(void) { // LOG
  seekmem(_LOG);
}
static void _land(void) { // LOGIC AND
  seekmem(_AND);
}
static void _lnot(void) { // LOGIC NOT
  seekmem(_NOT);
}
static void _lor(void) { // LOGIC OR
  seekmem(_OR);
}
static void _mat(void) { // MATRIX
  ismat = true;
}
static void _mod(void) { // MODULO
  seekmem(_MOD);
}
static void _mrcl(void) { // MRCL
  _mstorcl(false);
}
static void _msto(void) { // MSTO
  _mstorcl(true);
}
static void _mstorcl(boolean issto) { // MSTORCL
  if (base)
    if (issto) EEPROM.put(EEMEMB, dpopb());
    else {
      int64_t a;
      EEPROM.get(EEMEMB, a);
      dpushb(a);
    }
  else {
    byte tmp = dpopr();
    if (tmp < MEMNR) {
      struct data a;
      if (issto) {
        a = dpop();
        EEPROM.put(EEMEM + tmp * sizeof(double), a.r);
        EEPROM.put(EEMEM + (tmp + MEMNR) * sizeof(double), a.i);
      }
      else {
        EEPROM.get(EEMEM + tmp * sizeof(double), a.r);
        EEPROM.get(EEMEM + (tmp + MEMNR) * sizeof(double), a.i);
        dpush(a);
      }
    }
  }
}
static void _mul(void) { // MULT * (a+i*b)*(c+i*d)=(a*c-b*d)+i*(b*c+a*d)
  struct data b = dpop(), a = dpop();
  int8_t ear = (int8_t)log10(_abs(a.r)), eai = (int8_t)log10(_abs(a.i));
  int8_t ebr = (int8_t)log10(_abs(b.r)), ebi = (int8_t)log10(_abs(b.i));
  int8_t ac = ear + ebr, bd = eai + ebi, bc = eai + ebr, ad = ear + ebi;
  if (ac > OVERFLOW || bd > OVERFLOW || bc > OVERFLOW || ad > OVERFLOW) msgnr = MSGERROR;
  else dpush({a.r * b.r - a.i * b.i, a.r * b.i + a.i * b.r, a.b * b.b});
}
static void _nand(void) { // NAND
  long b = dpopr();
  dpushr(~((long)dpopr() & b));
}
static void _nd(void) { // ND
  seekmem(_ND);
}
static void _neg(void) { // NEGATE
  struct data a = dpop();
  dpush({ -a.r, -a.i, -a.b});
}
static void _numinput(byte k) { // NUM Numeric input (0...9)
  if (isAF) k += 10;
  if (isdot) dpushr(dpopr() + k / pow10(++decimals)); // Append decimal
  else if (isnewnumber) dpush({k, 0.0, k}); // Push new numeral
  else if (base) dpushb(dpopb() * base + k); // Append numeral
  else dpushr(dpopr() * 10 + k); // Append numeral
  isnewnumber = isAF = false;
}
static void _offset(void) { // OFF Time Setting
  darktime = dpopr();
  limitdarktime();
  EEPROM[EEDARKTIME] = darktime;
}
static void _over(void) { // OVER
  seekmem(_OVER);
}
static void _pause(void) { // PAUSE
  isprintscreen = true;
  pause = dpopr();
}
static void _peek(void) { // @ PEEK (address peek)
  int addr = dpopr();
  if (addr < EEUEND - EEUSTART) dpushr(EEPROM[EEUSTART + addr]);
}
static void _percent(void) { // PERCENT
  seekmem(_PERCENT);
}
static void _percentdelta(void) { // PERCENTCHANGE
  seekmem(_PERCENTDELTA);
}
static void _perm(void) { // PERM nPr=n!/(n-r)!
  byte a = dpopr(), b = dpopr();
  a = b - a;
  dpushr(1.0);
  for (byte i = a; i < b; i++) dpushr(dpopr() * (i + 1));
}
static void _pi(void) { // PI
  dpushr(PI);
}
static void _pick(void) { // PICK
  byte n = dpopr();
  if (n >= 1 && n <= dp) dpush(ds[dp - n]);
}
static void _poke(void) { // ! POKE (value address poke)
  int addr = dpopr();
  if (addr < EEUEND - EEUSTART) EEPROM[EEUSTART + addr] = (byte)dpopr();
}
void _pow(void) { // POWER
  seekmem(_POW);
}
static void _prgfree(void) { // PRG FREE
  dpushr(EEU - sou);
}
static void _prgselect(void) { // PRG SELECT
  if (!base) {
    prgselect = 0;
    isprgselect = true;
  }
}
static void _pv(void) { // PV
  seekmem(_PV);
}
void _pwr10(void) { // POWER10
  seekmem(_PWR10);
}
static void _qe(void) { // QE
  seekmem(_QE);
}
static void _rot(void) { // ROT
  if (dp > 2) {
    struct data a = dpop(), b = dpop(), c = dpop();
    dpush(b); dpush(a); dpush(c);
  }
}
static void _sin(void) { // SIN sin(a+i*b)=sin(a)*cosh(b)+i*cos(a)*sinh(b)
  if (isreal()) dpushr(sin(dpopr() / RAD));
  else {
    struct data a = dpop();
    double e = texp(a.i);
    dpush({sin(a.r) * (e + 1.0 / e) / 2.0, sin(PI / 2 - a.r) * (e - 1.0 / e) / 2.0, 0LL});
  }
}
static void _sinh(void) { // SINH
  seekmem(_SINH);
}
static void _sqrt(void) { // SQRT
  seekmem(_SQRT);
}
static void _sub(void) { // SUB - a-b=a+(-b)
  _neg(); _add();
}
static void _sumadd(void) { // SUM+
  if (!base) seekmem(_SUM);
}
static void _sumclear(void) { // SUM CLEAR
  if (!base) seekmem(_CLRSUM);
}
static void _sumstat(void) { // SUM STAT
  seekmem(_STAT);
}
static void _sumsub(void) { // SUM-
  seekmem(_SUMSUB);
}
static void _sumlr(void) { // SUM L.R.
  seekmem(_LR);
}
static void _swap(void) { // SWAP
  if (dp > 1) {
    struct data a = dpop(), b = dpop();
    dpush(a); dpush(b);
  }
}
static void _tan(void) { // TAN
  seekmem(_TAN);
}
static void _tanh(void) { // TANH
  seekmem(_TANH);
}
static void _textclear(void) { // TEXT CLEAR
  alpha[0] = NULL;
}
static void _textput(void) { // TEXT PUT
  byte l = strlen(alpha);
  if (l < DIGITS) {
    alpha[l] = (byte)dpopr(); alpha[l + 1] = NULL;
  }
}
static void _textshow(void) { // TEXT SHOW
  isprintalpha = true;
}
static void _until(void) { // UNTIL
  if (!ap) msgnr = MSGERROR; // No BEGIN for this UNTIL
  else if (dpopr()) apop(); // Go on (delete return address)
  else apush(mp = apop()); // Go back to BEGIN
}
static void _usrset(void) { // USR
  if (!base) {
    select = 0; isdict = issetusr = true;
  }
}
const char* const comment[] PROGMEM = {
  _70, 0, _71, 0, _72, 0, _73, 0, // PRIMARY
  _74, 0, _75, 0, _76, 0, _77, 0,
  _78, 0, _79, 0, _7DOT, 0, _7d, _7u,
  _7d, _7r, _7MINUS, 0, _7E, _7E, _7F, 0,
  _7C, 0, _7r, _7c, _7F, _7S, _7S, _7b, // F
  _7d, _7c, _7F, _7n, _7M, _7U, _7S, _7A,
  _7P, _7r, _7d, _7COMMALEFT, _7C, _7GRAD, _7A, _7d,
  _7c, _7L, _7o, _7V, _7S, _7W, _7G, 0,
  _7b, _7t, _7S, _7t, _7F, _7I, _7L, _7d, // G
  _7u, _7S, _7b, _7A, _7t, _7S, _7S, _7c,

  _7P, _7F, _7M, _7d, _7C, _7K, _7L, _7u,
  _7t, _7o, _7P, _7c, _7r, _7o, _7o, _7F,
  _7LESSTHAN, 0, _7EQUAL, 0, _7EQUAL | _7DOT, 0, _7GREATERTHAN, 0, // 48
  _7HIGHLINE, 0, _7t, _7A, _7P, _7S, _7MINUS | _7DOT, 0,
  _7AT, 0, _7APOSTROPHERIGHT | _7DOT, 0, _7b, _7E, _7u, _7n,
  _7ENTER, 0, _7COMMARIGHT, _7F, _7E, _7L, _7t, _7h,
  _7b, _7r, _7K, _7y, _7t, _7C, _7t, _7P, // 64
  _7r, _7QUESTIONMARK, _7C, _70, _7P, _7E, _7P, _7I,
  _7I, 0, _7S, 0, _7E, 0, _7L, _7n,
  _7M, _7o, _7h, _7E, _7A, _7n, _7n, _7t, // 76

  _7o, _7r, _7o, _7V, _7ABSOLUTE, 0, _7SQRT, 0,
  _7C, 0, _7t, 0, _7P, 0, _7P, _7TEN,
  _7L, _7G, _7S | _7DOT, 0, _7C | _7DOT, 0, _7t | _7DOT, 0, // 88
  _7h, _7S, _7h, _7C, _7h, _7t, _7h, _7S | _7DOT,
  _7h, _7C | _7DOT, _7h, _7t | _7DOT, _7L, _7GAMMA, _7MINUS, _7h,
  _7h, _7MINUS, _7P, _7V, _7n, _7d, _7q, _7E, // 100
  _7C, _7K, _7S, _7C, _7S, _7A, _7S, _7S,
  _7STATISTIC, 0, _7L, _7r, _7SOLIDUS, 0, _7d, _7SOLIDUS,
  _7DOT, _7MINUS, _7d | _7DOT, _7r, _7C | _7DOT, _7F, _7K | _7DOT, _7M, // 112
  _7M | _7DOT, _7F, _7c | _7DOT, _7COMMALEFT, _7K | _7DOT, _7L, _7L | _7DOT, _7G,
};
#define GETKEY_CODE 65 // Dispatch code of key() - needed for program use
#define _NOP 10 // Blank function for nop comments
const byte commentsort[MAXCMDB] PROGMEM = {
  // EN - NUM, MATH:
  0, 1, 2, 3, // 0 1 2 3
  4, 5, 6, 7, // 4 5 6 7
  8, 9, 10, 13, // 8 9 . -
  14, 71, 17, 33, // E PI RCL STO
  27, 19, 22, 25, // + - * /
  76, 82, 55, 112, // MOD ABS INT FRAC
  74, 75, 87, 88, // EXP LN POW10 LOG
  83, 72, 86, 98, // SQRT 1/x POW GAMMA
  69, 70, 110, 111, // nCr nPr % d%
  101, 102, 103, 113, // PV ND QE deg.rad

  // 41 - TRIG/HYP STAT CMPLX FN LOGIC SYSTEM:
  73, 84, 85, 89, // SIN COS TAN ASIN
  90, 91, 92, 93, // ACOS ATAN SINH COSH
  94, 95, 96, 97, // TANH ASINH ACOSH ATANH
  39, 23, 107, _NOP, // SUMCLR SUMADD SUMSUB nop
  108, 109, _NOP, _NOP, // STAT LR nop nop
  16, 26, 37, 77, // CMPLX CMPLXVIEW BASE HEX
  21, 18, 34, _NOP, // FN FSOLVE FINT nop
  52, 79, 78, 80, // NAND NOT AND OR
  20, 36, 47, 44, // DICT USR DARK SETOFF
  32, 43, 35, 42, // BATT LIT+ LIT- CLOCK

  // 52 - STACK PRG CONVERT:
  28, 11, 12, 30, // CLR DUP DROP SWAP
  46, 29, 45, 40, // ROT OVER PICK PRGFREE
  24, 56, 57, 60, // PRG @ ! EXE
  53, 54, _NOP, _NOP, // T+ PAUSE nop nop
  65, 66, 67, 38, // GETKEY ALPHACLEAR ALPHAPUT ALPHASHOW
  48, 49, 50, 51, // < = != >
  68, 61, 62, 63, // REAL? IF ELSE THEN
  58, 59, 64, _NOP, // BEGIN UNTIL BREAK nop
  99, 100, 118, 119, // HMS>H H>HMS KG<>LBS L<>GAL
  114, 115, 116, 117 // C<>F KM<>MI m<>FT CM<>INCH
};
static void (*dispatch[])(void) = { // Function pointer array
  &_n0, &_n1, &_n2, &_n3, &_n4, &_n5, &_n6, &_n7, &_n8, &_n9, // Primary keys
  &_dot, &_dup, &_drop, &_neg, &_e, &_keyf, //10

  &_complex, &_mrcl, &_fnsolve, &_sub, // 0123  FKEY Offset = 16
  &_dict, &_fnfx, &_mul, &_sumadd, // 4567 20
  &_prgselect, &_div, &_cview, &_add, // 89dx 24
  &_clr, &_over, &_swap, &_keyg, // csef 28

  &_batt, &_msto, &_fnintegrate, &_litdown, // 0123  GKEY Offset = 32
  &_usrset, &_base, &_textshow, &_sumclear, // 4567 36
  &_prgfree, &_mat, &_clock, &_litup, // 89dx 40
  &_offset, &_pick, &_rot, &_keygoff, // csef 44

  &_condlt, &_condeq, &_condne, &_condgt, //48
  &_nand, &_addduration, &_pause, &_integer, //52
  &_peek, &_poke, &_begin, &_until, //56
  &_exe, &_condif, &_condelse, &_condthen, //60

  &_break, &_getkey, &_textclear, &_textput, //64
  &_isreal, &_comb, &_perm, &_pi, //68
  &_inv, &_sin, &_exp, &_ln, //72

  &_mod, &_hex, &_land, &_lnot, // 76 BUILTIN
  &_lor, &_over, &_absolute, &_sqrt, //80
  &_cos, &_tan, &_pow, &_pwr10, //84
  &_log, &_asin, &_acos, &_atan, //88
  &_sinh, &_cosh, &_tanh, &_asinh, //92
  &_acosh, &_atanh, &_gammaln, &_hms2h, //96
  &_h2hms, &_pv, &_nd, &_qe, //100
  &_clock, &_sumclear, &_sumadd, &_sumsub, //104
  &_sumstat, &_sumlr, &_percent, &_percentdelta, //108
  &_frac, &_cvdeg2rad, &_cvc2f, &_cvkm2mi, //112
  &_cvm2ft, &_cvcm2in, &_cvkg2lbs, &_cvl2gal, //116
};

// SUBROUTINES - SYSTEM
static void delayshort(byte ms) { // Delay (with timer) in ms with 8 bit duration
  long t = millis();
  while ((byte)(millis() - t) < ms) ;
}
static void delaylong(byte nr) { // Delay nr quarters of a second
  for (byte i = 0; i < nr; i++) delayshort(250);
}


// SUBROUTINES - BOARD
static void command(byte val) { // Send command to shield
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, val);
  digitalWrite(STROBE, HIGH);
}

//#define MX0 0
#define MX1 1
#define MX2 2
#define MX3 3
#define MX4 4

static int rawadc() { // Measure Vcc
  power_adc_enable();
  ADMUX = (_BV(REFS0) | _BV(MX4) | _BV(MX3) | _BV(MX2) | _BV(MX1)); // Set voltage bits
  delayshort(2); // Wait for ADMUX setting to settle
  ADCSRA |= _BV(ADSC); // Start conversion
  while (bit_is_set(ADCSRA, ADSC)); // Measuring
  power_adc_disable();
  return (ADC);
}


// SUBROUTINES - DISPLAY
static void setframerate(byte rate) { // Calculate frameduration
  eachframemillis = 1000 / rate;
}

static void set_brightness(int8_t add) {  // Set and save brightness
  brightness = (brightness + add) & 0x07; // Calculate brightness and limit
  command(0x88 | brightness); // Set brightness
  EEPROM[EEBRIGHTNESS] = brightness;
}

static void dbuffill(byte c) { // Fill display buffer
  for (int i = 0; i < DIGITS; i++) dbuf[i] = c;
}

static byte animate(void) { // Animation
  dbuf[0] = anipos;
  if ((anipos <<= 1) & 0x40) {
    anipos = 1;
    if (anicount < _END) anicount--;
    if (!anicount) {
      dbuf[0] = 0;
      isprintscreen = true;
    }
  }
  printbuf();
}

static void limitdarktime(void) { // Limit darktime to DARKTIMEMIN
  darktime = max(darktime, DARKTIMEMIN);
}


// SUBROUTINES - KEYBOARD
static unsigned int getbuttons(void) { // Get button code (only one key at a time) - 16 bits
  uint16_t buttons = 0;
  byte val = 0;
  digitalWrite(STROBE, LOW);
  shiftOut(DATA, CLOCK, LSBFIRST, READCMD); // Demand key input
  pinMode(DATA, INPUT); // Switch data line to INPUT
  for (byte i = 0; i < 4; i++) {
    val = shiftIn(DATA, CLOCK, LSBFIRST) >> 1; // Get button value (and half it)
    if (val > 2) val >>= 2; // Reduce values 16 and 32 to 4 and 8
    buttons |= val << (4 * i); // Write value-nibble to buttons-nibbles (4,3,2,1)
  }
  pinMode(DATA, OUTPUT); // Switch data line back to output
  digitalWrite(STROBE, HIGH);
  return (buttons);
}

static byte getkey(void) { // Returns key
  const byte keycast[KEYS] = {END, _KF, _K1, _K7, _K2, _K8, _K3, _K9, _KX, _KE, _K0, _K4, _KD, _K5, _KP, _K6};
  unsigned int c = getbuttons();
  byte ans = NULL;
  if (c & 0x0001) return (_KN);
  while (c >>= 1) ans++;
  return (keycast[ans]);
}


// SUBROUTINES - MATH
#define _ones(x) ((x)%10)        // Calculates ones unit
#define _tens(x) (((x)/10)%10)   // Calculates tens unit
#define _huns(x) (((x)/100)%10)  // Calculates hundreds unit
#define _tsds(x) (((x)/1000)%10) // Calculates thousands unit

static double _abs(double f) { // Absolute value
  return (f < 0.0 ? -f : f);
}

static double pow10(int8_t e) { // Calculates 10 raised to the power of e
  double f = 1.0;
  if (e > 0) while (e--) f *= 10.0;
  else while (e++) f /= 10.0;
  return (f);
}

static double texp(double f) { // Calculate exp with Taylor series
  double result = 1.0, frac = 1.0;
  for (byte i = 1; i < 255; i++) result += frac *= f / i;
  return (result);
}

static double absolute(double a, double b) { // Returns the absolute value of a complex number
  a = _abs(a); b = _abs(b);
  if (a == 0.0) return (b);
  else if (b == 0.0) return (a);
  return (texp(0.5 * (log(a) + log(a + b / a * b)))); // Prevent big arguments
}

static double angle(double a, double b) { // Returns the angle of a complex number
  double tmp = atan(b / a) * RAD;
  if (a == 0.0) return ((b < 0.0) ? -90.0 : 90.0); // Special case imaginary
  else if (a < 0.0) {
    return ((b < 0.0) ? (tmp - 180.0) : (tmp + 180.0)); // Quadrant 2 or quadrant 3
  }
  else return (tmp); // Quadrant 1 or 4
}


// SUBROUTINES - MATRIX
static void mpush(byte mat, byte row, byte col) { // Push matrix element to stack
  dpush({m[2 * mat][row][col], m[2 * mat + 1][row][col], 0LL});
}

static void mpop(byte mat, byte row, byte col) { // Pop stack to matrix element
  struct data a = dpop();
  m[2 * mat][row][col] = a.r; m[2 * mat + 1][row][col] = a.i;
}

static void mc2a(void) { // Copy matrix C to A
  memcpy(m[0], m[4], 2 * 3 * 3 * sizeof(double)); // A=C
}
static void ma2b(void) { // Copy matrix A to B
  memcpy(m[2], m[0], 2 * 3 * 3 * sizeof(double)); // B=A
}

static void maddsub(byte op) { // Add or substract matrices (0:add 1:sub)
  for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++) {
      mpush(_MA, row, col); mpush(_MB, row, col);
      if (op) _sub(); else _add();
      mpop(_MC, row, col);
    }
  mc2a(); // A=C
}

static void mmult(void) { // Multiply matrices A*B
  for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++) {
      dpushr(0.0); // Initial value
      for (byte i = 0; i < 3; i++) {
        mpush(_MA, row, i); mpush(_MB, i, col);
        _mul(); _add();
      }
      mpop(_MC, row, col);
    }
  mc2a(); // A=C
}

static void mswap(void) { // Swap matrices A and B
  memcpy(m[4], m[2], 2 * 3 * 3 * sizeof(double)); // C=B
  ma2b(); // B=A
  mc2a(); // A=C
}

static void mtrans(void) { // Transpose matrix
  for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++) {
      mpush(mselect ? _MB : _MA, row, col);
      mpop(_MC, col, row);
    }
  if (mselect) memcpy(m[2], m[4], 2 * 3 * 3 * sizeof(double)); // B=C
  else mc2a(); // A=C
}

static void mdeterminant(void) { // Calculate determinant of matrix A
  mdet = mdeti = 0.0;
  mdetrc(0, 1, 2, 0, 1, 2, 1);
  mdetrc(0, 1, 2, 1, 2, 0, 1);
  mdetrc(0, 1, 2, 2, 0, 1, 1);
  mdetrc(2, 1, 0, 0, 1, 2, -1);
  mdetrc(2, 1, 0, 1, 2, 0, -1);
  mdetrc(2, 1, 0, 2, 0, 1, -1);
}
static void mdetrc(byte r1, byte r2, byte r3, byte c1, byte c2, byte c3, int8_t sign) {
  mpush(_MA, r1, c1);  mpush(_MA, r2, c2); mpush(_MA, r3, c3);
  _mul(); _mul();
  struct data a = dpop();
  mdet += sign * a.r; mdeti += sign * a.i;
}

static void minv(void) { // Calculate the inverse of matrix A
  mdeterminant();
  if (mdet == 0.0 && mdeti == 0.0) msgnr = MSGERROR;
  else {
    for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++)
        mminor(row, col);
    mc2a(); // A=C
    mtrans();
  }
}
static void mminor(byte r, byte c) { // Minor determinant, cofactor (-1) and *1/det
  byte r1 = (r + 1) % 3, r2 = (r + 2) % 3, c1 = (c + 1) % 3, c2 = (c + 2) % 3;
  mpush(_MA, r1, c2); mpush(_MA, r2, c1); mpush(_MA, r1, c1); mpush(_MA, r2, c2);
  _mul(); _rot(); _rot(); _mul(); _sub(); // Determinant and cofactor
  dpush({mdet, mdeti, 0LL}); _div(); // * 1/det
  mpop(_MC, r, c);
}


// SUBROUTINES - STACK
static void floatstack() {
  memcpy(ds, &ds[1], (DATASTACKSIZE - 1) * sizeof(struct data));
  dp--;
  isfloated = true;
}

static void dpush(struct data c) { // Push complex number to data-stack
  if (dp >= DATASTACKSIZE) floatstack(); // Float stack
  ds[dp++] = c;
}
static double dpushr(double f) { // Push real number to data-stack
  dpush({f, NULL, NULL});
  return (f);
}
static void dpushb(int64_t n) { // Push integer number to data-stack
  dpush({NULL, NULL, n});
}

static struct data dpop(void) { // Pop value from data-stack
  return (dp ? ds[--dp] : C0);
}
static float dpopr(void) { // Pop real number from data-stack
  return (dp ? ds[--dp].r : NULL);
}
static int64_t dpopb(void) { // Pop base number from data-stack
  return (dp ? ds[--dp].b : NULL);
}

static void apush(int addr) { // Push address (int) to address-stack
  if (ap >= ADDRSTACKSIZE) msgnr = MSGERROR; else as[ap++] = addr;
}
static int apop(void) { // Pop address (int) from address-stack
  return (ap ? as[--ap] : NULL);
}

static void B2stack (void) { // Copy base stack to stack
  for (byte i = 0; i < dp; i++)  ds[i].r = ds[i].b;
}
static void stack2B (void) { // Copy stack to base stack
  for (byte i = 0; i < dp; i++)
    ds[i].b = (ds[i].r * 10LL + 5LL) / 10LL; // Includes rounding
}


// SUBROUTINES - APPLICATION

static boolean isreal(void) { // True if TOS is real
  return (dp ? ds[dp - 1].i == 0.0 : true);
}

static void inkey(void) { // Read keyboard key
  byte k;
  while ((k = getkey()) >= _END) ; // Wait for keypress
  dpushr(k);
  delayshort(200); // Prevent early keypress
  isgetkey = false;
  isprintscreen = true;
}

static void execute(byte command) { // Execute command
  if (command < MAXCMDB) (*dispatch[command])(); // Dispatch intrinsic/builtin command
  else if (command < MAXCMDU)
    mp = (seekusr(command) + PRGNAMEMAX) - EEUSTART + sizeof(mem); // Execute user command
  if (!isAF && command > 10 && command != 12) { // New number - except: 0-9.A-F CE
    decimals = 0; isdot = false; isnewnumber = true;
  }
  if (fgm && setfgm) fgm = setfgm = 0; // Hold demanded f-key status one cycle
  setfgm = 1;
}

static int seekusr(byte n) { // Find run-address (ptr) of n-th function
  int ptr = EEUSTART;
  while (n - MAXCMDB) if (EEPROM[ptr++] == _END) n--;
  return (ptr);
}
static int seekusrp(int8_t n) { // Seekusr for prgselect+MAXCMDB + n
  return (seekusr(prgselect + MAXCMDB + n));
}
static void seekmem(byte n) { // Find run-address (mp) of n-th builtin function
  mp = 0;
  while (n + 1 - MAXCMDI) if (pgm_read_byte(mem + mp++) == _END) n--;
}

static void moveprgup(void) { // Move program in EEPROM up
  int a1 = seekusrp(-1), a2 = seekusrp(0), a3 = seekusrp(1);
  int a = a1 + a3 - a2;
  for (byte i = 0; i < a2 - a1; i++) prgbuf[i] = EEPROM[a1 + i]; // Rescue area1
  for (int i = a1; i < a; i++) EEPROM[i] = EEPROM[i + a2 - a1]; // Move EEPROM area
  for (byte i = 0; i < a2 - a1; i++) EEPROM[a1 + a3 - a2 + i] = prgbuf[i]; // Replace temp
  byte cmd1 = MAXCMDB + prgselect - 1, cmd2 = MAXCMDB + prgselect; // Changed programs
  for (byte n = 0; n < nou; n++) // Repair program calls in EEPROM
    for (int i = seekusr(n + MAXCMDB) + PRGNAMEMAX; i < seekusr(n + MAXCMDB + 1); i++)
      if (EEPROM[i] == cmd1) EEPROM[i] = cmd2;
      else if (EEPROM[i] == cmd2) EEPROM[i] = cmd1;
  for (int i = EEMENU; i < EEMENU + MENUITEMS; i++) // Repair program calls in user menu
    if (EEPROM[i] == cmd1) EEPROM[i] = cmd2;
    else if (EEPROM[i] == cmd2) EEPROM[i] = cmd1;
}

static void prgstepins(byte c) { // Insert step (character c in prgbuf at prgeditstart)
  if (prgbuflen < PRGSIZEMAX) {
    for (byte i = prgbuflen; i > prgeditstart; i--) prgbuf[i] = prgbuf[i - 1];
    if (isprgeditfirst) prgbuf[prgeditstart] = c;
    else prgbuf[prgeditstart + 1] = c;
    prgbuflen++;
    prgeditstart++;
    fgm = 0;
  }
}

static void idofusr(void) { // Count size (sou) and number (nou) of user programs
  int ptr = EEUSTART;
  boolean loop = true;
  byte n = 0, tmp1 = EEPROM[ptr];
  if (tmp1 != _END || EEPROM[ptr + 1] != _END)
    while (loop) {
      byte tmp2 = EEPROM[++ptr];
      if (tmp1 == _END) {
        n++; if (tmp2 == _END) loop = false;
      }
      tmp1 = tmp2;
    }
  nou = n; sou = ptr + 1 - EEUSTART;
}

static void upn(byte n, byte l) { // Selection up l lines
  byte sel = select;
  if (select > n * l && select <= (n + 1) * l - 1) select--; else select = (n + 1) * l - 1;
}
static void downn(byte n, byte l) { // Selection down l lines
  if (select >= n * l && select < (n + 1) * l - 1) select++; else select = n * l;
}
static byte menuselect(byte lines) { // Selection (1 line = 16 items)
  char k = key;
  if (k == _KF || k >= _K7 && k <= _K9) { // Execute
    if (k == _KF) {
      k = _K6; // Cast f-key to key 6
      key = oldkey;
    }
    return (select * 4 + k - _K6);
  }
  else if (k == _KE) upn(0, lines); // # Up0
  else if (k == _KN) downn(0, lines); // # Down0
  else if (k == _K4) upn(1, lines); // # Up1
  else if (k == _K1) downn(1, lines); // # Down1
  else if (k == _K5) upn(2, lines); // # Up2
  else if (k == _K2) downn(2, lines); // # Down2
  else if (k == _K6) upn(3, lines); // # Up3
  else if (k == _K3) downn(3, lines); // # Down3
    else if (k == _K0) if (select > 0) select--; else select = 4 * lines - 1; // # Left
    else if (k == _KD) if (select < 4 * lines - 1) select++; else select = 0; // # Right
  else if (k == _KX) return (_END);
  return (_PREEND);
}


// SUBROUTINES - PRINTING
static void printmsg(byte nr) { // Print message
  strcpy_P(dbuf, (char*)pgm_read_word(msg + nr));
}

static void printindicator(void) { // Print indicators
  if (fgm == ISF) dbuf[0] |= _7HIGHLINE; // F-key
  else if (fgm == ISG) dbuf[0] |= _7LOWLINE; // G-key
  if (isfloated) { // Stack float (data loss)
    dbuf[0] |= _7COMMARIGHT | _7DOT;
    isfloated = false;
  }
  if (!isnewnumber) // Number input
    if (base) dbuf[7] |= _7DOT; // Base mode
    else dbuf[0] |= _7DOT; // Regular number input
  if (view) { // Complex view
    if (view == VIEWRE) dbuf[0] |= _7Cre; // re
    else if (view == VIEWIM) dbuf[0] |= _7Cim; // im
    else if (view == VIEWABS) dbuf[0] |= _7Cabs; // abs
    else dbuf[0] |= _7Cangle; // angle
  }
  else if (!isreal() && !ismenu && !isprgselect && !isprgedit) // Complex number mode
    if (ispolar) dbuf[0] |= _7COMMARIGHT; // Polar
    else dbuf[0] |= _7COMMALEFT; // Rectangular
  if (ismat || ismatview) { // Matrix
    if (mselect) dbuf[0] |= _7MATB; // B
    else dbuf[0] |= _7MATA; // A
    if (ismatview) dbuf[0] |= _7DOT; // Matrix view
  }
}

static void printbuf() { // Write buffer to display
  if (dotpos < DIGITS) dbuf[dotpos] |= _7DOT; // Dot for normal print
  if (!msgnr && !anicount) printindicator(); // Indicators
  if (darkscreen) dbuffill(NULL); // Dark screen

  command(PRINTCMD); // Demand write command
  for (byte j = 0; j < SEGMENTS; j++) { // Write segment of all digits
    byte digbuf = 0;
    for (byte i = 0; i < DIGITS; i++)
      digbuf = (digbuf << 1) | ((dbuf[i] & (0x01 << j)) ? 0x01 : 0x00);
    for (byte i = 0; i < DIGITS; i++) { // Print segments for each digit
      digitalWrite(STROBE, LOW);
      shiftOut(DATA, CLOCK, LSBFIRST, 0xc0 | 2 * j); // Address - segment
      shiftOut(DATA, CLOCK, LSBFIRST, 0x00 | digbuf); // Data - digit
      digitalWrite(STROBE, HIGH);
    }
  }
  dotpos = DIGITS;
}

static double printsign(double f) { // Common code of printfix and printsci
  dbuf[0] = NULL;
  if (f < 0.0) {
    f = - f; dbuf[0] = _7MINUS;
  }
  return (f);
}

static void printfix(double f) { // Print float with fixed decimals
  f = printsign(f);
  byte fix = 6; // Number of displayed digits
  int8_t e = (f >= 1.0) ? e = log10(f) : 0; // Exponent
  double d = f + 0.5 / (pow10(fix - e)); // Rounding
  long m = d; // Mantissa
  for (byte i = e + 1; i > 0; i--) {
    dbuf[i] = digit[_ones(m)];
    m /= 10L;
  }
  if ((long)f >= (long)d) d = f; // Calculate after dot (and suppress trailing zeros)
  m = (d - (long)d) * pow10(fix - e) + 0.5;
  boolean istrail = true;
  for (byte i = DIGITS; i > e + 2; i--) {
    byte one = _ones(m);
    if (!istrail || (isnewnumber || i - e - 2 <= decimals) && (!isnewnumber || one != 0)) {
      dbuf[i - 1] = digit[one]; // Assign digit
      istrail = false; // End of trailing zeros
    }
    m /= 10L;
  }
  dotpos = e + 1;
}

static void printsci(double f) { // Print float with mantissa height (mh) at line y
  f = printsign(f);
  long m; // Mantissa
  int8_t e = log10(f); // Exponent
  m = (f / pow10(e - FIXSCI)) + 0.5; // Mantissa
  if (m > 0 && m < pow10(FIXSCI)) m = (f / pow10(--e - FIXSCI)) + 0.5; // "New" mantissa
  for (byte i = (FIXSCI + 1); i > 0; i--) { // Print mantissa
    dbuf[i] = digit[_ones(m)];
    m /= 10;
  }
  if (e < 0) { // Negative exponent
    dbuf[5] = _7MINUS;
    e = -e;
  }
  dbuf[6] = (e >= 10) ? digit[_tens(e)] : _70; // Exponent
  dbuf[7] = digit[_ones(e)];
  dotpos = 1;
}

static void printnum(double f) { // Print number
  double a = _abs(f);
  if (dp || ismatview)
    if (a < ALMOSTZERO || a >= FIXMIN && a < FIXMAX) printfix(f); // Fixed format
    else printsci(f); // Sci format
  else {
    dbuffill(NULL); dbuf[1] = _7LOWLINE;
  }
}

static void printbase() { // Print TOS for base (10 or other)
  if (dp) {
    uint64_t m = ds[dp - 1].b;
    dbuf[DIGITS - 1] = _70;
    for (byte i = DIGITS; i > 0 && m > 0; i--) {
      dbuf[i - 1] = digit[m % base];
      m /= base;
    }
  }
  else dbuf[DIGITS - 1] = _7LOWLINE;
}

static boolean printscreen(void) { // Print screen
  dbuffill(NULL);

  if (!issolve && !isint) { // Do not print, if running

    if (msgnr) printmsg(msgnr); // Print message

    else if (isgetkey) printmsg(MSGASK); // Getkey

    else if (isprintalpha) // Print text
      for (byte i = 0; i < strlen(alpha); i++) dbuf[i] = alpha[i];

    else if (base) printbase(); // Print base

    else if (view) { // Print complex view
      if (view == VIEWRE) printnum(ds[dp - 1].r); // re
      else if (view == VIEWIM) printnum(ds[dp - 1].i); // im
      else if (view == VIEWABS) printnum(absolute(ds[dp - 1].r, ds[dp - 1].i)); // abs
      else printnum(angle(ds[dp - 1].r, ds[dp - 1].i)); // angle
    }

    else if (isdict) { // Dict
      int tmp = select * 4;
      dbuffill(NULL);
      if (tmp < MAXCMDB) // Intrinsic or builtin command
        for (byte i = 0; i < 4; i++) {
          byte cs = pgm_read_byte(&commentsort[tmp + i]);
          for (byte j = 0; j < 2; j++) dbuf[2 * i + j] = pgm_read_byte(&comment[2 * cs + j]);
        }
      else { // User command
        for (byte i = 0; i < 4; i++) {
          if (tmp + i < MAXCMDB + nou) {
            int ptr = seekusr(tmp + i);
            dbuf[2 * i] = EEPROM[ptr++]; dbuf[2 * i + 1] = EEPROM[ptr];
          }
          else dbuf[2 * i] = _7LOWLINE;
        }
      }
    }

    else if (ismenu) { // Menu
      byte tmp = select * 4;
      dbuffill(NULL);
      for (byte i = 0; i < 4; i++) //{
        if (EEPROM[EEMENU + tmp + i] < MAXCMDB) // Intrinsic or builtin command
          for (byte j = 0; j < 2; j++)
            dbuf[2 * i + j] = pgm_read_byte(&comment[2 * EEPROM[EEMENU + tmp + i] + j]);
        else { // User command
          int ptr = seekusr(EEPROM[EEMENU + tmp + i]);
          for (byte j = 0; j < 2; j++) dbuf[2 * i + j] = EEPROM[ptr + j];
        }
    }

    else if (isprgselect) { // Select program
      dbuf[1] = _7P; // PRG indicator
      byte nr = prgselect;
      dbuf[3] = digit[_tens(nr + 1)]; // Program number
      dbuf[4] = digit[_ones(nr + 1)];
      int addr = seekusr(MAXCMDB + prgselect); // Program name
      for (byte i = 0; i < PRGNAMEMAX; i++) dbuf[6 + i] = EEPROM[addr + i];
    }
    else if (isprgrename) // Rename program
      for (byte i = 0; i < 4; i++) dbuf[2 * i] = select * 4 + i;
    else if (isprgedit) { // Edit program
      byte pi = prgeditstart, cmdpi = prgbuf[pi];
      dbuf[1] = _7E; // Print edit indicator
      dbuf[3] = digit[_tens(pi + 1)]; // Print line number
      dbuf[4] = digit[_ones(pi + 1)];
      if (pi < prgbuflen) { // Print command
        if (cmdpi < MAXCMDB)
          for (byte i = 0; i < 2; i++) dbuf[6 + i] = pgm_read_byte(&comment[2 * cmdpi + i]);
        else {
          int ptr = seekusr(cmdpi);
          dbuf[6] = EEPROM[ptr++]; dbuf[7] = EEPROM[ptr];
        }
      }
    }

    else if (ismatview) printnum(mselect ? m[2][matr][matc] : m[0][matr][matc]); // Matrix

    else { // Print stack
      double a = 0.0;
      if (dp) a = ds[dp - 1].r;
      printnum(a);
    }

    printbuf();
  }
  return (NULL); // To determine isprintscreen
}


// Temporary user programs for testing
const byte umem[] PROGMEM =
{ _7F , _7n, _DUP, _MULT, _2, _SUB, _END,
  _7t , _7t, _1, _5, 67, 38, _2, _PAUSE, 65, /*120,*/ _1, _ADD, _END,// 67alphaput 38alphashow 65key?
  _72 , _7M, _2, _MULT, _END,
  _72 , _7d, _2, _DIV, _END,
  _7I , _7I, _DUP, _MULT, _SWAP, _DUP, _MULT, _ADD, _SQRT, _END,
  _7S | _7DOT , _7SPACE, _DUP, _DUP, _DUP, _MULT, _NEG, _1, _ADD, _SQRT, //13 ASIN asin(z) = -i*ln(i*z+sqrt(1-z*z))
  _SWAP, _0, _DUP, _1, _CMPLX, _MULT, _ADD, _LN, _0, _DUP, _1, _CMPLX, _NEG, _MULT,
  _SWAP, _ISREAL, _1, _EQ, _IF, _DROP, _1, _8, _0, _MULT, _PI, _DIV, // *RAD, if input was real
  _ISREAL, _1, _EQ, _IF, _ELSE, _CMPLX, _DROP, _THEN, // if result is imaginary - set im to 0
  _ELSE, _DROP, _THEN, _END,
  _END // Double _END needed to identify end of area
};


void setup() {
  pinMode(STROBE, OUTPUT); pinMode(CLOCK, OUTPUT); pinMode(DATA, OUTPUT);
  brightness = EEPROM[EEBRIGHTNESS]; set_brightness(0); // Read and set brightness
  darktime = EEPROM[EEDARKTIME]; limitdarktime(); // Load darktime
  base = EEPROM[EEBASE]; // Load base mode
  Serial.begin(9600);
  setframerate(FRAMERATE);
  idofusr(); // Count sou and nou

  // Load test user programs to EEPROM
  //for (byte i = 0; i < sizeof(umem); i++) EEPROM[EEUSTART + i] = pgm_read_byte(&(umem[i]));

}

void loop() {
  if (isprintscreen) isprintscreen = printscreen(); // Print screen
  if (pause) { // Pause
    delaylong(pause);
    isprintalpha = false;
    pause = 0;
  }

  if (mp) { // *** Execute/run code
    if (msgnr) mp = 0; // Stop running if message is shown
    if (mp < sizeof(mem)) key = pgm_read_byte(mem + mp++); // Builtin
    else if (mp < sizeof(mem) + sou) key = EEPROM[mp++ -sizeof(mem) + EEUSTART]; // User
    else mp = 0; // cmd > MAXCMDU
    if (key >= MAXCMDI && key != _END) apush(mp); // Subroutine detected - branch

    if (key == GETKEY_CODE) inkey(); // getkey demanded

    else if (key == _END) { // _END reached
      if (ap) mp = apop(); // End of subroutine - return
      else { // End of run
        mp = 0;
        isprintscreen = true; // Finally print screen
      }
    }
    else execute(key);
    if (getkey() == _KX) mp = 0; // Stop program execution
  }

  else { // *** Evaluate valid new key
    if (!(nextFrame())) return; // Idle for next frame
    key = getkey(); // READ KEY

    if (getkey() == _KX) issolve = isint = isprintalpha = false; // Stop execution
    if (key == _KF && !base) { // Check MENU (longpressed f)
      if (millis() - timestamp > FLONGPRESSTIME) {
        if (isprgedit) { // Menu from prgedit demanded
          isprgmenu = true; isprgedit = false;
        }
        else { // Menu demanded
          fgm = 0;
          freleased = false;
        }
        ismenu = isprintscreen = true;
        select = 0; // Comment out, if menu should't start at 0
      }
    }
    else timestamp = millis();

    if ((byte)((millis() - powertimestamp) / 10000L) >= darktime) { // Dark if no activity
      _keygoff(); anicount = 0; isprintscreen = true;
    }

    if (issolve) { // # SOLVE
      cycles++;
      if (cycles < 3) { // First run - f(x0)
        execute(MAXCMDB);
        if (cycles == 2) { // Second run - f(x0+dx)
          f0 = dpopr(); // Save f0 from first run
          dpushr(x0 + DELTAX); // Prepare new x-value for next run
        }
      }
      else { // Third run - x1
        double diffx = DELTAX / (dpopr() / f0 - 1.0);
        dpushr(x0 -= diffx); // Improved x-value
        cycles = 0;
        if (diffx < DELTAX && diffx > -DELTAX) { // Exit
          isnewnumber = true; issolve = false;
          anicount = 0;
          isprintscreen = true;
        }
      }
    }
    else if (isint) { // # INTEGRATE
      cycles++;
      execute(MAXCMDB);
      if (cycles == 1) {} // First run - f(b)
      else if (cycles == 2) { // Second run - f(a)
        intf += dpopr(); // F=F+f(b)
        dpushr(inta);
      }
      else if (cycles == 3) { // Third run - xa
        intf += dpopr(); // F=F+f(a)
        dpushr(intx = inta + intdelta);
      }
      else if (cycles <= 2 * INTSTRIPES - 1 + 3) { // 2n-1 stripes
        double tmp = 4 * ((cycles - 3) / 2.0 - (int)((cycles - 3) / 2.0) + 0.5);
        tmp *= dpopr(); intf += tmp;
        dpushr(intx += intdelta);
      }
      else { // Last run
        dpopr(); // Delete last push (intx)
        dpushr(intf * intdelta / 3);
        mp = anicount = 0; // Prevent one more cycle and stop animation
        isnewnumber = isprintscreen = true;
        isint = false; // Exit
      }
    }


    if (key != oldkey) {
      oldkey = key; // New valid key is old key
      freleased = true;
      msgnr = 0;
      if (key < _END) {
        powertimestamp = millis(); // Keep awake when active
        darkscreen = false; // Reactivate screen in case of dark
        isprintalpha = false; // Stop printing alpha
        anicount = 0; // Stop animation if key was pressed

        if (isgetkey) inkey(); // ### Get keypress

        else if (view) { // ### View complex number
          if (key == _K0 || key == _KE) { // Previous
            if (view > 1) view--;
            else view = VIEWMAX - 1;
          }
          else if (key == _KD || key == _KN) { // Next
            if (view < VIEWMAX - 1) view++;
            else view = 1;
          }
          else if (key == _KP) { // Push viewed value to stack
            struct data a = dpop();
            if (view == VIEWRE) dpushr(a.r); // re
            else if (view == VIEWIM) dpushr(a.i); // im
            else if (view == VIEWABS) dpushr(absolute(a.r, a.i)); // abs
            else dpushr(angle(a.r, a.i)); // angle
            view = 0;
          }
          else if (key == _KX) { // Escape
            ispolar = false; if (view > 2) ispolar = true; // Set ispolar
            view = 0;
            anicount = _AEXIT;
          }
        }

        else if (isdict) { // ### Dictionary
          byte tmp = menuselect(MAXCMDU / 16);
          if (tmp < _PREEND) { // Item selected
            if (issetusr) { // Go back to select menu slot
              setusrselect = pgm_read_byte(&commentsort[tmp]);
              select = 0;
              issetusr = false; ismenu = ismenusetusr = true;
            }
            else if (isprgdict) { // Go back to prgedit
              if (tmp < MAXCMDB) tmp = pgm_read_byte(&commentsort[tmp]); // Intrinsic or builtin command
              prgstepins(tmp);
              isprgdict = false; isprgedit = true;
            }
            else { // Execute command directly
              if (tmp < MAXCMDB) tmp = pgm_read_byte(&commentsort[tmp]); // Intrinsic or builtin command
              execute(tmp);
              isnewnumber = true;
            }
            isdict = false;
          }
          else if (tmp == _END) { // Escape
            isdict = false;
          }
        }

        else if (ismenu) { // ### Menu
          if (freleased) { // Execute only if f-key was released
            byte tmp = menuselect(2);
            if (tmp < _PREEND) { // Item selected
              if (ismenusetusr) { // Save user menu entry
                EEPROM[EEMENU + tmp] = setusrselect;
                ismenusetusr = false;
              }
              else if (isprgmenu) { // Go back to prgedit
                prgstepins(EEPROM[EEMENU + tmp]);
                isprgmenu = false; isprgedit = true;
              }
              else { // Execute selected command
                execute(EEPROM[EEMENU + tmp]); // Execute directly
                isnewnumber = true;
              }
              ismenu = false;
            }
            else if (tmp == _END) ismenu = ismenusetusr = false; // Escape
          }
        }

        else if (isprgselect) { // ### Select program
          if (key == _KE) // # 6 - move up
            if (prgselect > 0) prgselect--;
            else prgselect = nou - 1;
          else if (key == _KN) // # 3 - move down
            if (prgselect < nou - 1) prgselect++;
            else prgselect = 0;
          else if (key == _K0) { // # 0 - rename program
            prgaddr = seekusrp(0);
            select = renamenr = 0;
            isprgrename = true; isprgselect = false;
          }
          else if (key == _K1) { // # 1 - new program
            prgeditstart = prgbuflen = 0;
            isprgnew = isprgeditfirst = isprgedit = true; isprgselect = false;
            idofusr();
          }
          else if (key == _KD) { // # Dot - delete program
            if (nou > 1) {
              int a1 = seekusrp(0), a2 = seekusrp(1);
              byte diff = a2 - a1;
              for (int i = a1; i < EEUSTART + sou - diff; i++) // Adjust programs
                EEPROM[i] = EEPROM[i + diff];
              if (prgselect == nou - 1) prgselect--; // Last program in list deleted
            }
            else { // Delete the last remaining program -> format
              EEPROM[EEUSTART] = _END; EEPROM[EEUSTART + 1] = _END;
              prgselect = 0;
            }
            idofusr();
          }
          else if (key == _K6) { // # 6 - move up
            if (prgselect) {
              moveprgup(); prgselect--;
            }
          }
          else if (key == _K3) { // # 3 - move down (= move up with prgselect++)
            if (prgselect < nou - 1) {
              prgselect++;
              moveprgup();
            }
          }
          else if (key == _K2) { // # 2 - load program (in)
            byte in, nr = 0;
            long tim = millis();
            prgbuf[0] = NULL;
            while (Serial.available() > 0) byte tmp = Serial.read(); // Flush serial buffer
            while (Serial.available() <= 0 && millis() - tim < 3000) ; // Wait for serial input
            while (Serial.available() > 0 && (in = Serial.read()) >= ' ' && nr < PRGSIZEMAX) {
              prgbuf[nr++] = in - ' ';
              prgbuf[nr] = _END;
              idofusr();
            }
            prgbuflen = nr;
            isprgnew = isprgedit = true; isprgselect = false;
          }
          else if (key == _K5) { // # 5 - save program (out)
            for (int i = seekusrp(0) + 3; i < seekusrp(1) - 1; i++)
              Serial.write(EEPROM[i] + ' ');
            delayshort(200);
          }
          else if (key == _KP && nou) { // # Enter - edit program
            prgaddr = seekusrp(0);
            prgbuflen = seekusrp(1) - prgaddr - PRGNAMEMAX - 1;
            oldprgbuflen = prgbuflen; // Save length for later
            for (byte i = 0 ; i < prgbuflen; i++) prgbuf[i] = EEPROM[prgaddr + PRGNAMEMAX + i];
            prgbuf[prgbuflen] = NULL;
            isprgselect = false; isprgedit = true;
          }
          else if (key == _KX) { // # Escape
            isprgselect = false;
            anicount = _AEXIT;
          }
        }

        else if (isprgrename) { // ### Program rename
          byte tmp = menuselect(16);
          if (tmp < _PREEND) // Item selected - replace letter of program name
            EEPROM[prgaddr + renamenr++] = tmp;
          if (tmp == _END || renamenr >= PRGNAMEMAX) { // Escape
            isprgrename = false; isprgselect = true;
          }
          idofusr();
        }

        else if (isprgedit) { // ### Program edit
          if (key == _KF) { // # F-key
            if (fgm == ISG) fgm = 0; else if (fgm == ISF) fgm = ISG; else fgm = ISF;
          }
          else if (key == _K4 && fgm == ISF) { // # F-4 ... select via dict
            select = 0; fgm = 0; isprgdict = isdict = true; isprgedit = false;
          }
          else if (fgm == ISF || fgm == ISG) // # Insert F- or G-shifted key
            prgstepins(key + fgm * 16);
          else if (key == _KE) { // # Up
            if (prgeditstart > 0) prgeditstart--; else prgeditstart = prgbuflen - 1;
            isprgeditfirst = false;
          }
          else if (key == _KN) { // # Down
            if (prgeditstart < prgbuflen - 1) prgeditstart++; else prgeditstart = 0;
            isprgeditfirst = false;
          }
          else if (key == _KD) { // # Dot - Delete step
            if (prgbuflen) {
              for (byte i = prgeditstart; i < prgbuflen; i++) prgbuf[i] = prgbuf[i + 1];
              prgbuflen--;
              if (prgeditstart == prgbuflen && prgeditstart) prgeditstart--; // Deleted last step
              if (isprgeditfirst && prgeditstart) prgeditstart--;
              if (!prgeditstart) isprgeditfirst = true;
            }
          }
          else if (key <= _K9 || key == _KP) prgstepins(key); // # Insert direct key
          else if (key == _KX) { // # Escape and save program to EEPROM
            if (isprgnew) { // Save new program
              if (sou + prgbuflen >= EEU) msgnr = MSGERROR; // New program is to big
              else {
                for (byte i = 0; i < PRGNAMEMAX; i++) EEPROM[EEUSTART + sou - 1 + i] = _7MINUS;
                for (byte i = 0; i < prgbuflen; i++) // Save program to EEPROM
                  EEPROM[EEUSTART + sou + PRGNAMEMAX - 1 + i] = prgbuf[i];
                for (byte i = 0; i < 2; i++) // Add 2 x _END
                  EEPROM[EEUSTART + sou + PRGNAMEMAX - 1 + prgbuflen + i] = _END;
                idofusr();
              }
              isprgnew = false;
            }
            else { // Save edited program
              int8_t diff = prgbuflen - oldprgbuflen;
              if (diff < 0) // New program is smaller
                for (int i = prgaddr + PRGNAMEMAX; i <= EEUSTART + sou; i++) // Adjust programs
                  EEPROM[i] = EEPROM[i - diff];
              else if (diff > 0) // New program is bigger
                for (int i = EEUSTART + sou + diff; i >= prgaddr + PRGNAMEMAX; i--) // Adjust programs
                  EEPROM[i] = EEPROM[i - diff];
              for (byte i = 0; i < prgbuflen; i++) // Save program to EEPROM
                EEPROM[prgaddr + PRGNAMEMAX + i] = prgbuf[i];
              idofusr();
            }
            prgbuflen = prgeditstart = 0; isprgselect = true; isprgedit = false;
            anicount = _AEXIT;
          }
        }

        else if (ismat) { // ### Matrix mode
          if (fgm == ISF) { // # Operation
            if (key == _KP) maddsub(0); // +
            else if (key == _K3) maddsub(1); // -
            else if (key == _K6) mmult(); // *
            else if (key == _KE) mswap(); // SWAP
            else if (key == _K0) mtrans(); // TRANS
            else if (key == _K9) minv(); // INV
            else if (key == _KD) { // DET
              mdeterminant();
              dpush({mdet, mdeti, 0LL});
            }
            else if (key == _KN) { // COPY
              if (mselect) memcpy(m[0], m[2], 2 * 3 * 3 * sizeof(double)); // A=B
              else ma2b(); // B=A
            }
            else if (key == _KX) memset(m, NULL, 3 * 2 * 3 * 3 * sizeof(double)); // CLR
            fgm = 0;
          }
          else if (key == _KF) fgm = fgm ? 0 : ISF; // # Toggle F-key
          else if (key == _KD) // # Load matrix from stack
            for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++)
                mpop(mselect, row, col);
          else if (key > _K0 && key <= _K9) { // Slot keys
            byte k = key - _K1;
            byte m2r[9] = {2, 2, 2, 1, 1, 1, 0, 0, 0}, m2c[9] = {0, 1, 2, 0, 1, 2, 0, 1, 2};
            matr = m2r[k]; matc = m2c[k];
            if (!ismatview) mpop(mselect, matr, matc); // POP
          }
          else if (key == _KE) mselect = mselect ? 0 : 1; // # Toggle MatA <-> MatB
          else if (key == _KN) ismatview = ismatview ? false : true; // # Toggle VIEW <-> POP
          else if (key == _KP && ismatview) { // # PUSH
            mpush(mselect, matr, matc);
            ismat = ismatview = false;
          }
          else if (key == _K0) { // Flush matrix to stack
            for (byte row = 0; row < 3; row++) for (byte col = 0; col < 3; col++)
                mpush(mselect, row, col);
            ismat = ismatview = false;
          }
          else if (key == _KX) { // # Escape
            ismat = ismatview = false;
            anicount = _AEXIT;
          }
        }

        else execute(key + fgm * 16); // ### Execute key
        isprintscreen = true;
      }
    }
    else if (anicount) animate();
  } // End of evaluating key

}
