CC			:= clang
CXX			:= clang++
AR			:= ar
CFLAGS		:= -std=c89 -Wall -Wextra -pedantic
CXXFLAGS	:= -std=c++98 -Wall -Wextra -pedantic
LDFLAGS		:=
MAKEFLAGS	:= --no-print-directory

SILENT		:= $(if $(verbose),,@)
PRETTY		:= $(if $(ugly),@#,@)

platform	?= linux
config		?= debug
link		?= static

ifeq ($(config), debug)
	CFLAGS		+= -Og -g3
	LDFLAGS		+= -Og -g3
else ifeq ($(config), release)
	CFLAGS		+= -O3 -g3
	LDFLAGS		+= -O3 -g3
	no_san		:= 1
else ifeq ($(config), distr)
	CFLAGS		+= -O3 -g0
	LDFLAGS		+= -O3 -g0
	no_san		:= 1
else
$(error invalid config '$(config)')
endif

ifeq ($(platform), linux)
	lib_file	= lib$(1).a
	dll_file	= lib$(1).so
	bin_file	= $(1)
	CFLAGS		+= -DLI_LINUX=1
	color		:= 1
else ifeq ($(platform), mingw)
	lib_file	= $(1).lib
	dll_file	= $(1).dll
	bin_file	= $(1).exe
	CC			:= x86_64-w64-mingw32-gcc
	CXX			:= x86_64-w64-mingw32-g++
	AR			:= x86_64-w64-mingw32-ar
	CFLAGS		+= -DLI_WIN32=1
	color		:= 1
	no_san		:= 1
else ifeq ($(platform), macos)
	lib_file	= lib$(1).a
	dll_file	= lib$(1).so
	bin_file	= $(1)
	CFLAGS		+= -DLI_MACOS=1
	color		:= 1
	no_san		:= 1
else
$(error invalid platform '$(platform)')
endif

san			?= $(if $(no_san),none,address)

ifeq ($(san), none)
	
else ifdef no_san
$(error sanitizer is not supported for '$(platform)-$(config)')
else ifeq ($(san), address)
	CFLAGS		+= -fsanitize=address,undefined
	LDFLAGS		+= -fsanitize=address,undefined
else ifeq ($(san), memory)
	CFLAGS		+= -fsanitize=memory,undefined
	LDFLAGS		+= -fsanitize=memory,undefined
else ifeq ($(san), thread)
	CFLAGS		+= -fsanitize=thread,undefined
	LDFLAGS		+= -fsanitize=thread,undefined
else
$(error invalid sanitizer '$(san)')
endif

ifdef color
	RESET				:= "\\033[0m"
	BOLD				:= "\\033[1m"
	FAINT				:= "\\033[2m"
	ITALIC				:= "\\033[3m"
	UNDERLINE			:= "\\033[4m"
	SLOW_BLINK			:= "\\033[5m"
	FAST_BLINK			:= "\\033[6m"
	INVERT				:= "\\033[7m"
	FG_BLACK			:= "\\033[30m"
	FG_RED				:= "\\033[31m"
	FG_GREEN			:= "\\033[32m"
	FG_YELLOW			:= "\\033[33m"
	FG_BLUE				:= "\\033[34m"
	FG_MAGENTA			:= "\\033[35m"
	FG_CYAN				:= "\\033[36m"
	FG_WHITE			:= "\\033[37m"
	BG_BLACK			:= "\\033[40m"
	BG_RED				:= "\\033[41m"
	BG_GREEN			:= "\\033[42m"
	BG_YELLOW			:= "\\033[43m"
	BG_BLUE				:= "\\033[44m"
	BG_MAGENTA			:= "\\033[45m"
	BG_CYAN				:= "\\033[46m"
	BG_WHITE			:= "\\033[47m"
	FG_BRIGHT_BLACK		:= "\\033[90m"
	FG_BRIGHT_RED		:= "\\033[91m"
	FG_BRIGHT_GREEN		:= "\\033[92m"
	FG_BRIGHT_YELLOW	:= "\\033[93m"
	FG_BRIGHT_BLUE		:= "\\033[94m"
	FG_BRIGHT_MAGENTA	:= "\\033[95m"
	FG_BRIGHT_CYAN		:= "\\033[96m"
	FG_BRIGHT_WHITE		:= "\\033[97m"
	BG_BRIGHT_BLACK		:= "\\033[100m"
	BG_BRIGHT_RED		:= "\\033[101m"
	BG_BRIGHT_GREEN		:= "\\033[102m"
	BG_BRIGHT_YELLOW	:= "\\033[103m"
	BG_BRIGHT_BLUE		:= "\\033[104m"
	BG_BRIGHT_MAGENTA	:= "\\033[105m"
	BG_BRIGHT_CYAN		:= "\\033[106m"
	BG_BRIGHT_WHITE		:= "\\033[107m"
endif

tmp_dir			:= build
doc_dir			:= docs
rep_dir			:= reports

tmp_dir_engine	:= $(tmp_dir)/$(platform)-$(config)-engine
src_dir_engine	:= engine/src
inc_dir_engine	:= engine/include
src_engine		:= $(shell find $(src_dir_engine) \( -name "*.c" -o -name "*.cc" \))
inc_engine		:= $(shell find $(inc_dir_engine) \( -name "*.h" -o -name "*.hh" \))
obj_engine		:= $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.o,$(basename $(src_engine)))
dep_engine		:= $(patsubst $(src_dir_engine)/%,$(tmp_dir_engine)/%.d,$(basename $(src_engine)))
lib_engine		:= $(tmp_dir)/$(call lib_file,engine)
dll_engine		:= $(tmp_dir)/$(call dll_file,engine)

