#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define WIDTH 1920
#define HEIGHT 1080
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// Helper function to find minimum / maximums
float fminf_custom(float a, float b) { return a < b ? a : b; }
float fmaxf_custom(float a, float b) { return a > b ? a : b; }

// Helper function to map dynamic values across cyclic sine waves
void hsl_to_rgb(float h, float s, float l, float *r, float *g, float *b) {
    float c = (1.0f - fabsf(2.0f * l - 1.0f)) * s;
    float x = c * (1.0f - fabsf(fmodf(h / 60.0f, 2.0f) - 1.0f));
    float m = l - c / 2.0f;
    float rt = 0, gt = 0, bt = 0;
    
    if (h < 60.0f)       { rt = c; gt = x; }
    else if (h < 120.0f) { rt = x; gt = c; }
    else if (h < 180.0f) { gt = c; bt = x; }
    else if (h < 240.0f) { gt = x; bt = c; }
    else if (h < 300.0f) { rt = x; bt = c; }
    else                 { rt = c; bt = x; }
    
    *r = (rt + m) * 255.0f;
    *g = (gt + m) * 255.0f;
    *b = (bt + m) * 255.0f;
}

// Infinite Menger Fractal Signed Distance Function for complex architecture shapes
float sd_menger_sponge(float x, float y, float z, float *out_color_index) {
    // Infinite space repetition to fill the landscape with structures
    float rx = fmodf(fabsf(x), 6.0f) - 3.0f;
    float ry = fmodf(fabsf(y), 6.0f) - 3.0f;
    float rz = fmodf(fabsf(z), 6.0f) - 3.0f;

    // Base Box Size
    float box_x = fabsf(rx) - 2.0f;
    float box_y = fabsf(ry) - 2.0f;
    float box_z = fabsf(rz) - 2.0f;
    float d = fmaxf_custom(box_x, fmaxf_custom(box_y, box_z));

    float scale = 1.0f;
    float color_accumulator = 0.0f;

    // 4 Iterations of fractal carving to generate complex geometry structures
    for (int i = 0; i < 4; i++) {
        float ax = fmodf(fabsf(rx * scale), 3.0f) - 1.5f;
        float ay = fmodf(fabsf(ry * scale), 3.0f) - 1.5f;
        float az = fmodf(fabsf(rz * scale), 3.0f) - 1.5f;
        scale *= 3.0f;

        float c_box_x = 1.0f - fabsf(ax);
        float c_box_y = 1.0f - fabsf(ay);
        float c_box_z = 1.0f - fabsf(az);

        float c = fmaxf_custom(fminf_custom(c_box_x, c_box_y), fminf_custom(c_box_y, c_box_z)) / scale;
        
        if (c > d) {
            d = c;
            color_accumulator += sinf(scale + ax) * 0.5f + 0.5f; // Track coordinates for colors
        }
    }
    *out_color_index = color_accumulator;
    return d;
}

// Raymarching scene traversal loop
float raymarch(float ro_x, float ro_y, float ro_z, float rd_x, float rd_y, float rd_z, float *color_idx) {
    float t = 0.0f;
    for (int i = 0; i < 120; i++) {
        float px = ro_x + rd_x * t;
        float py = ro_y + rd_y * t;
        float pz = ro_z + rd_z * t;

        float d = sd_menger_sponge(px, py, pz, color_idx);
        if (d < 0.002f) return t; // Ray struck an edge
        t += d * 0.9f;             // Safe distance step
        if (t > 40.0f) break;      // Clipping horizon
    }
    return -1.0f;
}

// Surface normal generator for structural shading profiles
void get_normal(float x, float y, float z, float *nx, float *ny, float *nz) {
    float eps = 0.005f;
    float dummy;
    float d = sd_menger_sponge(x, y, z, &dummy);
    *nx = sd_menger_sponge(x + eps, y, z, &dummy) - d;
    *ny = sd_menger_sponge(x, y + eps, z, &dummy) - d;
    *nz = sd_menger_sponge(x, y, z + eps, &dummy) - d;

    float mag = sqrtf((*nx)*(*nx) + (*ny)*(*ny) + (*nz)*(*nz));
    if (mag > 0.0f) { *nx /= mag; *ny /= mag; *nz /= mag; }
}

