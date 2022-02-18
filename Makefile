CFLAGS = -std=c11 -Wall -Wextra -pedantic -O0 -g3
CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -O0 -g
LDFLAGS =

tmp_dir = build/$(platform)
src_dir_engine = Lightning-Engine/src
src_dir_sandbox = Sandbox/src
inc_dir_engine = Lightning-Engine/include
inc_dir_sandbox = Sandbox/include
tmp_dir_engine = $(tmp_dir)/liengine
tmp_dir_sandbox = $(tmp_dir)/sandbox
bin_dir = $(tmp_dir)/bin

bin_engine = $(bin_dir)/$(call platform_shared,liengine)
src_engine = $(src_dir_engine)/li_assert.c $(src_dir_engine)/li_gl.c $(src_dir_engine)/li_win.c $(src_dir_engine)/window.cc $(src_dir_engine)/application.cc $(src_dir_engine)/util/log.c $(src_dir_engine)/util/list.c $(src_dir_engine)/memory.c $(src_dir_engine)/util/logger.cc
obj_engine = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.o,$(basename $(src_engine)))
dep_engine = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.d,$(basename $(src_engine)))

bin_sandbox = $(bin_dir)/$(call platform_executable,sandbox)
src_sandbox = $(src_dir_sandbox)/main.cc
obj_sandbox = $(patsubst $(src_dir_sandbox)/%,$(tmp_dir_sandbox)/%.o,$(basename $(src_sandbox)))
dep_sandbox = $(patsubst $(src_dir_sandbox)/%,$(tmp_dir_sandbox)/%.d,$(basename $(src_sandbox)))

bin_win_xlib = $(bin_dir)/$(call platform_shared,liengine_win_xlib)
src_win_xlib = $(src_dir_engine)/win/xlib_win.c $(src_dir_engine)/win/xlib_keymap.c
obj_win_xlib = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.o,$(basename $(src_win_xlib)))
dep_win_xlib = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.d,$(basename $(src_win_xlib)))

bin_win_win32 = $(bin_dir)/$(call platform_shared,liengine_win_win32)
src_win_win32 = $(src_dir_engine)/win/win32_win.c $(src_dir_engine)/win/win32_keymap.c
obj_win_win32 = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.o,$(basename $(src_win_win32)))
dep_win_win32 = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.d,$(basename $(src_win_win32)))

bin_win_cocoa = $(bin_dir)/$(call platform_shared,liengine_win_cocoa)
src_win_cocoa = $(src_dir_engine)/win/cocoa_win.c $(src_dir_engine)/win/cocoa_keymap.c
obj_win_cocoa = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.o,$(basename $(src_win_cocoa)))
dep_win_cocoa = $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.d,$(basename $(src_win_cocoa)))

BLACK					:="\033[0;30m"
RED						:="\033[0;31m"
GREEN					:="\033[0;32m"
ORANGE					:="\033[0;33m"
BLUE					:="\033[0;34m"
PURPLE					:="\033[0;35m"
CYAN					:="\033[0;36m"
LIGHT_GRAY				:="\033[0;37m"

DARK_GRAY				:="\033[1;30m"
LIGHT_RED				:="\033[1;31m"
LIGHT_GREEN				:="\033[1;32m"
YELLOW					:="\033[1;33m"
LIGHT_BLUE				:="\033[1;34m"
LIGHT_PURPLE			:="\033[1;35m"
LIGHT_CYAN				:="\033[1;36m"
WHITE					:="\033[1;37m"

RESET					:="\033[0m"

COMPILE_COLOR			:= $(GREEN)
LINK_COLOR				:= $(CYAN)
OBJECT_COLOR			:= $(RED)

ifndef platform
	platform = linux
endif

ifeq ($(platform),linux)
	CFLAGS += -fsanitize=address
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
	LDFLAGS += -Wl,-rpath,\$$ORIGIN
	src_engine += $(src_dir_engine)/posix_dl.c
	platform_engine_flags = -ldl
	platform_libs = $(bin_win_xlib)
	platform_shared = lib$(1).so
	platform_executable = $(1)
	platform_soname = -Wl,-soname,$(1)
