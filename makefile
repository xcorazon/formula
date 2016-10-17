SYSROOT := d:/Soft/MinGW/
INCLUDE_PATH := d:/Soft/MinGW/include/
COMPILER_PATH := d:/Soft/MinGW/bin/

OS = OS_WINDOWS
#CFLAGS := -std=c99 -g -Wall -O2 -I$(INCLUDE_PATH) -B$(COMPILER_PATH)
CFLAGS := -std=gnu99 -g -Wall -I$(INCLUDE_PATH) -B$(COMPILER_PATH) -D$(OS)

CC := gcc.exe

OUT_DIR := d:/ForSite/calculation_c/result/

.PHONY: all

VPATH = $(OUT_DIR)

test.exe : test.o common.o starmath.o summ.o mul.o transform.o reciprocal.o treeview.o debug.o sincos.o asincos.o
	$(CC) $(OUT_DIR)test.o\
                  $(OUT_DIR)common.o\
                  $(OUT_DIR)starmath.o\
                  $(OUT_DIR)summ.o\
                  $(OUT_DIR)mul.o\
                  $(OUT_DIR)reciprocal.o\
                  $(OUT_DIR)transform.o\
                  $(OUT_DIR)treeview.o\
                  $(OUT_DIR)debug.o\
                  $(OUT_DIR)sincos.o\
                  $(OUT_DIR)asincos.o\
                  -o $(OUT_DIR)$@

#test.exe : test.obj
#	echo Build test.exe
#	$(COMPILER_PATH)link.exe /LIBPATH:$(LIB_PATH1) /LIBPATH:$(LIB_PATH2) /OUT:$(OUT_DIR)$@ $^ 

common.o: common.c common.h eqtypes.h
	$(COMPILER_PATH)$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ common.c

starmath.o: starmath.c starmath.h eqtypes.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ starmath.c

treeview.o: treeview.c treeview.h eqtypes.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ treeview.c

summ.o: summ.c summ.h eqtypes.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ summ.c

mul.o: mul.c mul.h eqtypes.h common.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ mul.c

sincos.o: sincos.c sincos.h eqtypes.h common.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ sincos.c

asincos.o: asincos.c asincos.h common.h eqtypes.h summ.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ asincos.c

reciprocal.o: reciprocal.c reciprocal.h eqtypes.h common.h mul.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ reciprocal.c

transform.o: transform.c transform.c mul.h summ.h eqtypes.h common.h reciprocal.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ transform.c

debug.o: debug/debug.c debug/debug.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ debug/debug.c

test.o: test.c common.h eqtypes.h debug/debug.h
	$(CC) -c $(CFLAGS) -fdata-sections -ffunction-sections -o $(OUT_DIR)$@ test.c