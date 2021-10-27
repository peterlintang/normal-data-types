
/*
 * NOT IMPLEMENTED YET
 */

b_tree_search(x, k)
{
	int i = 0;

	while (i < x.n && k > x.key[i])
	{
		i += 1;
	}

	if (i < x.n && k == x.key[i])
		return (x, i);
	else if (x.leaf)
		return nil;
	else
	{
		disk_read(x, x.c[i]);
		return b_tree_search(x.c[i], k);
	}
}

b_tree_create(T)
{
	x = allocate_node();
	x.leaf = true;
	x.n = 0;
	disk_write(x);
	T.root = x;
}

static b_tree_split_child(x, i)
{
	z = allocate_node();
	y = x.c[i];
	z.leaf = y.leaf;
	z.n = t - 1;

	for (int j = 0; j < t; j++)
	{
		z.key[j] = y.key[t + j];
	}

	if (y.leaf == false)
	{
		for (int j = 0; j < t; j++)
			z.c[j] = y.c[t + j];
	}

	y.n = t - 1;

	for (int j = x.n + 1; j > i + 1; j--)
	{
		x.c[j + 1] = x.c[j];
	}
	x.c[i + 1] = z;

	for (int j = x.n; j > i; j--)
	{
		x.key[j + 1] = x.key[j];
	}
	x.key[i] = y.key[t];
	x.n = x.n + 1;
	disk_write(y);
	disk_write(z);
	disk_write(x);
}

static b_tree_insert_nonfull(x, k)
{
	i = x.n;
	if (x.leaf)
	{
		while (i >= 1 && k < x.key[i])
		{
			x.key[i + 1] = x.key[i];
			i = i - 1;
		}
		x.key[i + 1] = k;
		x.n = x.n + 1;
		disk_write(x);
	}
	else
	{
		while (i >= 1 && k < x.key[i])
		{
			i = i - 1;
		}
		i = i + 1;
		disk_read(x.c[i]);
		if (x.c[i].n == 2t - 1)
		{
			b_tree_split_child(x, i);
			if (k > x.key[i])
				i = i + 1;
		}
		b_tree_insert_nonfull(x.c[i], k);
	}
}

b_tree_insert(T, k)
{
	r = T.root;
	if (r.n == 2t - 1)
	{
		s = allocate_node();
		T.root = s;
		s.leaf = false;
		s.n = 0;
		s.c[0] = r;
		b_tree_split_child(s, 1);
		b_tree_insert_nonfull(s, k);
	}
	else
	{
		b_tree_insert_nonfull(r, k);
	}
}

b_tree_delete(T, k)
{
}






