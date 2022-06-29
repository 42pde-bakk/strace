//
// Created by peerdb on 29-6-22.
//

#include <time.h>

double	measure_elapsed_time(const clock_t start) {
	clock_t	now = clock();

	return ((double)(now - start) / CLOCKS_PER_SEC);
}
