# tool macros
CXX := g++
CXXFLAGS := 
DBGFLAGS := -g
CCOBJFLAGS := $(CXXFLAGS) -c

# path macros
BIN_PATH := bin
OBJ_PATH := obj
SRC_PATH := src
DBG_PATH := debug
INC_PATH := -Iinclude -Isrc/GUI_inc -Isrc/moore_inc
LIB_PATH := lib/gcc_dll
TES_PATH := tests

# compile macros
TARGET_NAME := moore
ifeq ($(OS),Windows_NT)
	TARGET_NAME := $(addsuffix .exe,$(TARGET_NAME))
endif
TARGET := $(BIN_PATH)/$(TARGET_NAME)
TARGET_DEBUG := $(DBG_PATH)/$(TARGET_NAME)

# src files & obj files
SRC := $(foreach x, $(SRC_PATH), $(wildcard $(addprefix $(x)/*,.c*)))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))
OBJ_DEBUG := $(addprefix $(DBG_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

#libs construction
LIBS := $(wildcard $(LIB_PATH)/*.dll)
LIBS := $(basename $(LIBS:$(LIB_PATH)/%=%))
LIBS := $(addprefix -l, $(LIBS))

# default rule
default: all

# non-phony targets
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) -L$(LIB_PATH) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(INC_PATH) $(CCOBJFLAGS) -o $@ $<


$(TARGET_DEBUG): $(OBJ_DEBUG)
	$(CXX) $(CXXFLAGS) $(DBGFLAGS) -o $@ $(OBJ_DEBUG) -L$(LIB_PATH) $(LIBS)

$(DBG_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(INC_PATH) $(CCOBJFLAGS) $(DBGFLAGS) -o $@ $<

# phony rules
.PHONY: dirs
dirs:
	@mkdir $(BIN_PATH) $(OBJ_PATH) $(DBG_PATH)
	@echo "Copying icons to $(BIN_PATH) and $(DBG_PATH)"
	@xcopy icons $(BIN_PATH)\icons /E /I /Y
	@xcopy icons $(DBG_PATH)\icons /E /I /Y

	@echo "Copying syntax_config to $(BIN_PATH) and $(DBG_PATH)"
	@copy syntax_config.txt $(BIN_PATH) /Y
	@copy syntax_config.txt $(DBG_PATH) /Y

	@echo "Dirs setup completed."

.PHONY: all
all: $(TARGET)

.PHONY: debug
debug: $(TARGET_DEBUG)

.PHONY: layout
layout:
	@echo LAYOUT
	move moore_lay.cpp $(SRC_PATH)/moore_lay.cpp
	move moore_lay.h $(SRC_PATH)/GUI_inc/moore_lay.h

.PHONY: clean
clean:
	@echo CLEAN
	del $(BIN_PATH)\$(TARGET_NAME)
	del $(OBJ_PATH)\*.o

.PHONY: execlean
execlean:
	@echo CLEAN
	del $(BIN_PATH)\$(TARGET_NAME)

# Help target
.PHONY: help
help:
	@echo "Makefile Help:"
	@echo "  make all       - Build the main target."
	@echo "  make debug     - Build the debug target."
	@echo "  make clean     - Remove compiled binaries and object files."
	@echo "  make execlean  - Remove the executable only."
	@echo "  make layout    - Move layout files to the source directory."
	@echo "  make dirs   - Create necessary directories and copy files (before first compilation)."
	@echo "  make help      - Display this help message."

# Test section
.PHONY: keysight
keysight:
# non-phony targets
$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $(OBJ) -L$(LIB_PATH) $(LIBS)

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(INC_PATH) $(CCOBJFLAGS) -o $@ $<