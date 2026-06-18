#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define WIDTH 800
#define HEIGHT 600
#define BYTES_PER_PIXEL 3
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

void generate_bitmap(uint8_t image[HEIGHT][WIDTH][BYTES_PER_PIXEL], const char* filename) {
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
            fwrite(&image[y][x][2], 1, 1, f); // Blue
            fwrite(&image[y][x][1], 1, 1, f); // Green
            fwrite(&image[y][x][0], 1, 1, f); // Red
        }
        fwrite(pad, 1, pad_size, f);
    }
    fclose(f);
}

void draw_line(uint8_t image[HEIGHT][WIDTH][BYTES_PER_PIXEL], int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b) {
    int dx = abs(x2 - x1), sx = x1 < x2 ? 1 : -1;
    int dy = -abs(y2 - y1), sy = y1 < y2 ? 1 : -1;
    int err = dx + dy, e2;

    while (1) {
        if (x1 >= 0 && x1 < WIDTH && y1 >= 0 && y1 < HEIGHT) {
            image[y1][x1][0] = r;
            image[y1][x1][1] = g;
            image[y1][x1][2] = b;
        }
        if (x1 == x2 && y1 == y2) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x1 += sx; }
        if (e2 <= dx) { err += dx; y1 += sy; }
    }
}

int main() {
    // Allocate memory for the image matrix
    uint8_t(*image)[WIDTH][BYTES_PER_PIXEL] = calloc(HEIGHT * WIDTH * BYTES_PER_PIXEL, 1);
    if (!image) return 1;

    // Fill background with off-white (draft paper look)
    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            image[y][x][0] = 248;
            image[y][x][1] = 246;
            image[y][x][2] = 240;
        }
    }

    // Draw grid lines
    for (int i = 0; i < WIDTH; i += 50) {
        draw_line(image, i, 0, i, HEIGHT, 230, 225, 215);
        draw_line(image, 0, i, WIDTH, i, 230, 225, 215);
    }

    // Render Walls (Dark gray - BIM schematic)
    // Exterior Walls
    draw_line(image, 100, 100, 700, 100, 50, 50, 50);
    draw_line(image, 700, 100, 700, 500, 50, 50, 50);
    draw_line(image, 700, 500, 100, 500, 50, 50, 50);
    draw_line(image, 100, 500, 100, 100, 50, 50, 50);

    // Interior partitions
    draw_line(image, 350, 100, 350, 500, 80, 80, 80);
    draw_line(image, 100, 300, 350, 300, 80, 80, 80);

    // Render Doors (Green curves/lines)
    draw_line(image, 300, 500, 350, 450, 0, 150, 0); // Door swing
    draw_line(image, 350, 450, 350, 500, 0, 150, 0); // Door panel

    // Render Windows (Light blue lines along exterior)
    draw_line(image, 200, 100, 300, 100, 0, 150, 255);
    draw_line(image, 500, 100, 600, 100, 0, 150, 255);

    // Save output
    generate_bitmap(image, "bim_floorplan.bmp");
    free(image);
    
    printf("BIM Floorplan rendered successfully to bim_floorplan.bmp\n");
    return 0;
}
