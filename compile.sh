#!/bin/bash
gcc -o LSMC-User-Utility LSMC-User-Utility.c `pkg-config --libs --cflags gtk+-3.0` -w
