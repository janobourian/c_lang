#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600
#define BYTES_PER_PIXEL 3
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// Dog sprite data: 16 rows by 16 columns
// 'X' = Body, 'E' = Eye, 'C' = Collar, '.' = Background
const char DOG_SPRITE[16][16] = {
    "....XXXX........",
    "...XXXXXX.......",
    "..XXEXXEXX......",
    "..XXXXXXXX......",
    "..XXXXXXXX......",
    "...CCCCCC.......",
    "....XXXX...X....",
    "....XXXX..XX....",
    "....XXXXXXXXX...",
    "...XXXXXXXXXXX..",
    "..XXXXXXXXXXXXX.",
    ".XXXXXXXXXXXXXX.",
    ".XXXXXXXXXXXXXX.",
    ".XX.XX....XX.XX.",
    ".XX.XX....XX.XX.",
    ".XX.XX....XX.XX."
};

void draw_pixel_block(uint8_t image[HEIGHT][WIDTH][BYTES_PER_PIXEL], int start_x, int start_y, int size, uint8_t r, uint8_t g, uint8_t b) {
    for (int y = 0; y < size; y++) {
        for (int x = 0; x < size; x++) {
            int px = start_x + x;
            int py = start_y + y;
            if (px >= 0 && px < WIDTH && py >= 0 && py < HEIGHT) {
                // BMP format uses Blue-Green-Red byte alignment
                image[py][px][0] = b; 
                image[py][px][1] = g;
                image[py][px][2] = r;
            }
        }
    }
}

void save_bmp(uint8_t image[HEIGHT][WIDTH][BYTES_PER_PIXEL], const char* filename) {
    FILE *f = fopen(filename, "wb");
    if (!f) return;

    int pad_size = (4 - (WIDTH * BYTES_PER_PIXEL) % 4) % 4;
    int image_size = (WIDTH * BYTES_PER_PIXEL + pad_size) * HEIGHT;
    int file_size = FILE_HEADER_SIZE + INFO_HEADER_SIZE + image_size;

    unsigned char file_header[FILE_HEADER_SIZE] = {
        'B', 'M',
        file_size, file_size >> 8, file_size >> 16, file_size >> 24,
        0, 0, 0, 0,
        FILE_HEADER_SIZE + INFO_HEADER_SIZE, 0, 0, 0
    };

    unsigned char info_header[INFO_HEADER_SIZE] = {
        INFO_HEADER_SIZE, 0, 0, 0,
        WIDTH, WIDTH >> 8, WIDTH >> 16, WIDTH >> 24,
        HEIGHT, HEIGHT >> 8, HEIGHT >> 16, HEIGHT >> 24,
        1, 0, 24, 0,
        0, 0, 0, 0,
        image_size, image_size >> 8, image_size >> 16, image_size >> 24,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    fwrite(file_header, 1, FILE_HEADER_SIZE, f);
    fwrite(info_header, 1, INFO_HEADER_SIZE, f);

    unsigned char pad[3] = {0, 0, 0};
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            fwrite(&image[y][x], 1, BYTES_PER_PIXEL, f);
        }
        fwrite(pad, 1, pad_size, f);
    }
    fclose(f);
}

int main() {
    uint8_t(*image)[WIDTH][BYTES_PER_PIXEL] = calloc(HEIGHT * WIDTH * BYTES_PER_PIXEL, 1);
    if (!image) return 1;

    // 1. Extreme Contrast Background: Deep Pitch Black
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            image[y][x][0] = 5;   // Dark blue hint
            image[y][x][1] = 5;   // Dark green hint
            image[y][x][2] = 5;   // Dark red hint
        }
    }

    // 2. Define High-Contrast Neon Colors
    uint8_t orange_r = 255, orange_g = 100, orange_b = 0;   // Bright Neon Orange (Body)
    uint8_t cyan_r   = 0,   cyan_g   = 240, cyan_b   = 255; // Electrifying Cyan (Eyes/Collar)

    // Scaling mechanics for retro pixel block look
    int block_size = 24; 
    int total_sprite_w = 16 * block_size;
    int total_sprite_h = 16 * block_size;
    int offset_x = (WIDTH - total_sprite_w) / 2;
    int offset_y = (HEIGHT - total_sprite_h) / 2;

    // 3. Render map grid to the matrix
    for (int row = 0; row < 16; row++) {
        for (int col = 0; col < 16; col++) {
            char pixel_type = DOG_SPRITE[row][col];
            int px = offset_x + (col * block_size);
            // Flip Y axis logic to align header orientations with standard BMP formats
            int py = offset_y + ((15 - row) * block_size); 

            if (pixel_type == 'X') {
                draw_pixel_block(image, px, py, block_size - 2, orange_r, orange_g, orange_b); // Body with thin border gap
            } else if (pixel_type == 'E' || pixel_type == 'C') {
                draw_pixel_block(image, px, py, block_size - 2, cyan_r, cyan_g, cyan_b);       // Accents
            }
        }
    }

    save_bmp(image, "contrast_8bit_dog.bmp");
    free(image);

    printf("High-contrast 8-bit dog image compiled successfully into contrast_8bit_dog.bmp\n");
    return 0;
}
