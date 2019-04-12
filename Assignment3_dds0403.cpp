
// Use suffix array and LCP to compute
// longest common substring of two input strings. BPW

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include<iostream>

char s[1000000], s1[500000], s2[500000], s3[500000];
int n,           // length of s[] including \0
sa[1000000],   // suffix array for s[]
rank[1000000], // rank[i] gives the rank (subscript) of s[i] in sa[]
lcp[1000000];  // lcp[i] indicates the number of identical prefix symbols
			   // for s[sa[i-1]] and s[sa[i]]

int suffixCompare(const void *xVoidPt, const void *yVoidPt)
{
	// Used in qsort call to generate suffix array.
	int *xPt = (int*)xVoidPt, *yPt = (int*)yVoidPt;

	return strcmp(&s[*xPt], &s[*yPt]);
}

void computeRank()
{
	// Computes rank as the inverse permutation of the suffix array
	int i;

	for (i = 0; i<n; i++)
		rank[sa[i]] = i;
}

void computeLCP()
{
	//Kasai et al linear-time construction
	int h, i, j, k;

	h = 0;  // Index to support result that lcp[rank[i]]>=lcp[rank[i-1]]-1
	for (i = 0; i<n; i++)
	{
		k = rank[i];
		if (k == 0)
			lcp[k] = (-1);
		else
		{
			j = sa[k - 1];
			// Attempt to extend lcp
			while (i + h<n && j + h<n && s[i + h] == s[j + h])
				h++;
			lcp[k] = h;
		}
		if (h>0)
			h--;  // Decrease according to result
	}
}

int fibIndex = 0;

void fib(int k)
{
	if (k == 0)
		s[fibIndex++] = '1';
	else if (k == 1)
		s[fibIndex++] = '0';
	else
	{
		fib(k - 1);
		fib(k - 2);
	}
}

int main()
{
	int i, j, p, k, dollarPos, ampPos, LCSpos = 0, LCSlength = 0;


	scanf("%s", s1);
	scanf("%s", s2);
	scanf("%s", s3);

	for (i = 0; s1[i] != '\0'; i++)
		s[i] = s1[i];

	dollarPos = i;
	s[i++] = '$';
	for (j = 0; s2[j] != '\0'; j++)
		s[i + j] = s2[j];

	n = i + j;
	ampPos = n;
	s[n++] = '&';
	for (k = 0; s3[k] != '\0'; k++)
		s[n + k] = s3[k];

	printf(" \n", s);
	s[n + k] = '\0';
	n = n + k + 1;

	//n=strlen(s)+1;
	printf("n is %d\n", n);

	// Quick-and-dirty suffix array construction
	for (i = 0; i<n; i++)
		sa[i] = i;
	qsort(sa, n, sizeof(int), suffixCompare);
	computeRank();
	computeLCP();
	if (n<20000)
	{
		printf("i   sa  suffix                              lcp s rank lcp[rank]\n");
		for (i = 0; i<n; i++)
			printf("%-3d %-3d %-35.35s %-3d %c  %-3d  %-3d\n",
				i, sa[i], &s[sa[i]], lcp[i], s[i], rank[i], lcp[rank[i]]);
	}
	int c = 0;
	int stringCheck_1 = 0, stringCheck_2 = 0, stringCheck_3 = 0;
	int x, y, z;
	int min, mid, xPos, yPos, zPos;
	int minLCP = 0, startPosition, endPosition;
	for (j = 1; j < n; j++) {
		for (i = j; i < n; i++) {
			if (!(stringCheck_1	== 1 && stringCheck_2 == 1 && stringCheck_3 == 1)) {
				if (sa[i] < dollarPos) {
					if (stringCheck_1 == 0)
						c++;
					stringCheck_1 = 1;
					x = i;
				}
				if (sa[i] > dollarPos && sa[i] < ampPos) {
					if (stringCheck_2 == 0)
						c++;
					stringCheck_2 = 1;
					y = i;
				}
				if (sa[i] > ampPos) {
					if (stringCheck_3 == 0)
						c++;
					stringCheck_3 = 1;
					z = i;
				}
			}


			if (c == 3) {

				if (x<y && x<z) {
					j = x;
					stringCheck_1 = 0;
					min = x;
				}
				else if (y<x && y<z) {
					j = y;
					stringCheck_2 = 0;
					min = y;
				}
				else {
					j = z;
					stringCheck_3 = 0;
					min = z;
				}
				if (x>y && x>z) {
					endPosition = x;
				}
				else if (y>x && y>z) {
					endPosition = y;
				}
				else {
					endPosition = z;
				}
				if (x != endPosition && x != min) {
					mid = x;
				}
				if (y != endPosition && y != min) {
					mid = y;
				}
				if (z != endPosition && z != min) {
					mid = z;
				}

				minLCP = lcp[endPosition - 1] < lcp[endPosition] ? lcp[endPosition - 1] : lcp[endPosition];

				if (minLCP > LCSlength) {
					LCSpos = endPosition;
					LCSlength = minLCP;
					j = j + 1;
					c = c - 1;
					xPos = min;
					yPos = mid;
					zPos = endPosition;
					break;
				}
				j = j + 1;
				c = c - 1;
				break;
			}
		}
	}

	
	printf("Length of longest common substring is %d \n", LCSlength);
	printf("x at %d, y ends at %d, z at %d\n", xPos, yPos, zPos);
	for (i = 0; i<LCSlength; i++)
		printf("%c", s[sa[LCSpos] + i]);
	printf("\n");

	return 0;
}

