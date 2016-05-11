SYSROOT := d:/Soft/MinGW/
INCLUDE_PATH := d:/Soft/MinGW/include/
COMPILER_PATH := d:/Soft/MinGW/bin/

CFLAGS := -std=c99 -g -Wall -I$(INCLUDE_PATH) -B$(COMPILER_PATH)

CC := gcc.exe

OUT_DIR := d:/ForSite/calculation_c/result/

.PHONY: all

VPATH = $(OUT_DIR)

test.exe : test.o common.o starmath.o
	$(COMPILER_PATH)$(CC) $(OUT_DIR)test.o $(OUT_DIR)common.o $(OUT_DIR)starmath.o -o $(OUT_DIR)$@

#test.exe : test.obj
#	echo Build test.exe
#	$(COMPILER_PATH)link.exe /LIBPATH:$(LIB_PATH1) /LIBPATH:$(LIB_PATH2) /OUT:$(OUT_DIR)$@ $^ 

common.o: common.c common.h eqtypes.h
	$(COMPILER_PATH)$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ common.c

starmath.o: starmath.c starmath.h eqtypes.h
	$(COMPILER_PATH)$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ starmath.c
  
test.o: test.c
	$(COMPILER_PATH)$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ test.c