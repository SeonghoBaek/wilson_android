#####################################################
# Makefile for plogmannode 
# Any qustion : sanghyeun.kim@lge.com
#####################################################
TOP_DIR	:= $(shell pwd)/../..
include $(TOP_DIR)/common.mk

src_dir := src
objs_dir := src/objs
lib_dir := $(LIB_OUT)
bin_dir := $(BIN_OUT)
include_dirs := include $(TOP_DIR)/bus/include /usr/include/libxml2

#====================================================
# Source file 
#====================================================
SRC = 
SRC += plogman.cpp
SRCS += $(addprefix $(src_dir)/,$(SRC)) 

#object file
$(shell mkdir -p $(objs_dir))
SRCO = $(addprefix $(objs_dir)/,$(SRC))
OBJS = $(subst .cpp,.o,$(SRCO))

#====================================================
# Build Flags 
#====================================================
L_CFLAGS = $(addprefix -I,$(include_dirs)) 
L_LFLAGS = -lnbus -lxml2 -lpthread
#====================================================
# Build Rules 
#====================================================
.PHONY: all target clean

target : all ;
all : $(OBJS) 
	@echo -e "\t@@@ Building plogmannode..."
	$(CXX) -o $(bin_dir)/plogmannode $(OBJS) $(CXXLFLAGS) $(L_LFLAGS) 

$(objs_dir)/%.o : $(src_dir)/%.cpp 
	$(CXX) -c $< $(CXXCFLAGS) $(L_CFLAGS) -o $(@)  

clean : 
	rm -rf $(objs_dir) $(bin_dir)/plogmannode
#====================================================

