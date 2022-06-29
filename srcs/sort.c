//
// Created by peerdb on 29-6-22.
//
#include "summary.h"
#include <stdbool.h>
#include <stdio.h>

typedef int idx_t;

void	swap(t_summary *summaries[], const idx_t i, const idx_t j) {
	t_summary	*tmp = summaries[i];

	summaries[i] = summaries[j];
	summaries[j] = tmp;
}

bool	shouldSwap(const t_summary *symbol, const t_summary *pivot) {
	return (symbol->seconds > pivot->seconds);
}

idx_t	partition (t_summary *summaries[], const idx_t low, const idx_t high) {
	t_summary *pivot = summaries[high];
	idx_t i = low - 1; // Index of smaller element and indicates the right position of pivot found so far

	for (idx_t j = low; j <= high - 1; j++) {
		// if current element is smaller than the pivot
		if (shouldSwap(summaries[j], pivot)) {
			++i;
			swap(summaries, i, j);
		}
	}
	swap(summaries, i + 1, high);
	return (i + 1);
}

void quickSort(t_summary *summaries[], idx_t low, idx_t high) {
	if (low < high) {
		idx_t partitionIndex = partition(summaries, low, high);

		quickSort(summaries, low, partitionIndex - 1);
		quickSort(summaries, partitionIndex + 1, high);
	}
}

void	sort_summaries(t_summary *summaries[], const size_t amount) {
	quickSort(summaries, 0, (idx_t)(amount - 1));
}