tmp_dir_sandbox	:= $(tmp_dir)/$(platform)-$(config)-sandbox
src_dir_sandbox	:= sandbox/src
inc_dir_sandbox	:= sandbox/include
src_sandbox		:= $(shell find $(src_dir_sandbox) \( -name "*.c" -o -name "*.cc" \))
inc_sandbox		:= $(shell find $(inc_dir_sandbox) \( -name "*.h" -o -name "*.hh" \))
obj_sandbox		:= $(patsubst $(src_dir_sandbox)/%,$(tmp_dir_sandbox)/%.o,$(basename $(src_sandbox)))
dep_sandbox		:= $(patsubst $(src_dir_sandbox)/%,$(tmp_dir_sandbox)/%.d,$(basename $(src_sandbox)))
bin_sandbox		:= $(tmp_dir)/$(call bin_file,sandbox)

ifeq ($(link), static)
	bin_engine	:= $(lib_engine)
else ifeq ($(link), dynamic)
	bin_engine	:= $(dll_engine)
else
$(error invalid linkage '$(link)')
endif

.PHONY: all sandbox engine
.PHONY: docs compdb format tidy analyze
.PHONY: clean run re rerun
.DEFAULT: sandbox

all: $(bin_sandbox) $(lib_engine) $(dll_engine)
sandbox: $(bin_sandbox)
engine: $(bin_engine)

-include $(dep_engine)
-include $(dep_sandbox)

MAKING_EXE	= $(FG_YELLOW)Linking$(RESET) $(FG_RED)$(1)$(RESET)
MAKING_LIB	= $(FG_YELLOW)Archiving$(RESET) $(FG_RED)$(1)$(RESET)
MAKING_DLL	= $(FG_YELLOW)Linking$(RESET) $(FG_RED)$(1)$(RESET)
COMPILING	= $(FG_CYAN)Compiling$(RESET) $(FG_GREEN)$(1)$(RESET)
RUNNING		= $(FG_MAGENTA)Running$(RESET) $(FG_BLUE)$(1)$(RESET)
CLEANING	= $(FG_MAGENTA)Cleaning$(RESET)
EXECUTING	= $(FG_MAGENTA)Executing$(RESET) $(FG_RED)$(1)$(RESET)

$(bin_sandbox): $(obj_sandbox) $(bin_engine)
	$(PRETTY)printf "$(call MAKING_EXE,$@)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CXX) $(LDFLAGS) -o $@ $^

$(lib_engine): $(obj_engine)
	$(PRETTY)printf "$(call MAKING_LIB,$@)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(AR) rcs $@ $^

$(dll_engine): $(obj_engine)
	$(PRETTY)printf "$(call MAKING_DLL,$@)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CXX) $(LDFLAGS) -shared -o $@ $^

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.c
	$(PRETTY)printf "$(call COMPILING,$<)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CC) $(CFLAGS) -c -MMD -o $@ $< -I$(inc_dir_engine)

$(tmp_dir_engine)/%.o: $(src_dir_engine)/%.cc
	$(PRETTY)printf "$(call COMPILING,$<)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CXX) $(CFLAGS) -c -MMD -o $@ $< -I$(inc_dir_engine)

$(tmp_dir_sandbox)/%.o: $(src_dir_sandbox)/%.c
	$(PRETTY)printf "$(call COMPILING,$<)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CC) $(CFLAGS) -c -MMD -o $@ $< -I$(inc_dir_engine) -I$(inc_dir_sandbox)

$(tmp_dir_sandbox)/%.o: $(src_dir_sandbox)/%.cc
	$(PRETTY)printf "$(call COMPILING,$<)\n"
	$(SILENT)mkdir -p $(@D)
	$(SILENT)$(CXX) $(CFLAGS) -c -MMD -o $@ $< -I$(inc_dir_engine) -I$(inc_dir_sandbox)

docs:
	$(PRETTY)printf "$(call RUNNING,doxygen)\n"
	$(SILENT)doxygen

compdb:
	$(PRETTY)printf "$(call RUNNING,compiledb)\n"
	$(SILENT)compiledb -n $(MAKE) all

format:
	$(PRETTY)printf "$(call RUNNING,clang-format)\n"
	$(SILENT)clang-format -i $(src_engine) $(src_sandbox) $(inc_engine) $(inc_sandbox)

tidy: compdb
	$(PRETTY)printf "$(call RUNNING,clang-tidy)\n"
	$(SILENT)clang-tidy $(src_engine) $(src_sandbox) $(inc_engine) $(inc_sandbox)

analyze: compdb
	$(PRETTY)printf "$(call RUNNING,CodeChecker)\n"
	$(SILENT)CodeChecker analyze -c --ctu --enable extreme compile_commands.json -o $(rep_dir)
	$(SILENT)CodeChecker parse -e html $(rep_dir) -o $(rep_dir)/html

clean:
	$(PRETTY)printf "$(call CLEANING)\n"
	$(SILENT)rm -rf $(tmp_dir)
	$(SILENT)rm -rf $(doc_dir)
	$(SILENT)rm -rf $(rep_dir)
	$(SILENT)rm -rf compile_commands.json

run: sandbox
	$(PRETTY)printf "$(call EXECUTING,$(bin_sandbox))\n"
	$(SILENT)$(bin_sandbox)

re: clean
	$(SILENT)$(MAKE) all

rerun: clean
	$(SILENT)$(MAKE) run
