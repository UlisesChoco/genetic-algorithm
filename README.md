# Genetic Algorithm Playground

A small project I built to learn how genetic algorithms work from scratch.

The goal wasn't to write the most optimized or elegant implementation possible, but to understand the concepts by implementing everything myself and watching the simulation evolve in real time.

## Demo



## How it works

Each individual in the population contains a very small "DNA" represented by a sequence of genes:

```text
R U L D
```

Where:

* `R` → Move Right
* `U` → Move Up
* `L` → Move Left
* `D` → Move Down

Every generation:

1. A population is randomly generated.
2. Each individual executes its genes.
3. Individuals that reach the survival area are marked as survivors.
4. Survivors are selected as possible parents.
5. A new generation is created through crossover.
6. Each inherited gene has a small probability of mutation.
7. The process repeats forever.

The current implementation intentionally keeps the genome extremely simple so the evolutionary process can be easily visualized.

## Current parameters

```c
Population size:        1000
Genes per individual:   4
Mutation probability:   3%
```

The values are defined as compile-time constants and can be modified directly in the source code.

## Visualization

The simulation displays:

* Current generation
* Number of survivors
* Number of eliminated individuals

When a generation finishes:

* 🟢 Green individuals survived and can reproduce.
* 🔴 Red individuals are eliminated.

After a short pause, a new generation is created and the simulation continues.

## Building

This project uses **Raylib** for rendering.

Raylib is **not included** in this repository.

To compile the project, make sure Raylib is installed (or otherwise available in your build environment) and link against it when compiling.

Example (GCC):

```sh
gcc main.c -lraylib
```

The exact compilation command depends on your compiler, operating system and Raylib installation.

## Why?

Mostly because I thought it would be fun.

This project was written as an experiment to understand genetic algorithms by implementing one from scratch instead of reading about them.

It's intentionally simple, heavily iterative, and likely full of things I'll want to improve in the future.