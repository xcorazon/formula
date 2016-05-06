INCLUDE_PATH := d:/Work/devtools/vc2008/include/

COMPILER_PATH := d:/Work/devtools/vc2008/bin/

LIB_PATH1 := d:/Work/devtools/vc2008/lib/
LIB_PATH2 := d:/Work/devtools/win32.psdk/Lib/

OUT_DIR := d:/ForSite/calculation_c/result/

LIBS := libcmt.lib

.PHONY: all

VPATH = $(OUT_DIR)

#test.exe : test.obj symbol.obj helper.obj
#	$(COMPILER_PATH)link.exe /LIBPATH:$(LIB_PATH1) /LIBPATH:$(LIB_PATH2) /OUT:$(OUT_DIR)$@ $^ 

#test.exe : test.obj
#	echo Build test.exe
#	$(COMPILER_PATH)link.exe /LIBPATH:$(LIB_PATH1) /LIBPATH:$(LIB_PATH2) /OUT:$(OUT_DIR)$@ $^ 

common.obj: common.c common.h eqtypes.h
	$(COMPILER_PATH)cl.exe /c /O2 /I$(INCLUDE_PATH) /Fo$(OUT_DIR)$@ common.c
