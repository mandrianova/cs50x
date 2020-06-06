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
int get_strength(int pair);
int check_cycle(void);

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
    for (int i = 0; i < candidate_count; i++)
    {
        if (candidates[i] > 0 && strcmp(candidates[i], name) == 0) //Find out candidate with equal name for voting
        {
            ranks[rank] = i;
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count - 1; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            preferences[ranks[i]][ranks[j]]++; //add the current voter’s preferences, where i = candidate with the better rank, then candidate j (the next ranks)
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0; i < candidate_count; i ++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (preferences[i][j] - preferences[j][i] > 0) // difference between win and lose for i candidate and j candidate
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                pair_count++;
            }
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    int tmp[2];
    for (int i = 0; i < pair_count; i++)
    {
        for (int j = 0; j < pair_count - 1; j++)
        {
            if (get_strength(j) < get_strength(j + 1))
            {
                tmp[0] = pairs[j].winner;
                tmp[1] = pairs[j].loser;
                pairs[j].winner = pairs[j + 1].winner;
                pairs[j].loser = pairs[j + 1].loser;
                pairs[j + 1].winner = tmp[0];
                pairs[j + 1].loser = tmp[1];
            }
        }
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    int losers = 0;
    int n = check_cycle();
    for (int pair = 0; pair < pair_count; pair++)
    {
        locked[pairs[pair].winner][pairs[pair].loser] = true; // add the next pair
        if (pair == n)
        {
            locked[pairs[pair].winner][pairs[pair].loser] = false;
        }
        losers = 0;
        for (int i = 0; i < candidate_count; i++) //recount losers after add new pair
        {
            int wins = 0;
            for (int j = 0; j < candidate_count; j++) // count loseless points for each candidate
            {
                if (!(locked[j][i])) //i candidate doesn't lose for j candidate
                {
                    wins++;
                }
            }
            if (wins < candidate_count) // i candidate have one or more lose
            {
                losers++;
            }
        }
        if (losers == candidate_count) // stop loop for cycle
        {
            locked[pairs[pair].winner][pairs[pair].loser] = false;
            return;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    for (int i = 0; i < candidate_count; i++)
    {
        int wins = 0;
        for (int j = 0; j < candidate_count; j++)
        {
            if (!(locked[j][i]))
            {
                wins++;
            }
        }
        if (wins == candidate_count)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}

int get_strength(int pair)
{
    return preferences[pairs[pair].winner][pairs[pair].loser] - preferences[pairs[pair].loser][pairs[pair].winner];
}

int pair_cycle = 0;
int check_cycle(void) //try to find pair which create cycle
{
    for (int pair = 0; pair < pair_count; pair++)
    {
        int winner = pairs[pair].winner;
        int loser = pairs[pair].loser;
        while (winner != loser && pair_cycle != pair_count)
        {
            for (int i = 0; i < pair_count; i++)
            {
                if (loser == pairs[i].winner)
                {
                    loser = pairs[i].loser;
                    break;
                }
            }
            pair_cycle++;
        }
        if (winner == loser)
        {
            return pair_cycle;
        }
    }
    return pair_count;
}
