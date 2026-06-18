#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>

#define WIDTH 3840
#define HEIGHT 2160
#define FILE_HEADER_SIZE 14
#define INFO_HEADER_SIZE 40

// Simple hash function for noise generation
float hash(float n) { 
    return fmodf(sinf(n) * 43758.5453123f, 1.0f); 
}

// 2D Noise function for procedural terrain generation
float noise(float x, float y) {
    float ix = floorf(x);
    float iy = floorf(y);
    float fx = x - ix;
    float fy = y - iy;
    
    // Custom cubic interpolation curve
    float ux = fx * fx * (3.0f - 2.0f * fx);
    float uy = fy * fy * (3.0f - 2.0f * fy);
    
    float n = ix + iy * 57.0f;
    
    float a = hash(n);
    float b = hash(n + 1.0f);
    float c = hash(n + 57.0f);
    float d = hash(n + 58.0f);
    
    return a * (1.0f - ux) * (1.0f - uy) + b * ux * (1.0f - uy) + c * (1.0f - ux) * uy + d * ux * uy;
}

// Fractal Brownian Motion (stacks noise layers for natural jagged textures)
float fbm(float x, float y) {
    float value = 0.0f;
    float amplitude = 0.5f;
    float frequency = 0.05f;
    for (int i = 0; i < 5; i++) {
        value += amplitude * noise(x * frequency, y * frequency);
        frequency *= 2.0f;
        amplitude *= 0.5f;
    }
    return value * 45.0f; // Scale height mapping
}

// Ray-Terrain intersection loop
float intersect(float ro_x, float ro_y, float ro_z, float rd_x, float rd_y, float rd_z, float max_d) {
    float t = 0.1f;
    for (int i = 0; i < 180; i++) {
        float px = ro_x + rd_x * t;
        float py = ro_y + rd_y * t;
        float pz = ro_z + rd_z * t;
        
        float height = fbm(px, pz);
        if (py < height) {
            return t; // Ray hit the mountain geometry
        }
        t += (py - height) * 0.4f; // Safe step sizing
        if (t > max_d) break;
    }
    return -1.0f;
}

// Calculate normal vector of terrain surface for realistic light physics
void get_normal(float x, float z, float *nx, float *ny, float *nz) {
    float eps = 0.1f;
    float h = fbm(x, z);
    float hx = fbm(x + eps, z) - h;
    float hz = fbm(x, z + eps) - h;
    
    float mag = sqrtf(hx * hx + eps * eps + hz * hz);
    *nx = -hx / mag;
    *ny = eps / mag;
    *nz = -hz / mag;
}

int main() {
    uint8_t *image = (uint8_t *)malloc(WIDTH * HEIGHT * 3);
    if (!image) {
        printf("Memory allocation failed!\n");
        return 1;
    }

    printf("Rendering 4K Hyperrealistic Landscape... Please wait.\n");

    // Static environmental sunlight vector
    float sun_x = 0.57f, sun_y = 0.57f, sun_z = -0.57f;

    for (int y = 0; y < HEIGHT; y++) {
        for (int x = 0; x < WIDTH; x++) {
            // Normalized screen coordinates (-1.0 to 1.0)
            float px = (2.0f * x - WIDTH) / HEIGHT;
            float py = (2.0f * y - HEIGHT) / HEIGHT;

            // Camera Setup (Origin and Direction vectors)
            float ro_x = 0.0f, ro_y = 35.0f, ro_z = 0.0f;
            float rd_x = px, rd_y = py - 0.2f, rd_z = 1.5f; 
            
            // Normalize Ray Direction
            float mag = sqrtf(rd_x*rd_x + rd_y*rd_y + rd_z*rd_z);
            rd_x /= mag; rd_y /= mag; rd_z /= mag;

            float r = 0, g = 0, b = 0;

            // Step 1: Render procedural sky background gradient
            float sky_grad = (rd_y + 0.2f) * 0.5f;
            if (sky_grad < 0.0f) sky_grad = 0.0f;
            if (sky_grad > 1.0f) sky_grad = 1.0f;
            
            float sky_r = 135.0f * (1.0f - sky_grad) + 210.0f * sky_grad;
            float sky_g = 180.0f * (1.0f - sky_grad) + 230.0f * sky_grad;
            float sky_b = 240.0f * (1.0f - sky_grad) + 255.0f * sky_grad;

            float max_d = 400.0f;
            float t = intersect(ro_x, ro_y, ro_z, rd_x, rd_y, rd_z, max_d);

            if (t > 0.0f) {
                // Ray hit terrain: Calculate spatial points
                float hit_x = ro_x + rd_x * t;
                float hit_y = ro_y + rd_y * t;
                float hit_z = ro_z + rd_z * t;

                float nx, ny, nz;
                get_normal(hit_x, hit_z, &nx, &ny, &nz);

                // Lambertian diffuse lighting configuration
                float diff = nx * sun_x + ny * sun_y + nz * sun_z;
                if (diff < 0.1f) diff = 0.1f; // Ambient ground bounces

                // Biome texture mapping rules based on angle/height (Snow vs Rock vs Grass)
                float mountain_r = 90.0f, mountain_g = 85.0f, mountain_b = 80.0f; // Basic rock
                
                if (ny > 0.85f && hit_y > 22.0f) { // Caps the peaks in hyper-reflective snow
                    mountain_r = 245.0f; mountain_g = 245.0f; mountain_b = 250.0f;
                } else if (ny > 0.75f) { // Low slope valleys grow green meadows
                    mountain_r = 75.0f;  mountain_g = 110.0f; mountain_b = 60.0f;
                }

                // Render color output mixed with lighting parameters
                r = mountain_r * diff;
                g = mountain_g * diff;
                b = mountain_b * diff;

                // Atmospheric Rayleigh Fog Scattering Effect based on distance
                float fog = 1.0f - expf(-0.005f * t);
                r = r * (1.0f - fog) + sky_r * fog;
                g = g * (1.0f - fog) + sky_g * fog;
                b = b * (1.0f - fog) + sky_b * fog;
            } else {
                // Ray missed: Draw skybox instead
                r = sky_r; g = sky_g; b = sky_b;
            }

            // Write final color bytes to the heap tracking array (Clamped 0-255)
            int index = (y * WIDTH + x) * 3;
            image[index]     = (uint8_t)(b > 255.0f ? 255.0f : (b < 0.0f ? 0.0f : b));
            image[index + 1] = (uint8_t)(g > 255.0f ? 255.0f : (g < 0.0f ? 0.0f : g));
            image[index + 2] = (uint8_t)(r > 255.0f ? 255.0f : (r < 0.0f ? 0.0f : r));
        }
    }

    // Output raw block buffer to uncompressed system BMP container format
    FILE *f = fopen("hyperrealistic_landscape.bmp", "wb");
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
    unsigned char pad[3] = {0, 0, 0}; // Fixed missing brackets
    
    for (int y = 0; y < HEIGHT; y++) {
        fwrite(&image[y * WIDTH * 3], 1, WIDTH * 3, f);
        if (pad_size > 0) {
            fwrite(pad, 1, pad_size, f); // Fixed scalar pointer pass issue
        }
    }

    fclose(f);
    free(image);
    printf("Successfully completed! 4K render saved to hyperrealistic_landscape.bmp\n");
    return 0;
}
