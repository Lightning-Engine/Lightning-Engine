ifndef platform
	platform	:= linux
endif

LIENGINE_DIR		:= Lightning-Engine
SANDBOX_DIR			:= Sandbox
LIENGINE_INCLUDE	:= -I$(LIENGINE_DIR)/include
SANDBOX_INCLUDE		:= -I$(LIENGINE_DIR)/include -I$(SANDBOX_DIR)/include
BIN_DIR				:= bin

ifeq ($(platform), linux)
	CC				:= gcc
	OFLAGS			:= -lX11 -ldl -lGL -fsanitize=address -g -Og
	CFLAGS			:= -Wall -Wextra -pedantic -std=c11 -DLI_PLATFORM_POSIX -fPIC -fsanitize=address -g -Og
	CXX				:= g++
	LIB_CMD			:= ar

	STATIC_PREFIX	:=
	SHARED_PREFIX	:= lib
	BIN_PREFIX		:=
	STATIC_SUFFIX	:= .a
	SHARED_SUFFIX	:= .so
	BIN_SUFFIX		:=

	LIENGINE_SRC	:= posix_dl.c li_win.c li_assert.c
	LIENGINE_WIN_SRC:= win/xlib_win.c win/xlib_keymap.c
	SANDBOX_SRC		:= main.c
else ifeq ($(platform), linux-mingw)
	CC				:= x86_64-w64-mingw32-gcc
	OFLAGS			:= -lopengl32 -lgdi32
	CFLAGS			:= -Wall -Wextra -pedantic -std=c11 -DLI_PLATFORM_WINDOWS -fPIC -municode
	CXX				:= x86_64-w64-mingw32-g++
	LIB_CMD			:= x86_64-w64-mingw32-ar

	STATIC_PREFIX	:=
	SHARED_PREFIX	:=
	BIN_PREFIX		:=
	STATIC_SUFFIX	:= .lib
	SHARED_SUFFIX	:= .dll
	BIN_SUFFIX		:= .exe

	LIENGINE_SRC	:= win32_dl.c li_win.c li_assert.c
	LIENGINE_WIN_SRC:= win/win32_win.c win/win32_keymap.c
	SANDBOX_SRC		:= main.c
else
$(error "invalid platform $(platform)")
endif

LIENGINE_SRC		:= $(addprefix $(LIENGINE_DIR)/src/,$(LIENGINE_SRC))
LIENGINE_WIN_SRC	:= $(addprefix $(LIENGINE_DIR)/src/,$(LIENGINE_WIN_SRC))
SANDBOX_SRC			:= $(addprefix $(SANDBOX_DIR)/src/,$(SANDBOX_SRC))
LIENGINE_OBJ		:= $(LIENGINE_SRC:$(LIENGINE_DIR)/src/%.c=$(LIENGINE_DIR)/obj/%.o)
LIENGINE_WIN_OBJ	:= $(LIENGINE_WIN_SRC:$(LIENGINE_DIR)/src/%.c=$(LIENGINE_DIR)/obj/%.o)
SANDBOX_OBJ			:= $(SANDBOX_SRC:$(SANDBOX_DIR)/src/%.c=$(SANDBOX_DIR)/obj/%.o)
LIENGINE_BIN		:= bin/$(SHARED_PREFIX)liengine$(SHARED_SUFFIX)
LIENGINE_WIN_BIN	:= bin/$(SHARED_PREFIX)liengine_win$(SHARED_SUFFIX)
SANDBOX_BIN			:= bin/$(BIN_PREFIX)sandbox$(BIN_SUFFIX)

all: $(LIENGINE_WIN_BIN) $(SANDBOX_BIN)

$(LIENGINE_BIN): $(LIENGINE_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(LIENGINE_OBJ) -o $(LIENGINE_BIN) -shared $(OFLAGS)

$(LIENGINE_WIN_BIN): $(LIENGINE_BIN) $(LIENGINE_WIN_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(LIENGINE_WIN_OBJ) -o $(LIENGINE_WIN_BIN) -shared $(OFLAGS) $(LIENGINE_BIN)

$(SANDBOX_BIN): $(LIENGINE_BIN) $(SANDBOX_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(SANDBOX_OBJ) -o $(SANDBOX_BIN) $(OFLAGS) $(LIENGINE_BIN)

$(LIENGINE_DIR)/obj/%.o: $(LIENGINE_DIR)/src/%.c
	mkdir -p $(shell dirname $@)
	$(CC) $(CFLAGS) -o $@ $< -c $(LIENGINE_INCLUDE)

$(SANDBOX_DIR)/obj/%.o: $(SANDBOX_DIR)/src/%.c
	mkdir -p $(SANDBOX_DIR)/obj
	$(CC) $(CFLAGS) -o $@ $< -c $(SANDBOX_INCLUDE)

clean:
	rm -rf $(LIENGINE_DIR)/obj
	rm -f bin/sandbox
	rm -f bin/sandbox.exe
	rm -f bin/libliengine.so
	rm -f bin/liengine.dll
	rm -f bin/libliengine_win.so
	rm -f bin/liengine_win.dll
	rm -rf $(SANDBOX_DIR)/obj

re: clean all

run: re
	LSAN_OPTIONS=suppressions=lightning.supp $(SANDBOX_BIN)