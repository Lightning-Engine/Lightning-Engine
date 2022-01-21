include ../Makefile.common

NAME			:= liengine

LIN_TARGET		:= $(OUT_DIR)/lib$(NAME).a
WIN_TARGET		:= $(OUT_DIR)/$(NAME).lib
OSX_TARGET		:= $(OUT_DIR)/lib$(NAME).a

TARGET			:=

PRJ_DIR			:= $(SOL_DIR)/Lightning-Engine
SRC_DIR			:= $(PRJ_DIR)/src
SRCS			:= dl.c entry.c
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

all:
	echo $(SRC_DIR)

$(TARGET): $(OBJS)
	$(SILENT)mkdir -p $(OUT_DIR)
	$(SILENT)$(LIB_CMD) rcs $(TARGET) $(OBJS) -o

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