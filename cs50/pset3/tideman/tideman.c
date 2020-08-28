#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

//int prediff[MAX * (MAX - 1) / 2];

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;

    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }
        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Checking each character to see if it's the same as one of the candidates
    int namelen = strlen(name);
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < namelen; j++)
        {
            if (name[j] != candidates[i][j])
            {
                break;
            }
            else
            {
                //if it's the last character, go ahead and add the person!
                if (j == namelen - 1)
                {
                    ranks[rank] = i;
                    return true;
                }
            }
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // this was done with two loops, the second checks for every candidate after the ith in ranks
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            if (ranks[i] != candidate_count + 1 && ranks[j] != candidate_count + 1)
            {
                // +1 for each rank after rank i. j is always one or more ahead of i
                preferences[ranks[i]][ranks[j]] += 1;
            }
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    pair_count = 0;
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // who is preferred depends on which is greater in the "opposite" position in the matrix
            if (preferences[i][j] > preferences[j][i])
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count += 1;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                pair_count += 1;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    pair temp[pair_count];
    bool tprediff[pair_count];
    int k;
    int greatest;
    // n^2 solution... but it works. Check for greatest differences and sort by them
    for (int i = 0; i < pair_count; i++)
    {
        greatest = 0;
        for (int j = 0; j < pair_count; j++)
        {
            if (preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner] > greatest && tprediff[j] != 1)
            {
                greatest = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
                k = j;
            }
        }
        tprediff[k] = 1;
        temp[i] = pairs[k];
    }
    for (int i = 0; i < pair_count; i++)
    {
        pairs[i] = temp[i];
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int check[candidate_count];
    bool bcheck;
    bool losecheck[candidate_count], wincheck[candidate_count];
    int checkcount = 0;
    // hardcoded the first two since a cycle won't be created with only 2
    locked[pairs[0].winner][pairs[0].loser] = true;
    locked[pairs[1].winner][pairs[1].loser] = true;
    // checks to see if a candidate has won or lost at any point
    losecheck[pairs[0].loser] = true;
    losecheck[pairs[1].loser] = true;
    wincheck[pairs[0].winner] = true;
    wincheck[pairs[1].winner] = true;
    for (int i = 2; i < pair_count; i++)
    {
        // skips if the winner has lost and the loser has won at some point
        if (losecheck[pairs[i].winner] && wincheck[pairs[i].loser]) {}
        // otherwise locks the pair and makes record of the win and loss
        else
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
            losecheck[pairs[i].loser] = true;
            wincheck[pairs[i].winner] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (locked[j][i])
            {
                break;
            }
        }
        // print any candidate with all false in their locked column
        printf("%s\n", candidates[i]);
    }
    return;
}

