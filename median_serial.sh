#!/bin/bash

CC=gcc
EXEC=median_serial
SRC=median_serial.c
COMP=imageio.c
IN1=Input_Image1.pgm
IN2=Input_Image2.pgm
IN3=Input_Image3.pgm
IN4=Input_Image4.pgm
IN5=Input_Image5.pgm

OUT=MedianFiltered_Image_Serial.pgm

if [ "$SRC" -nt "$EXEC" ]
then
   echo "Compiling..."
   $CC -o $EXEC $SRC $COMP 
fi

./$EXEC -n 9 $IN1 $IN2 $IN3 $IN4 $IN5 $OUT 
