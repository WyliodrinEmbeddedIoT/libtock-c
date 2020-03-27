# Makefile for user application

# Specify this directory relative to the current application.
TOCK_USERLAND_BASE_DIR = ../..

# Which files to compile.
C_SRCS := $(wildcard *.c)

# External libraries used
EXTERN_LIBS += $(TOCK_USERLAND_BASE_DIR)/jerryscript

override CFLAGS += -I$(TOCK_USERLAND_BASE_DIR)/jerryscript/jerry-core/include -DFEATURE_ERROR_MESSAGES=ON

# Include userland master makefile. Contains rules and flags for actually
# building the application.

include $(TOCK_USERLAND_BASE_DIR)/jerryscript/Makefile
include $(TOCK_USERLAND_BASE_DIR)/AppMakefile.mk
