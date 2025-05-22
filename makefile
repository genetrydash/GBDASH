# Directories
SRC_DIR := src
INC_DIR := include
BUILD_DIR := build

# Tools and flags
CC      := lcc
CFLAGS  := -I$(INC_DIR) -c
LDFLAGS := \
    -I$(INC_DIR) \
    -Wl-lhugedriver/gbdk/hUGEDriver.lib \
	-Wl-yt19 -Wl-yo8


# Sources and objects
SOURCES := $(wildcard $(SRC_DIR)/*.c)
OBJECTS := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SOURCES))
TARGET := $(BUILD_DIR)/game.gb

# Detect OS
UNAME_S := $(shell uname -s)

# Default rule
all: run_processtotxt prebuild $(TARGET)

# Run processtotxt.py before build
#run_processtotxt:
#	python3 processtotxt.py

# Link final binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile each .c to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Ensure build directory exists
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean prebuild run_processtotxt
