#include <math.h>
#include <stdio.h>
const int ancho_pantalla = 100;
const int alto_pantalla = 100;

const float teta_step = 0.07;
const float phi_step = 0.02;

const float R1 = 1;
const float R2 = 2;

const float K2 = 5;
const float K1 = ancho_pantalla*K2*3/(8*(R1+R2));

void calcular_frame(float A, float B) {
    float cosA = cos(A), sinA = sin(A);
    float cosB = cos(B), sinB = sin(B);

    char salida[ancho_pantalla][alto_pantalla];
    float zbuffer[ancho_pantalla][alto_pantalla];

    for (int i = 0; i < ancho_pantalla; i++) {
        for (int j = 0; j < alto_pantalla; j++) {
            salida[i][j] = ' ';
            zbuffer[i][j] = 0;
        }
    }

    for (float teta = 0; teta < 2*M_PI; teta += teta_step) {
        float cosTeta = cos(teta), sinTeta = sin(teta);
        for (float phi = 0; phi < 2*M_PI; phi += phi_step ) {
            float cosPhi = cos(phi), sinPhi = sin(phi);

            float circlex = R2 + R1 * cosTeta;
            float circley = R1 * sinTeta;

            float x = circlex * (cosB * cosPhi + sinA * sinB * sinPhi) - circley * cosA * sinB;
            float y = circlex * (sinB * cosPhi - sinA * cosB * sinPhi) + circley * cosA * cosB;
            float z = K2 + cosA * circlex * sinPhi + circley * sinA;

            float inversaDz = 1/z;

            int projeccionX = (int) (ancho_pantalla/2 + K1 * inversaDz * x);
            int projeccionY = (int) (alto_pantalla/2 - K1 * inversaDz * y);

            float L = cosPhi * cosTeta * sinB - cosA * cosTeta * sinPhi - sinA * sinTeta + cosB * (cosA * sinTeta - cosTeta * sinA * sinPhi);

            if (L > 0) {

                if (inversaDz > zbuffer[projeccionX][projeccionY]) {
                    zbuffer[projeccionX][projeccionY] = inversaDz;
                    int indiceDeLuminancia = L * 8;
                    salida[projeccionX][projeccionY] = ".,-~:;=!*#$@"[indiceDeLuminancia];
                }
            }
        }
    }

    printf("\x1b[H");
    for (int j = 0; j < alto_pantalla; j++) {
        for (int i = 0; i < ancho_pantalla; i++) {
            putchar(salida[i][j]);
        }
        putchar('\n');
    }
}