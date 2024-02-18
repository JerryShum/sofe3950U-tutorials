/*
 * Tutorial 4 Jeopardy Project for SOFE 3950U / CSCI 3020U: Operating Systems
 *
 * Copyright (C) 2015, <GROUP MEMBERS>
 * All rights reserved.
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "questions.h"
#include "players.h"
#include "jeopardy.h"

// Put macros or constants here using #define
#define BUFFER_LEN 256
#define NUM_PLAYERS 4

// Put global environment variables here

// Processes the answer from the user containing what is or who is and tokenizes it to retrieve the answer.
void tokenize(char *input, char **tokens)
{
    return;
}

// Displays the game results for each player, their name and final score, ranked from first to last place
void show_results(player *players, int num_players)
{
    // Display the final results
    for (int i = 0; i < num_players; i++)
    {
        printf("Player: %s, Score: %d\n", players[i].name, players[i].score);
    }
}


int main(int argc, char *argv[])
{
    // An array of 4 players, may need to be a pointer if you want it set dynamically
    player players[NUM_PLAYERS];

    // Input buffer and and commands
    char buffer[BUFFER_LEN] = { 0 };

    // Display the game introduction and initialize the questions
    initialize_game();

    // Prompt for players names
    for(int i = 0; i < NUM_PLAYERS; i++)
    {
        memset(buffer, 0, sizeof(char) * BUFFER_LEN);
        printf("Enter the name of player %d: ", i+1);
        fgets(buffer, BUFFER_LEN, stdin);
        char *newline = strchr(buffer, '\n');
        if (newline != NULL)
        {
            *newline = '\0';
        }
        strcpy(players[i].name, buffer);
    }

    // initialize each of the players in the array
    for (int i = 0; i < NUM_PLAYERS; i++)
    {
        players[i].score = 0;
    }

    int currplayer = 0;
    int playing = 1;

    // Perform an infinite loop getting command input from users until game ends
    initialize_game();
    while (playing == 1)
    {
        // Call functions from the questions and players source files
        for(int i = 0; i < NUM_PLAYERS; i++) {
            display_categories();
            printf("Player %s, it's your turn! Pick a question (e.g. \'programming,400\').\n", players[i].name);
            memset(buffer, 0, sizeof(char) * BUFFER_LEN);
            fgets(buffer, BUFFER_LEN, stdin);
            char *copy = strdup(buffer);
            char *category = strtok(copy, ",");
            int value = atoi(strtok(NULL, ","));
            display_question(category, value);


            memset(buffer, 0, sizeof(char) * BUFFER_LEN);
            fflush(stdin); // Clear the input buffer
            fgets(buffer, BUFFER_LEN, stdin);
            char *answer = strdup(buffer);
            char *nl = strchr(answer, '\n');
            if(nl != NULL) {
                *nl = '\0';
            }
            printf("Category: %s, Value: %d, Answer: %s\n", category, value, answer);
            if (valid_answer(category, value, answer))
            {
                update_score(players, NUM_PLAYERS, players[i].name, value);
                for(int j = 0; j < NUM_QUESTIONS; j++) {
                    if(strcmp(questions[j].category, category) == 0 && questions[j].value == value) {
                        questions[j].answered = true;
                    }
                }
                printf("Correct!\n\n");
            } else {
                printf("Incorrect!\n\n");
            }
            free(copy);
            free(answer);
            printf("\n");
            show_results(players, NUM_PLAYERS);
            printf("\n");

            // figure out if the game is over
            playing = 0;
            for(int j = 0; j < NUM_QUESTIONS; j++) {
                if(questions[j].answered == false) {
                    playing = 1;
                    break;
                }
            }
        }
    }
    printf("\nGame over.\n");
    return EXIT_SUCCESS;
}
