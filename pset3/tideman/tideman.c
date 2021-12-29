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
int value(pair testpair);

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
        if (strcmp(name,candidates[i]) == 0)
        {
            ranks[rank] = i;
            //printf("%d\n",ranks[rank]);
            return true;
        }
    }
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (i < j)
            {
                preferences[ranks[i]][ranks[j]] += 1;

            }

        }


    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    for (int i = 0 ; i < candidate_count; i++)
    {
        for (int j = 0 ; j < candidate_count; j++)
        {
            if ((preferences[i][j] > 0) && (preferences[i][j] > preferences[j][i])) // checking that they are not tied
            {
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                //printf("%d%d\n",i,j);
                pair_count++;
            }
        }
    }

    /*
    for (int i = 0 ; i < pair_count; i++)
    {
        printf("%d%d%d\n", pairs[i].winner,pairs[i].loser,value(pairs[i]));
    }
    */
    return;
}
/*
pair recursive_mergeSort(int n, pair pairs[]) //n is number of pairs
{
    pair L[n+1];
    pair R[n+1];

    pair sorted[n];
    if (n == 1)
    {
        return pairs[0];
    }
    else
    {
        if (n%2==0)
        {
            //add to L
            for (int i = 0;i < (n/2)-1;i++)
            {
                L[i] = pairs[i];
            }

            //add to R
            int counter = 0;
            for (int i = (n/2); i < n; i++)
            {
                R[counter] = pairs[i];
                counter++;
            }


            for(int i = 0;i < n; i++)
            {

                largestL = preferences[L[i].winner] - preferences[L[i].loser];
                largestR = preferences[R[i].winner] - preferences[R[i].loser];
                if ( largestL > largestR)
                {
                    sorted[i] = L[i];
                }
                else
                {
                    sorted[i] = R[i];
                }

            }

            return sorted;

        }
    }


}

*/
// Sort pairs in decreasing order by strength of victory

int value(pair testpair)
{
    return preferences[testpair.winner][testpair.loser];
}
void sort_pairs(void)
{
    //BUBBLESORTED LMAO
    // sorted_pairs;
    //int largest = pairs[0].winner - pairs[0].loser;
    bool swapped = false;
    do
    {
        swapped = false;
        for (int i = 1 ; i < pair_count; i++)
        {
            //printf("\n\n%d\n\n",value(pairs[i]));
            if (value(pairs[i]) > value(pairs[i-1]))
            {
                pair temp = pairs[i];
                pairs[i] = pairs[i-1];
                pairs[i-1] = temp;
                swapped = true;
            }
        }

    } while (swapped == true);

    /*
    for (int i = 0 ; i < pair_count; i++)
    {
        printf("%d%d%d\n", pairs[i].winner,pairs[i].loser,value(pairs[i]));
    }
    */
    return;
}

bool find_cycle(int winner, int loser, int original)
{
    //printf("%d%d\n" , winner,loser);
    if (loser == original) //figure out base case??
    {
        return true;
    }
    else
    {

        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[loser][i] == true)
            {
                if (find_cycle(loser,i,original) == true)
                {
                    return true;
                }

            }
        }
        return false;

    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // TODO
    locked[pairs[0].winner][pairs[0].loser] = true;
    for (int i = 0; i < pair_count; i++) //SOMEHOW REMOVING THE = SIGN BREAKS ONE CHECK FUNCTION?? originally was (int i = 1; i <= pair_count; i++)
    {
        if (find_cycle(pairs[i].winner,pairs[i].loser,pairs[i].winner) == false)
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }

        /*
        if (find_cycle(pairs[i].winner,pairs[i].loser,pairs[i].winner) == true)
        {
            printf("%d%d\n", pairs[i].winner,pairs[i].loser);
        }
        */

    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    int possible_winners[candidate_count];
    int winner_count = 0;
    for (int i = 0; i < candidate_count; i++)
    {
        int counter = 0;
        for (int j = 0; j < candidate_count;j++)
        {
            if (locked[j][i] == true)
            {
                counter++;
            }
        }
        if (counter == 0)
        {
            printf("%s\n", candidates[i]);
            //possible_winners[winner_count] = i;
            //winner_count++;
        }
    }

    return;
}

