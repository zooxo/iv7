# iv7
## A Powerful Programable RPN Calculator based on FORTH (Arduino and 7 segment display)

  ____________________

  Welcome to Ivee 7 (IV7) - A Powerful Programable RPN Calculator based on FORTH
  running with an Arduino Micro and a QYF-Development-Board (7 segment display).  
  Version 1.0, (c) 2020 by deetee/zooxo  
  This software is covered by the 3-clause BSD license.
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

