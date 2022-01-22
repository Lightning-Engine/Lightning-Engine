ifndef platform
	platform	:= linux
endif

LIENGINE_DIR		:= Lightning-Engine
SANDBOX_DIR			:= Sandbox
LIENGINE_INCLUDE	:= -I$(LIENGINE_DIR)/include
SANDBOX_INCLUDE		:= -I$(LIENGINE_DIR)/include

ifeq ($(platform), linux)
	CC				:= clang
	OFLAGS			:= -lX11
	CXX				:= clang++
	LIENGINE_SRC	:= platform/posix/dl.c platform/posix/entry.c platform/xlib/win.c platform/xlib/keymap.c
	LIENGINE		:= bin/liengine.a
	SANDBOX_SRC		:= main.c
	SANDBOX			:= bin/sandbox
	LIB_CMD			:= ar
else ifeq ($(platform), linux-mingw)
	CC				:= x86_64-w64-mingw32-gcc
	OFLAGS			:= -lmingw32
	CFLAGS			:= 
	CXX				:= x86_64-w64-mingw32-g++
	LIENGINE_SRC	:= platform/windows/dl.c platform/windows/entry.c platform/windows/win.c platform/windows/keymap.c
	LIENGINE		:= bin/liengine.lib
	SANDBOX_SRC		:= main.c
	SANDBOX			:= bin/sandbox.exe
	LIB_CMD			:= x86_64-w64-mingw32-ar
else
$(error "invalid platform $(platform)")
endif

LIENGINE_SRC		:= $(addprefix $(LIENGINE_DIR)/src/,$(LIENGINE_SRC))
SANDBOX_SRC			:= $(addprefix $(SANDBOX_DIR)/src/,$(SANDBOX_SRC))
LIENGINE_OBJ		:= $(LIENGINE_SRC:$(LIENGINE_DIR)/src/%.c=$(LIENGINE_DIR)/obj/%.o)
SANDBOX_OBJ			:= $(SANDBOX_SRC:$(SANDBOX_DIR)/src/%.c=$(SANDBOX_DIR)/obj/%.o)

all: $(LIENGINE) $(SANDBOX)

$(LIENGINE): $(LIENGINE_OBJ)
	$(LIB_CMD) rcs $(LIENGINE) $(LIENGINE_OBJ)

$(SANDBOX): $(LIENGINE) $(SANDBOX_OBJ)
	$(CC) $(SANDBOX_OBJ) -o $(SANDBOX) $(OFLAGS) $(LIENGINE) 

$(LIENGINE_DIR)/obj/%.o: $(LIENGINE_DIR)/src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $< -c $(LIENGINE_INCLUDE)

$(SANDBOX_DIR)/obj/%.o: $(SANDBOX_DIR)/src/%.c
	mkdir -p $(SANDBOX_DIR)/obj
	$(CC) $(CFLAGS) -o $@ $< -c $(SANDBOX_INCLUDE)

clean:
	rm -rf $(LIENGINE_DIR)/obj
	rm -f bin/liengine.a
	rm -f bin/liengine.lib
	rm -rf $(SANDBOX_DIR)/obj
	rm -f $(SANDBOX)

re: clean all
