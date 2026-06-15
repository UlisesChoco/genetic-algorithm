#include "raylib.h"
#include <time.h>
#include <stdlib.h>
#include <stdio.h>

#define WINDOW_WIDTH    1200
#define WINDOW_HEIGHT   720

#define GENES                "RULD"
#define GENES_QUANTITY       4
#define MAX_GENES            4
#define MUTATION_PROBABILITY 0.03

#define POBLATION       1000
#define POBLATION_SPEED 1.5
#define POBLATION_SIZE  5.0

struct Individual {
    float x;
    float y;
    char genes[MAX_GENES];
    int can_breed;
};

void populate_poblation(struct Individual population[POBLATION]) {
    for (int i = 0 ; i < POBLATION ; i++) {
        struct Individual individual = { };
        individual.x = 0 + rand() % WINDOW_WIDTH;
        individual.y = 0 + rand() % WINDOW_HEIGHT;

        for (int j = 0 ; j < MAX_GENES ; j++) {
            int selected_gene = 0 + rand() % GENES_QUANTITY;

            individual.genes[j] = GENES[selected_gene];
        }

        population[i] = individual;
    }
}

void execute_gene(struct Individual population[POBLATION], int current_gene) {
    for (int i = 0 ; i < POBLATION ; i++) {
        char gene = population[i].genes[current_gene];
        if (gene == 'R' && population[i].x + POBLATION_SPEED < WINDOW_WIDTH) {
            population[i].x += POBLATION_SPEED;
        } else if (gene == 'U' && population[i].y - POBLATION_SPEED > 0) {
            population[i].y -= POBLATION_SPEED;
        } else if (gene == 'L' && population[i].x - POBLATION_SPEED > 0) {
            population[i].x -= POBLATION_SPEED;
        } else if (gene == 'D' && population[i].y + POBLATION_SPEED < WINDOW_HEIGHT) {
            population[i].y += POBLATION_SPEED;
        }

        DrawCircle(population[i].x, population[i].y, POBLATION_SIZE, RAYWHITE);
    }
}

int determine_who_can_breed(struct Individual population[POBLATION]) {
    int survivors_size = 0;
    for (int i = 0 ; i < POBLATION ; i++) {
        if (population[i].x < WINDOW_WIDTH / 4) {
            survivors_size++;
            population[i].can_breed = 1;
        } else {
            population[i].can_breed = 0;
        }
    }
    return survivors_size;
}

void generate_next_generation(struct Individual population[POBLATION], int survivors_size) {
    struct Individual survivors[survivors_size];
    int j = 0;
    for (int i = 0 ; i < POBLATION ; i++) {
        if (population[i].can_breed == 1) {
            survivors[j] = population[i];
            j++;
        }
    }

    for (int i = 0 ; i < POBLATION ; i++) {
        printf("--- child number %d ---\n", i);
        int genes_assigned = 0;

        struct Individual father = survivors[rand() % survivors_size];
        struct Individual mother = survivors[rand() % survivors_size];

        struct Individual child = { };
        child.x = 0 + rand() % WINDOW_WIDTH;
        child.y = 0 + rand() % WINDOW_HEIGHT;
        for (int j = 0 ; j <= (MAX_GENES / 2) - 1 ; j++) {
            float mutation_roll = (float) rand() / RAND_MAX;

            if (mutation_roll < MUTATION_PROBABILITY) {
                int selected_gene = 0 + rand() % GENES_QUANTITY;
                child.genes[j] = GENES[selected_gene];
                printf("mutated gen -> %c\n", child.genes[j]);
            } else {
                child.genes[j] = father.genes[j];
                printf("assigned gen -> %c\n", child.genes[j]);
            }
            
            genes_assigned++;
        }
        for (int j = MAX_GENES / 2 ; j < MAX_GENES ; j++) {
            float mutation_roll = (float) rand() / RAND_MAX;

            if (mutation_roll < MUTATION_PROBABILITY) {
                int selected_gene = 0 + rand() % GENES_QUANTITY;
                child.genes[j] = GENES[selected_gene];
                printf("mutated gen -> %c\n", child.genes[j]);
            } else {
                child.genes[j] = mother.genes[j];
                printf("assigned gen -> %c\n", child.genes[j]);
            }
            
            genes_assigned++;
        }

        printf("genes_assigned = %d\n", genes_assigned);
        population[i] = child;
    }
}

int main(void) {
    srand(time(NULL));

    struct Individual population[POBLATION];

    populate_poblation(population);

    int generation = 0;
    int frames = 0;
    int current_gene = 0;
    int paused = 0;
    int elapsed_time_from_pause = 0;
    int survivors_size = 0;

    char generation_message[100];
    char survivors_message[250];

    sprintf(generation_message, "generation = %d", generation);
    sprintf(survivors_message, "survivors = %d\ndead = %d", survivors_size, POBLATION - survivors_size);

    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "genetic algorithm");
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
            ClearBackground(BLACK);

            DrawLine(WINDOW_WIDTH / 4, 0, WINDOW_WIDTH / 4, WINDOW_HEIGHT, RAYWHITE);

            DrawText(generation_message, 0, 0, 20, RAYWHITE);
            DrawText(survivors_message, 0, 20, 20, RAYWHITE);

            if (paused == 0) execute_gene(population, current_gene);

            if (frames >= (WINDOW_WIDTH) / (MAX_GENES * POBLATION_SPEED)) {
                frames = 0;
                if (current_gene < MAX_GENES) current_gene++;
            }

            if (current_gene == MAX_GENES) {
                paused = 1;
                survivors_size = determine_who_can_breed(population);
                sprintf(survivors_message, "survivors = %d\ndead = %d", survivors_size, POBLATION - survivors_size);
                for (int i = 0 ; i < POBLATION ; i++) {
                    Color assigned_color = (population[i].can_breed == 1) ? GREEN : RED;
                    DrawCircle(population[i].x, population[i].y, POBLATION_SIZE, assigned_color);
                }
            }

            if (paused == 1) {
                if (elapsed_time_from_pause == 60) {
                    generate_next_generation(population, survivors_size);
                    generation++;
                    current_gene = 0;
                    paused = 0;
                    elapsed_time_from_pause = 0;
                    sprintf(generation_message, "generation = %d", generation);
                }
                elapsed_time_from_pause++;
            }

            frames++;
        EndDrawing();
    }

    CloseWindow();

    return 0;
}