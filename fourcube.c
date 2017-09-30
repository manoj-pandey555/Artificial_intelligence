#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<string.h>
#include<time.h>

//struct which will replicate 4x4 matrix along with pointes to traverse both 
//forward and backward states
struct node{
	int zero_zero;
	int zero_one;
	int one_zero;
	int one_one;
	struct node* parent;
	struct node* lchild;
	struct node* rchild;
};

//co-ordinates of the matrix
enum{
	ZERO_ZERO,
	ZERO_ONE,
	ONE_ZERO,
	ONE_ONE
};

//queue for temporary storage of next states
//it help us to traverse in BFS
struct node *queue[2] = {NULL, NULL};

//Insertion logic in queue
void insert( struct node* temp)
{
	if(queue[0] == NULL)
		queue[0] = temp;
	else if(queue[1] == NULL)
		queue[1] = temp;
	else
		assert(0 && "unable to insert");
}

void swap( int* temp1, int* temp2)
{
	int temp;
	temp = *temp1;
	*temp1 = *temp2;
	*temp2 = temp;
}

int is_goal_found(int *goal_found_at_index, struct node* pgoal)
{
	int i;
	for(i = 0; i < 2; i++)
	{
		if((queue[i]->zero_zero == pgoal->zero_zero)&&(queue[i]->zero_one == pgoal->zero_one)&&(queue[i]->one_zero == pgoal->one_zero)\
			&&(queue[i]->one_one == pgoal->one_one))
		{
			*goal_found_at_index == i;
			return 1;
		}

	}
	
	return 0;
}

int main()
{

	int empty_space, i, temp, goal_found_at_index = 0;
	double start_time, end_time;
	struct node *pproblem, *pgoal, *ptemp = NULL;
	
	//initializing problem node. Hardcoded for testing purpose
	pgoal = malloc(sizeof(struct node));
	pproblem = malloc(sizeof(struct node));
	pproblem->zero_zero = 3;
	pproblem->zero_one = 0;
	pproblem->one_zero = 2;
	pproblem->one_one = 1;
	pproblem->parent = NULL;
	pproblem->lchild = NULL;
	pproblem->rchild = NULL;

	//initializing goal node, hardcoded for testing purpose	
	pgoal->zero_zero = 1;
	pgoal->zero_one = 2;
	pgoal->one_zero = 3;
	pgoal->one_one = 0;
	pgoal->parent = NULL;
	pgoal->lchild = NULL;
	pgoal->rchild = NULL;

	start_time = clock();
	insert(pproblem);

	while(1)
	{
		for( i = 0; i < 2; i++)
		{
			if(queue[i])
			{
				ptemp = queue[i];
				queue[i] = NULL;
				//logic to find the empty co-ordinate in the matrix
				if(!ptemp->zero_zero)
					empty_space = ZERO_ZERO;
					
				else if(!ptemp->zero_one)
					empty_space = ZERO_ONE;
					
				else if(!ptemp->one_zero)
					empty_space = ONE_ZERO;
					
				else if(!ptemp->one_one)
					empty_space = ONE_ONE;
				else 
					assert(0 && "none of the element in this node is having value 0");
				
				//create two possible next state from the current state and initialize them
				ptemp->lchild = malloc(sizeof(struct node));
				ptemp->rchild = malloc(sizeof(struct node));
				ptemp->lchild = (struct node*)memcpy(ptemp->lchild, ptemp, sizeof(struct node));
				ptemp->rchild = (struct node*)memcpy(ptemp->rchild, ptemp, sizeof(struct node));
				ptemp->lchild->parent = ptemp;
				ptemp->rchild->parent = ptemp;
				ptemp->lchild->lchild = NULL;
				ptemp->lchild->rchild = NULL;
				ptemp->rchild->lchild = NULL;
				ptemp->rchild->rchild = NULL;

				//logic to calculate the next possible states from the current state
				switch(empty_space)
				{
					case ZERO_ZERO:
						swap(&ptemp->lchild->zero_zero, &ptemp->lchild->zero_one);
						swap(&ptemp->rchild->zero_zero, &ptemp->rchild->one_zero);
						break;
					case ZERO_ONE:
						swap(&ptemp->lchild->zero_one, &ptemp->lchild->one_one);
						swap(&ptemp->rchild->zero_one, &ptemp->rchild->zero_zero);
						break;
					case ONE_ZERO:
						swap(&ptemp->lchild->one_zero, &ptemp->lchild->one_one);
						swap(&ptemp->rchild->one_zero, &ptemp->rchild->zero_zero);
						break;
					case ONE_ONE:
						swap(&ptemp->lchild->one_one, &ptemp->lchild->one_zero);
						swap(&ptemp->rchild->one_one, &ptemp->rchild->zero_one);
						break;
					default:
						assert(0 && "switch case value is out of range");
						
				}			

				//logic to remove redundant states to save comutation time and space for storage
				if((ptemp->parent)&&(ptemp->parent->zero_zero == ptemp->lchild->zero_zero)&&(ptemp->parent->zero_one == ptemp->lchild->zero_one)\
					&&(ptemp->parent->one_zero == ptemp->lchild->one_zero)\
					&&(ptemp->parent->one_one == ptemp->lchild->one_one))
				{
				    free(ptemp->lchild);
				    ptemp->lchild = NULL;
				}
				if((ptemp->parent)&&(ptemp->parent->zero_zero == ptemp->rchild->zero_zero)&&(ptemp->parent->zero_one == ptemp->rchild->zero_one)\
					&&(ptemp->parent->one_zero == ptemp->rchild->one_zero)\
					&&(ptemp->parent->one_one == ptemp->rchild->one_one))
				{
				    free(ptemp->rchild);
				    ptemp->rchild = NULL;
				}

				//insert only the relevant next states in the queue.
				if(ptemp->lchild)insert(ptemp->lchild);
				if(ptemp->rchild)insert(ptemp->rchild);
				if(ptemp->lchild && ptemp->rchild)break;
			}//if(queue[i])
		}//for(i = 0; i < 2; i++)
		if(is_goal_found(&goal_found_at_index, pgoal))
			break;
	}//while(1)
	end_time = clock();	
	
//logic to print all the states from the first found goal state to start state
	ptemp = queue[goal_found_at_index];
	do{
		printf("[%d  %d]\n", ptemp->zero_zero, ptemp->zero_one);
		printf("[%d  %d]\n", ptemp->one_zero, ptemp->one_one);
		printf("   ||   \n");
	}while(ptemp = ptemp->parent);
	printf("Total time taken by program to reach to goal state from start state:%lfs\n",(double)(end_time - start_time)/CLOCKS_PER_SEC);
	
	return 0;
}

