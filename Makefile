SRC_DIR = src
TARGET_DIR = target

SRCS = $(wildcard $(SRC_DIR)/*.c)
HEADERS = $(wildcard $(SRC_DIR)/*.h)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(TARGET_DIR)/%.o,$(SRCS))

default: run

$(TARGET_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | TARGET_DIR
	@gcc -g -c $< -o $@

TARGET_DIR:
	@mkdir -p $(TARGET_DIR)

stage1exe: $(OBJECTS)
	@gcc -g $(OBJECTS) -o $@

build: stage1exe

run: stage1exe
	@./stage1exe

clean:
	@-rm -f $(OBJECTS)
	@-rm -f stage1exe
