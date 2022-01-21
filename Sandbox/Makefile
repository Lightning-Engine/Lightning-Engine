include ../Makefile.common

NAME			:= liengine

LIN_TARGET		:= $(OUT_DIR)/Sandbox
WIN_TARGET		:= $(OUT_DIR)/Sandbox.exe
OSX_TARGET		:= $(OUT_DIR)/Sandbox

TARGET			:=

PRJ_DIR			:= $(SOL_DIR)/Sandbox
SRC_DIR			:= $(PRJ_DIR)/src
SRCS			:= main.c
INC_DIR			:= $(PRJ_DIR)/include


VPATH			:= $(SRC_DIR)

INCLUDES		:= -I $(INC_DIR)

ifeq ($(platform), linux)
	TARGET		:= $(LIN_TARGET)
	VPATH		+= $(SRC_DIR)/platform/posix
else ifeq ($(platform), windows)
	TARGET		:= $(WIN_TARGET)
	VPATH		+= $(SRC_DIR)/platform/windows
else ifeq ($(platform), osx)
	TARGET		:= $(OSX_TARGET)
	VPATH		+= $(SRC_DIR)/platform/osx
else
$(error "invalid platform $(platform)")
endif

OBJS			:= $(addprefix $(INT_DIR)/,$(SRCS:%.c=%.o))

all: $(TARGET)

$(TARGET): $(OBJS)
	$(SILENT)mkdir -p $(OUT_DIR)
	$(SILENT)$(LINK_CMD) -o $(TARGET) $(OBJS) $(ALL_LINK_FLAGS)

$(INT_DIR)/%.o: %.c
	$(SILENT)echo $(notdir $<)
	$(SILENT)mkdir -p $(INT_DIR)
	$(SILENT)$(CC) $(ALL_CFLAGS) $(INCLUDES) -c $< -o $@

$(INT_DIR)/%.o: %.cpp
	$(SILENT)echo $(notdir $<)
	$(SILENT)mkdir -p $(INT_DIR)
	$(SILENT)$(CXX) $(ALL_CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	$(SILENT)rm -f $(OBJS)
	$(SILENT)rm -f $(TARGET)

re: clean all