SRC_DIR = src
TARGET_DIR = target

SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(TARGET_DIR)/%.o,$(SRCS))

default: run

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS)
	@gcc -c $< -o $@

$(TARGET_DIR)/program: $(OBJECTS)
	@gcc $(OBJECTS) -o $@

build: $(TARGET_DIR)/program

run: $(TARGET_DIR)/program
	@$(TARGET_DIR)/program

clean:
	@-rm -f $(OBJECTS)
	@-rm -f $(TARGET_DIR)/program
