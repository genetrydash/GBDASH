# Directories
SRC_DIR := src
MUSIC_DIR := $(SRC_DIR)/music
INC_DIR := include
BUILD_DIR := build
BUILD_MUSIC_DIR := $(BUILD_DIR)/music

# Tools and flags
CC      := lcc
CFLAGS  := -I$(INC_DIR) -I$(SRC_DIR) -c -debug
ROM_TITLE := GB_DASH

LDFLAGS := \
	-I$(INC_DIR) \
	-I$(SRC_DIR) \
	-Wl-lhugedriver/gbdk/hUGEDriver.lib \
	-Wl-yt19 -Wl-yo8 -debug


# Sources and objects
SRC_SOURCES   := $(wildcard $(SRC_DIR)/*.c)
MUSIC_SOURCES := $(wildcard $(MUSIC_DIR)/*.c)
SOURCES       := $(SRC_SOURCES) $(MUSIC_SOURCES)

SRC_OBJECTS   := $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC_SOURCES))
MUSIC_OBJECTS := $(patsubst $(MUSIC_DIR)/%.c,$(BUILD_MUSIC_DIR)/%.o,$(MUSIC_SOURCES))
OBJECTS       := $(SRC_OBJECTS) $(MUSIC_OBJECTS)

TARGET := $(BUILD_DIR)/game.gb

# Default rule
all: run_processtotxt prebuild $(TARGET)

# Run processtotxt.py before build
run_processtotxt:
	python processtotxt.py

# Link final binary
$(TARGET): $(OBJECTS)
	$(CC) $(LDFLAGS) -o $@ $^

# Compile each .c to .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

$(BUILD_MUSIC_DIR)/%.o: $(MUSIC_DIR)/%.c | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $<

# Ensure build directories exist
$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)
	mkdir -p $(BUILD_MUSIC_DIR)

# Clean build files
clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean prebuild run_processtotxt
