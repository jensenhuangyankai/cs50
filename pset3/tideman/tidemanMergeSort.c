#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

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

// Sort pairs in decreasing order by strength of victory

int value(pair testpair)
{
    return preferences[testpair.winner][testpair.loser];
}


void merge(pair array[], int begin, int mid, int end, int size)
{
    
    //pair arrayCopy[size*sizeof(pair)];
    //memcpy(arrayCopy, array, size);
    /*
    for (int i = 0 ; i < size; i++)
    {
        printf("%d%d%d\n", arrayCopy[i].winner,arrayCopy[i].loser,value(arrayCopy[i]));
        printf("%i\n", i);
    }
    */
    /*
    pair left[(int)ceil(size/2)];
    int leftLength = mid - begin + 1;
    memcpy(left,arrayCopy, (floor(size/2)) * sizeof(pair));

    pair right[(int)ceil(size/2)];
    int rightLength = end - mid;
    memcpy(right, arrayCopy + (int)(ceil(size/2)), (floor(size/2)) * sizeof(pair));
    */
    //help me i hate mergesort

    //copy the left right into their respective arrays
    int leftSize = (mid-begin)+1;
    int rightSize = (end-mid);

    pair left[leftSize];
    pair right[rightSize];

    for (int i = 0; i<leftSize; i++)
    {
        left[i] = array[begin+i];
    }
    for (int i = 0; i < rightSize;i++)
    {
        right[i] = array[mid+i + 1];
    }

    for (int i = 0 ; i < leftSize; i++)
    {
        printf("left");
        printf("%d%d%d\n", left[i].winner,left[i].loser,value(left[i]));
        //printf("%i\n", i);
    }
    for (int i = 0 ; i < rightSize; i++)
    {
        printf("right");
        printf("%d%d%d\n", right[i].winner,right[i].loser,value(right[i]));
        //printf("%i\n", i);
    }




    int indexLeft = 0;
    int indexRight = 0;
    int mergedIndex = begin;

    while (indexLeft > 0 && indexRight > 0)
    {
        if (left[indexLeft] > right[indexRight])
        {
            array[mergedIndex] = right[indexRight];
            mergedIndex++;
            indexRight++;
        }
        else
        {
            array[mergedIndex] = left[indexLeft];
            mergedIndex++;
            indexLeft++;
        }
    }

    printf("original");
    for (int i = 0 ; i < size; i++)
    {
        printf("%d%d%d\n", array[i].winner,array[i].loser,value(array[i]));
        //printf("%i\n", i);
    }




}


void mergeSort(pair array[], int begin, int end,int size)
{
    if (size == 1) return;
    
    int mid = begin + (end-begin)/2;

    mergeSort(array,begin,mid,floor(size/2));
    mergeSort(array,mid + 1,end,ceil(size/2));
    merge(array, begin, mid, end,size);
    


    /*
    for (int i = 0 ; i < ceil(size/2); i++)
    {
        printf("%d%d%d\n", left[i].winner,left[i].loser,value(left[i]));
        printf("%d%d%d\n", right[i].winner,right[i].loser,value(right[i]));
        //printf("%i", i);
    }
    */
        



}

void sort_pairs(void)
{   
    /*
    for (int i = 0 ; i < pair_count; i++)
    {
        printf("%d%d%d\n", pairs[i].winner,pairs[i].loser,value(pairs[i]));
    }
    */
    //printf("lmaos\n");
    mergeSort(pairs, 0, pair_count-1, pair_count);




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

