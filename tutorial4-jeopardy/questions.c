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
#include "questions.h"

// Initializes the array of questions for the game
void initialize_game(void)
{
    // initialize each question struct and assign it to the questions array
    for (int i = 0; i < NUM_CATEGORIES; i++)
    {
        for (int j = 0; j < NUM_QUESTIONS / NUM_CATEGORIES; j++)
        {
            // assign the category and value for each question
            question *q = &(questions[(i * (NUM_QUESTIONS / NUM_CATEGORIES)) + j]);
            strcpy(q->category, categories[i]);
            strcpy(q->question, questionsstrings[i][j]);
            strcpy(q->answer, answerstrings[i][j]);
            q->value = (j + 1) * 100;
            q->answered = false;
        }
    }
}

// Displays each of the remaining categories and question dollar values that have not been answered
void display_categories(void)
{
    printf("Remaining Categories and Question Dollar Values:\n");
    printf("-----------------------------------------------\n");
    printf("| Category        | Dollar Value | Answered |\n");
    printf("-----------------------------------------------\n");

    // print categories, dollar values, and answered status for each question in questions array
    for (int i = 0; i < NUM_QUESTIONS; i++)
    {
        printf("| %-15s | $%-12d | %s      |\n", questions[i].category, questions[i].value, questions[i].answered ? "[O]" : "[X]");
    }

    printf("-----------------------------------------------\n");
}

// Displays the question for the category and dollar value
void display_question(char *category, int value)
{
    for (int i = 0; i < NUM_QUESTIONS; i++)
    {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value)
        {
            printf("%s\n", questions[i].question);
            return;
        }
    }
    printf("Question not found with %s %d.", category, value);
}

// Returns true if the answer is correct for the question for that category and dollar value
bool valid_answer(char *category, int value, char *answer)
{
    // Look into string comparison functions
    for (int i = 0; i < NUM_QUESTIONS; i++)
    {
        // printf("Checking category \"%s\" == \"%s\"\n", questions[i].category, category);
        // printf("Checking if %d == %d\n", questions[i].value, value);
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value)
        {
            // printf("Checking answer \"%s\" == \"%s\"\n", questions[i].answer, answer);
            if (strcmp(questions[i].answer, answer) == 0)
            {
                return true;
            }
        }
    }
    return false;
}

// Returns true if the question has already been answered
bool already_answered(char *category, int value)
{
    // lookup the question and see if it's already been marked as answered
    for (int i = 0; i < NUM_QUESTIONS; i++)
    {
        if (strcmp(questions[i].category, category) == 0 && questions[i].value == value)
        {
            if (questions[i].answered == true)
            {
                return true;
            }
        }
    }
    return false;
}
