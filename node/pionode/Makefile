#####################################################
# Makefile for Pionode 
# Any qustion : sanghyeun.kim@lge.com
#####################################################

#NODE_BIN := pionode

TOP_DIR	:= $(shell pwd)/../..
include $(TOP_DIR)/common.mk

src_dir := src
objs_dir := src/objs
lib_dir := $(LIB_OUT)
bin_dir := $(BIN_OUT)
include_dirs := $(TOP_DIR)/bus/include /usr/include/libxml2

#====================================================
# Source file 
#====================================================

SRC = 
SRC += Pionode.cpp
SRC += AtmgrInterface.cpp
SRCS += $(addprefix $(src_dir)/,$(SRC)) 

#object file
$(shell mkdir -p $(objs_dir))
SRCO = $(addprefix $(objs_dir)/,$(SRC))
OBJS = $(subst .cpp,.o,$(SRCO))

#====================================================
# Build Flags 
#====================================================
L_CFLAGS = $(addprefix -I,$(include_dirs)) 
L_LFLAGS = -lnbus -lxml2 -lpthread -lsqlite3
#====================================================
# Build Rules 
#====================================================
.PHONY: all target clean

target : all ;
all : 

clean : 
	rm -rf $(objs_dir) $(bin_dir)/$(NODE_BIN)
#====================================================
