/* Description:- This program is created to analyze the search complexity in
				 any inference(without knowledge) system.
** Author:- Manoj Pandey
*/

#include<stdio.h>
#include<assert.h>
#include<malloc.h>
#include<string.h>
#include<time.h>

/*Structure which will replicate 4x4 matrix along with pointers to traverse
* both forward and backward states.
*/
/*In four-cube problem we will have a maximum of 2 child nodes corresponding
* to a parent node. Hence in the node structure only two pointers are sufficient
* for holding these child(lchild & rchild) nodes.
*/
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

//queue for the temporary storage of next states
//it help us to traverse in BFS
//For four-cube a queue of length 2 is sufficient.
struct node *queue[2] = {NULL, NULL};

//Insertion logic in queue.
void insert( struct node* temp)
{
	if(queue[0] == NULL)
		queue[0] = temp;
	else if(queue[1] == NULL)
		queue[1] = temp;
	else
		assert(0 && "unable to insert the node in queue");
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

void print_node(struct node *pnode)
{
	printf("[%d %d]\n", pnode->zero_zero, pnode->zero_one);
	printf("[%d %d]\n", pnode->one_zero, pnode->one_one);
	printf("\n");
}
int main()
{

	int empty_space, i, temp, goal_found_at_index = 0;
	double start_time, end_time;
	struct node *pproblem, *pgoal, *ptemp = NULL;

	//initializing problem(start state) node. Hard-coded for testing purpose
	pgoal = malloc(sizeof(struct node));
	pproblem = malloc(sizeof(struct node));
	pproblem->zero_zero = 3;
	pproblem->zero_one = 0;
	pproblem->one_zero = 2;
	pproblem->one_one = 1;
	pproblem->parent = NULL;
	pproblem->lchild = NULL;
	pproblem->rchild = NULL;

    printf("Start state node:\n");
	print_node(pproblem);
	//initializing goal node, hard-coded for testing purpose
	pgoal->zero_zero = 1;
	pgoal->zero_one = 2;
	pgoal->one_zero = 3;
	pgoal->one_one = 0;
	pgoal->parent = NULL;
	pgoal->lchild = NULL;
	pgoal->rchild = NULL;

	printf("Goal state node:\n");
	print_node(pgoal);

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

				/*In four-cube, each grandfather node will be exactly similar to
				* one of its grandchildren node. Hence we can remove redundant
				* states to save computation time and storage space. Below
				* mentioned conditions are specific to the four-cube problem.
				*/
				if((ptemp->parent)&&(ptemp->parent->zero_zero == ptemp->lchild->zero_zero)\
					&&(ptemp->parent->zero_one == ptemp->lchild->zero_one)\
					&&(ptemp->parent->one_zero == ptemp->lchild->one_zero)\
					&&(ptemp->parent->one_one == ptemp->lchild->one_one))
				{
				    free(ptemp->lchild);
				    ptemp->lchild = NULL;
				}
				if((ptemp->parent)&&(ptemp->parent->zero_zero == ptemp->rchild->zero_zero)\
					&&(ptemp->parent->zero_one == ptemp->rchild->zero_one)\
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
	printf("Traversing back from goal node to start nod:\n");
	do{
        print_node(ptemp);
	}while(ptemp = ptemp->parent);
	printf("Total time taken by program to reach to goal state from start state:\
            %lfs\n",(double)(end_time - start_time)/CLOCKS_PER_SEC);

	return 0;
}