else ifeq ($(platform), mingw)
	CC = x86_64-w64-mingw32-cc
	CXX = x86_64-w64-mingw32-g++
	CFLAGS += -municode
	CXXFLAGS += -municode
	LDFLAGS += -static
	src_engine += $(src_dir_engine)/win32_dl.c
	platform_engine_flags =
	platform_libs = $(bin_win_win32)
	platform_shared = $(1).dll
	platform_executable = $(1).exe
	platform_soname = -Wl,-soname,$(1)
else ifeq ($(platform), macos)
	CFLAGS += -fsanitize=address
	CXXFLAGS += -fsanitize=address
	LDFLAGS += -fsanitize=address
	LDFLAGS += -Wl,-rpath,@loader_path
	src_engine += $(src_dir_engine)/posix_dl.c
	platform_engine_flags = 
	platform_libs = $(bin_win_cocoa)
	platform_shared = lib$(1).so
	platform_executable = $(1)
	platform_soname = -Wl,-install_name,@rpath/$(1)
else
$(error "invalid platform $(platform)")
endif

SILENT		:= @
ifdef verbose
	SILENT	:=
endif

all: $(bin_sandbox) $(platform_libs)

$(bin_engine): $(obj_engine)
	@mkdir -p $(@D)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(CXX) $(LDFLAGS) $(call platform_soname,$(notdir $@)) -shared -o $@ $^ $(platform_engine_flags)

$(bin_sandbox): $(obj_sandbox) $(bin_engine)
	@mkdir -p $(@D)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(CXX) $(LDFLAGS) -o $@ $^

$(bin_win_xlib): $(obj_win_xlib) $(bin_engine)
	@mkdir -p $(@D)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(CXX) $(LDFLAGS) $(call platform_soname,$(notdir $@)) -shared -o $@ $^ -lX11 -lGL

$(bin_win_win32): $(obj_win_win32) $(bin_engine)
	@mkdir -p $(@D)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(CXX) $(LDFLAGS) $(call platform_soname,$(notdir $@)) -shared -o $@ $^ -lgdi32 -lopengl32

$(bin_win_cocoa): $(obj_win_cocoa) $(bin_engine)
	@mkdir -p $(@D)
	@printf $(LINK_COLOR)Linking$(RESET)\ $(OBJECT_COLOR)$(notdir $@)$(RESET)\\n
	$(SILENT)$(CXX) $(LDFLAGS) $(call platform_soname,$(notdir $@)) -shared -o $@ $^ -framework Cocoa -framework OpenGL

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.c
	@mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine)

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.cc
	@mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CXX) $(CXXFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine)

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.m
	@mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine) -Wno-deprecated-declarations

$(tmp_dir_sandbox)/%.o: $(src_dir_sandbox)/%.c
	@mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine) -I$(inc_dir_sandbox)

$(tmp_dir_sandbox)/%.o: $(src_dir_sandbox)/%.cc
	@mkdir -p $(@D)
	@printf $(COMPILE_COLOR)Compiling$(RESET)\ $(notdir $<)\\n
	$(SILENT)$(CXX) $(CXXFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine) -I$(inc_dir_sandbox)

.PHONY: clean
clean:
	$(SILENT)rm -rf $(tmp_dir)
.PHONY: re
re: clean
	$(SILENT)$(MAKE)
.PHONY: run
run: all
	$(SILENT)ASAN_OPTIONS=detect_leaks=0 ./$(bin_sandbox)
.PHONY: rerun
rerun: clean
	$(SILENT)$(MAKE) run

-include $(dep_engine)
-include $(dep_sandbox)
-include $(dep_win_xlib)
-include $(dep_win_win32)
-include $(dep_win_cocoa)
