Target := AutoTest

CFLAGS := -Wall -g

INCLUDE := -I. -I.. -ICMockeryLib -I../CFakeLib

C_CODES	:=				\
	Test_Main.c			\
	Test_GameOfLife.c	\
	Test_CFakeLib.c		\

ifeq ($(TEST_SYS), 32)
	CFLAGS += -m32
endif

ifeq ($(TEST_SYS), 64)
	CFLAGS += -m64
endif

## Create a list of objects from list of source files
C_OBJECTS := $(patsubst %.c, %.o, ${C_CODES})
C_DEPENDS := $(patsubst %.c, %.d, ${C_CODES})

LIB_FAKE     := ../CFakeLib/libfake.a
LIB_CMOCKERY :=	"./CMockeryLib/libcmockery.a"

default: $(Target)

$(Target): $(C_OBJECTS)
	$(CC) -o $@ $(CFLAGS) $^ $(LIB_FAKE) $(LIB_CMOCKERY)

## Compile objects from C source
$(C_OBJECTS): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(INCLUDE)

$(C_DEPENDS): %.d: %.c
	$(CC) -M $< $(CFLAGS) $(INCLUDE) > $@

ifneq (,$(C_DEPENDS))
-include $(C_DEPENDS)
endif
