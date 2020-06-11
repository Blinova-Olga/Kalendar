#include "Header.h"

void Quick_sort(int a[NUM][2], int l, int r)
{
	int i = l, j = r;
	int w,v;
	int k = (l + r) / 2;
	int pivot = a[k][0];
	while (i <= j)
	{
		while (a[i][0] < pivot) i++;
		while (a[j][0] > pivot) j--;
		if (i <= j) {
			w = a[i][0];
			v = a[i][1];
			a[i][0] = a[j][0];
			a[i][1] = a[j][1];
			a[j][0] = w;
			a[j][1] = v;
			i++;
			j--;
		}
	}

	if (i < r)
		Quick_sort(a, i, r);
	if (l < j)
		Quick_sort(a, l, j);
}