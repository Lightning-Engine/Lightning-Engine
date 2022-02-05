CFLAGS = -std=c11 -Wall -Wextra -pedantic -Og -g
LDFLAGS =

tmp_dir = build
src_dir_engine = Lightning-Engine/src
src_dir_sandbox = Sandbox/src
inc_dir_engine = Lightning-Engine/include
inc_dir_sandbox = Sandbox/include
tmp_dir_engine = $(tmp_dir)/liengine
tmp_dir_sandbox = $(tmp_dir)/sandbox
bin_dir = $(tmp_dir)/bin

bin_engine = $(bin_dir)/$(platform_shared_prefix)liengine$(platform_shared_suffix)
src_engine = $(src_dir_engine)/li_assert.c $(src_dir_engine)/li_gl.c $(src_dir_engine)/li_win.c $(platform_engine_source)
obj_engine = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.o,$(src_engine))
dep_engine = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.d,$(src_engine))

bin_sandbox = $(bin_dir)/$(platform_executable_prefix)sandbox$(platform_executable_suffix)
src_sandbox = $(src_dir_sandbox)/main.c
obj_sandbox = $(patsubst $(src_dir_sandbox)/%.c,$(tmp_dir_sandbox)/%.o,$(src_sandbox))
dep_sandbox = $(patsubst $(src_dir_sandbox)/%.c,$(tmp_dir_sandbox)/%.d,$(src_sandbox))

bin_win_xlib = $(bin_dir)/$(platform_shared_prefix)liengine_win_xlib$(platform_shared_suffix)
src_win_xlib = $(src_dir_engine)/win/xlib_win.c $(src_dir_engine)/win/xlib_keymap.c
obj_win_xlib = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.o,$(src_win_xlib))
dep_win_xlib = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.d,$(src_win_xlib))

bin_win_win32 = $(bin_dir)/$(platform_shared_prefix)liengine_win_win32$(platform_shared_suffix)
src_win_win32 = $(src_dir_engine)/win/win32_win.c $(src_dir_engine)/win/win32_keymap.c
obj_win_win32 = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.o,$(src_win_win32))
dep_win_win32 = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.d,$(src_win_win32))

bin_win_macos = $(bin_dir)/$(platform_shared_prefix)liengine_win_macos$(platform_shared_suffix)
src_win_macos = $(src_dir_engine)/win/macos_win.c $(src_dir_engine)/win/macos_keymap.c
obj_win_macos = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.o,$(src_win_macos))
dep_win_macos = $(patsubst $(src_dir_engine)/%.c,$(tmp_dir_engine)/%.d,$(src_win_macos))

ifndef platform
	platform = linux
endif

ifeq ($(platform),linux)
	LDFLAGS += -Wl,-rpath,\$$ORIGIN
	platform_engine_flags = -ldl
	platform_engine_libs = $(bin_win_xlib)
	platform_engine_source = $(src_dir_engine)/posix_dl.c
	platform_shared_prefix = lib
	platform_shared_suffix = .so
	platform_executable_prefix =
	platform_executable_suffix =
else ifeq ($(platform), mingw)
	CC = x86_64-w64-mingw32-cc
	CFLAGS = -municode
	platform_engine_flags =
	platform_engine_libs = $(bin_win_win32)
	platform_engine_source = $(src_dir_engine)/win32_dl.c
	platform_shared_prefix =
	platform_shared_suffix = .dll
	platform_executable_prefix =
	platform_executable_suffix = .exe
else ifeq ($(platform), macos)
	LDFLAGS += -Wl,-rpath,@loader_path
	platform_engine_flags =
	platform_engine_libs = $(bin_win_macos)
	platform_engine_source = $(src_dir_engine)/posix_dl.c
	platform_shared_prefix = lib
	platform_shared_suffix = .so
	platform_executable_prefix =
	platform_executable_suffix =
else
$(error "invalid platform $(platform)")
endif

all: $(bin_sandbox) $(platform_engine_libs)

$(bin_engine): $(obj_engine)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -Wl,-soname,$(notdir $@) -shared -o $@ $^ $(platform_engine_flags)

$(bin_sandbox): $(obj_sandbox) $(bin_engine)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -o $@ $^

$(bin_win_xlib): $(obj_win_xlib) $(bin_engine)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -Wl,-soname,$(notdir $@) -shared -o $@ $^ -lX11 -lGL

$(bin_win_win32): $(obj_win_win32) $(bin_engine)
	@mkdir -p $(@D)
	$(CC) $(LDFLAGS) -Wl,-soname,$(notdir $@) -shared -o $@ $^ -lgdi32 -lopengl32

$(bin_win_macos): $(obj_win_macos) $(bin_engine)
	@mkdir -p $(@D)
<<<<<<< HEAD
	$(CC) $(LDFLAGS) -shared -o $@ $^ -framework Cocoa -framework Quartz -framework OpenGL
=======
	$(CC) $(LDFLAGS) -Wl,-soname,$(notdir $@) -shared -o $@ $^ -framework Cocoa
>>>>>>> 105a2c80c720505637e0a1ba709ca23939a44975

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine)

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.m
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine)

$(tmp_dir_sandbox)/%.o: $(src_dir_sandbox)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c -fPIC -o $@ $< -MMD -I$(inc_dir_engine) -I$(inc_dir_sandbox)

.PHONY: clean
clean:
	rm -rf $(tmp_dir)
.PHONY: re
re: clean
	$(MAKE)
.PHONY: run
run: all
	./$(bin_sandbox)

-include $(dep)
-include $(dep_win_xlib)