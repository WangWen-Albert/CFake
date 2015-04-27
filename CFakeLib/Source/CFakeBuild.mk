Target := ../libfake.a

CFLAGS := -Wall -g

INCLUDE := -I. -I..

C_CODES	:=			\
	CFake.c			\
	CFakePlatform.c	\
	CFakeDatabase.c	\
	CFakeLog.c		\

ifeq ($(FAKE_OS), win)
	C_CODES +=	CFakeOsWin.c
	CP		:=	cp
endif

ifeq ($(FAKE_OS), linux)
	C_CODES +=	CFakeOsLinux.c
	CP		:=	cp
endif

ifeq ($(FAKE_HW), x86)
	C_CODES +=	CFakeHwX86.c
endif

ifeq ($(FAKE_SYS), 32)
	CFLAGS += -m32
endif

ifeq ($(FAKE_SYS), 64)
	CFLAGS += -m64
endif

## Create a list of objects from list of source files
C_OBJECTS := $(patsubst %.c, %.o, ${C_CODES})
C_DEPENDS := $(patsubst %.c, %.d, ${C_CODES})
LIB       := ../libfake_$(FAKE_HW)$(FAKE_OS)$(FAKE_SYS).a

default: $(LIB)

$(Target): $(C_OBJECTS)
	ar cru $@ $^

$(LIB): $(Target)
	$(CP) $(Target) $(LIB)

## Compile objects from C source
$(C_OBJECTS): %.o: %.c
	$(CC) -c $< $(CFLAGS) $(INCLUDE)

$(C_DEPENDS): %.d: %.c
	$(CC) -M $< $(CFLAGS) $(INCLUDE) > $@

ifneq (,$(C_DEPENDS))
-include $(C_DEPENDS)
endif