int main() {
    uint8_t *image = (uint8_t *)malloc(WIDTH * HEIGHT * 3);
    if (!image) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Generating Hyper-Complex Colorful Fractal Landscape... Please stand by.\n");

    // Static direction vector for ambient environment sun
    float sun_x = 0.57f, sun_y = 0.57f, sun_z = -0.57f;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Normalize current pixel coordinate systems (-1.0 to 1.0)
            float px = (2.0f * x - WIDTH) / HEIGHT;
            float py = (2.0f * y - HEIGHT) / HEIGHT;

            // Camera System Vectors
            float ro_x = 1.5f, ro_y = 1.2f, ro_z = -4.0f; // Origin position
            float rd_x = px,   rd_y = py,   rd_z = 1.2f;  // Target view projection direction

            // Normalize ray vector length
            float mag = sqrtf(rd_x*rd_x + rd_y*rd_y + rd_z*rd_z);
            rd_x /= mag; rd_y /= mag; rd_z /= mag;

            float r = 0, g = 0, b = 0;
            float color_idx = 0.0f;

            float t = raymarch(ro_x, ro_y, ro_z, rd_x, rd_y, rd_z, &color_idx);

            if (t > 0.0f) {
                // Ray intersection found! Compute coordinate maps
                float hit_x = ro_x + rd_x * t;
                float hit_y = ro_y + rd_y * t;
                float hit_z = ro_z + rd_z * t;

                float nx, ny, nz;
                get_normal(hit_x, hit_y, hit_z, &nx, &ny, &nz);

                // Basic directional lighting calculation
                float diff = nx * sun_x + ny * sun_y + nz * sun_z;
                if (diff < 0.1f) diff = 0.1f; // Ambient visibility floor

                // Generate rich color hues based on fractal structural depth calculations
                float hue = fmodf(color_idx * 90.0f + t * 15.0f, 360.0f);
                float sat = 0.85f; // High rich saturation level
                float light = 0.5f * diff + 0.1f; // Brightness mapping scaled to sun

                hsl_to_rgb(hue, sat, light, &r, &g, &b);

                // Add exponential color fading across depth distances
                float fog = 1.0f - expf(-0.06f * t);
                r = r * (1.0f - fog) + 20.0f * fog; // Fade deep shadows out to black background
                g = g * (1.0f - fog) + 10.0f * fog;
                b = b * (1.0f - fog) + 30.0f * fog;
            } else {
                // Space Background: Dark psychedelic plasma gradient lines
                float radial = sqrtf(px * px + py * py);
                r = fmaxf_custom(0.0f, 40.0f - radial * 20.0f);
                g = fmaxf_custom(0.0f, 15.0f - radial * 10.0f);
                b = fmaxf_custom(0.0f, 60.0f - radial * 30.0f);
            }

            // Write RGB channels into main array matrix tracking heap
            int index = (y * WIDTH + x) * 3;
            image[index]     = (uint8_t)(b > 255.0f ? 255.0f : (b < 0.0f ? 0.0f : b));
            image[index + 1] = (uint8_t)(g > 255.0f ? 255.0f : (g < 0.0f ? 0.0f : g));
            image[index + 2] = (uint8_t)(r > 255.0f ? 255.0f : (r < 0.0f ? 0.0f : r));
        }
    }

    // Export structural system heap array memory into file system container block
    FILE *f = fopen("complex_fractal_landscape.bmp", "wb");
    if (!f) {
        free(image);
        return 1;
    }

    int pad_size = (4 - (WIDTH * 3) % 4) % 4;
    int image_size = (WIDTH * 3 + pad_size) * HEIGHT;
    int file_size = FILE_HEADER_SIZE + INFO_HEADER_SIZE + image_size;

    unsigned char file_header[FILE_HEADER_SIZE] = {
        'B', 'M', 
        (unsigned char)(file_size), (unsigned char)(file_size >> 8), 
        (unsigned char)(file_size >> 16), (unsigned char)(file_size >> 24),
        0, 0, 0, 0, 
        FILE_HEADER_SIZE + INFO_HEADER_SIZE, 0, 0, 0
    };

    unsigned char info_header[INFO_HEADER_SIZE] = {
        INFO_HEADER_SIZE, 0, 0, 0, 
        (unsigned char)(WIDTH), (unsigned char)(WIDTH >> 8), (unsigned char)(WIDTH >> 16), (unsigned char)(WIDTH >> 24),
        (unsigned char)(HEIGHT), (unsigned char)(HEIGHT >> 8), (unsigned char)(HEIGHT >> 16), (unsigned char)(HEIGHT >> 24), 
        1, 0, 24, 0,
        0, 0, 0, 0, 
        (unsigned char)(image_size), (unsigned char)(image_size >> 8), (unsigned char)(image_size >> 16), (unsigned char)(image_size >> 24),
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    fwrite(file_header, 1, FILE_HEADER_SIZE, f);
    fwrite(info_header, 1, INFO_HEADER_SIZE, f);
    unsigned char pad[3] = {0, 0, 0};

    for (int y = 0; y < HEIGHT; y++) {
        fwrite(&image[y * WIDTH * 3], 1, WIDTH * 3, f);
        if (pad_size > 0) {
            fwrite(pad, 1, pad_size, f);
        }
    }

    fclose(f);
    free(image);
    printf("Successfully rendered! Output file saved to complex_fractal_landscape.bmp\n");
    return 0;
}
