BUILD_DIR = build
SRC = bitstream.c default_helpers.c deflate.c footer.c gunzip.c header.c huffman_fixed.c LZ77.c main.c
OBJ = $(patsubst %.c, $(BUILD_DIR)/%.o, $(SRC))

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(BUILD_DIR)
	gcc -Wall -Wextra -O2 -I. -c $< -o $@

gunzip: $(OBJ)
	gcc -Wall -Wextra -O2 -o $@ $^

clean:
	rm -rf $(BUILD_DIR) gunzip

.PHONY: clean

