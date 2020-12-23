#include <cs50.h>
#include <stdio.h>
#include <string.h>

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
    for (int k = 0; k < candidate_count; k++)
    {
        if (!strcmp(candidates[k], name))
        {
            ranks[rank] = k;
            //for (int l = 0; l < candidate_count; l++)
            //{
            //    printf("%i", ranks[l]);
            //}
            //printf("\n");
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = k; l < candidate_count; l++)
        {
            if (k < l)
            {
                preferences[ranks[k]][ranks[l]]++;
            }
        }
    }
    //for (int k = 0; k < candidate_count; k++)
    //{
    //    for (int l = 0; l < candidate_count; l++)
    //    {
    //        printf("%i", preferences[k][l]);
    //    }
    //}
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = k + 1; l < candidate_count; l++)
        {
            if (preferences[k][l] > preferences[l][k])
            {
                pairs[pair_count].winner = k;
                pairs[pair_count].loser = l;
                pair_count++;
            }
            else if (preferences[k][l] < preferences[l][k])
            {
                pairs[pair_count].winner = l;
                pairs[pair_count].loser = k;
                pair_count++;
            }
        }
    }
    for (int k = 0; k < pair_count; k++)
    {
        printf("Winner: %i and Loser: %i\n", pairs[k].winner, pairs[k].loser);
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int x = 0;
    int margin[pair_count];
    for (int k = 0; k < candidate_count; k++)
    {
        for (int l = k + 1; l < candidate_count; l++)
        {
            if (preferences[k][l] > preferences[l][k])
            {
                margin[x] = preferences[k][l] - preferences[l][k];
                x++;
            }
            else if (preferences[k][l] < preferences[l][k])
            {
                margin[x] = preferences[l][k] - preferences[k][l];
                x++;
            }
        }
    }
    
    /*for (int k = 0; k < pair_count; k++)
    {
        printf("Margin: %i\n", margin[k]);
    }*/
    
    for (int rep = 0; rep < pair_count; rep++)
    {
        for (int k = 0; k < pair_count - 1; k++)
        {
            if (margin[k] < margin[k + 1])
            {
                int winner = pairs[k].winner;
                int loser = pairs[k].loser;
                int margin_dummy = margin[k];
                
                pairs[k].winner = pairs[k + 1].winner;
                pairs[k].loser = pairs[k + 1].loser;
                margin[k] = margin[k + 1];
                
                pairs[k + 1].winner = winner;
                pairs[k + 1].loser = loser;
                margin[k + 1] = margin_dummy;
            }
        }
    }
    /*for (int k = 0; k < pair_count; k++)
    {
        printf("Winner %i over Loser %i by margin: %i\n", pairs[k].winner, pairs[k].loser, margin[k]);
    }*/
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    for (int k = 0; k < pair_count; k++)
    {
        locked[pairs[k].winner][pairs[k].loser] = true;
        
        int candidates_source = 0;
        for (int m = 0; m < candidate_count; m++)
        {
            int count = 0;
            for (int n = 0; n < candidate_count; n++)
            {
                if (locked[n][m] == false)
                {
                    count++;
                }
                if (count == candidate_count)
                {
                    candidates_source++;
                    //printf("%i", candidates_source);
                }
            }
        }
        if (candidates_source == 0)
        {
            //printf("We have a cycle\n");
            locked[pairs[k].winner][pairs[k].loser] = false;
        }
    }
    
    /*for (int k = 0; k < candidate_count; k++)
    {
        for (int l = 0; l < candidate_count; l++)
        {
            printf("%d", locked[k][l]);
        }
    }*/
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int k = 0; k < candidate_count; k++)
    {
        int count = 0;
        for (int l = 0; l < candidate_count; l++)
        {
            if (locked[l][k] == false)
            {
                count++;
            }
            if (count == candidate_count)
            {
                printf("\n%s\n", candidates[k]);
            }
        }
    }
    return;
}

