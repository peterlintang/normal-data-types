
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

/*
 * 10-1 
 * implement the stack described by this sector
 */

#define STACK_SIZE	1024

struct Stack {
	int s[STACK_SIZE];
	int top;
};

int stack_empty(struct Stack *s)
{
	assert(s);

	if (s->top == 0)
		return 1;
	else
		return 0;
}

int stack_push(struct Stack *s, int x)
{
	assert(s);

	if (s->top >= STACK_SIZE)
	{
		fprintf(stderr, "overflow\n");
		return -1;
	}

	s->s[s->top] = x;
	s->top = s->top + 1;
}

int stack_pop(struct Stack *s)
{
	if (stack_empty(s))
	{
		fprintf(stderr, "underflow\n");
		return -1;
	}

	s->top = s->top - 1;
	return s->s[s->top];
}


/*
 * test code
 */
int main(int argc, char *argv[])
{
	struct Stack *s = NULL;
	int ret = 0;

	s = (struct Stack *)calloc(1, sizeof(*s));
	if (s == NULL)
	{
		fprintf(stderr, "no mem\n");
		return -1;
	}

	for (int i = 0; i < 10; i++)
	{
		stack_push(s, i);
	}

	for (int i = 0; i < 10; i++)
	{
		ret = stack_pop(s);
		fprintf(stdout, "i: %d, s: %d\n", i, ret);
	}

		ret = stack_pop(s);
	return 0;
}




