
#include "labrat.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include "rtc.h"

intersection_list_t intersection_list() {
	intersection_list_t ilist;
	ilist.size = 0;
	ilist.cap = INTERSECTION_LIST_INITIAL_CAP;
	ilist.items = (intersection_t*)calloc(ilist.cap, sizeof(intersection_t));

	ilist.hit = NULL;

	return ilist;
}

void intersection_list_free(intersection_list_t ilist) {
	free(ilist.items);
	free(ilist.hit);
}

void add_intersection(intersection_list_t *ilist, intersection_t item) {
	// OPT: can we avoid heap allocations all together???????
	//      hit could be tracked on the stack
	//      we need to track something else behind for reflection/refraction


	// Update the hit if item is > 0 and less than the current hit
	if (item.t > 0 && (ilist->hit == NULL || item.t < ilist->hit->t)) {
		// If this is the first positive item, initialize the hit
		if (ilist->hit == NULL) {
			ilist->hit = (intersection_t*)calloc(1, sizeof(intersection_t));
		}
		ilist->hit->t = item.t;
		ilist->hit->s = item.s;
	}

	// Grow the array when out of room
	if (ilist->size >= ilist->cap - 1) {
		int oc = ilist->cap;
		ilist->cap *= 2;
		ilist->items = (intersection_t*)realloc(
				ilist->items, sizeof(intersection_t)*ilist->cap);
		if (ilist->items == NULL) {
			fprintf(stderr, "Unable to grow intersection_list from %d to %d", oc, ilist->cap);
			exit(ERROR_GROW_INTERSECTION_LIST);
			return;
		}
	}

	// Find where the item should be inserted (ascending order)
	int i = 0;
	for (; i < ilist->size; i++) {
		if (item.t <= ilist->items[i].t) {
			break;
		}
	}

	// Shift everything to the right of the insertion spot to make room
	ilist->size++;
	for (int j = ilist->size; j > i; j--) {
		ilist->items[j] = ilist->items[j-1];
	}

	// Insert the item
	ilist->items[i] = item;
}

TEST_CASE(intersections_are_kept_in_order) {
	for (int w = 0; w < 100; w++) {
		shape_t s = sphere();
		intersection_list_t ilist = intersection_list();
		const int n = 100;
		for (int i = 0; i < n; i++) {
			intersection_t i1 = {
				.t = rand(),
				.s = s,
			};
			add_intersection(&ilist, i1);
		}

		for (int i = 0; i < n-1; i++) {
			ASSERT_TRUE(ilist.items[i].t <= ilist.items[i+1].t);
		}
		shape_free(s);
		intersection_list_free(ilist);
	}
}

TEST_CASE(hit_all_intersections_positive) {
	shape_t s = sphere();
	intersection_t i1 = {
		.t = 1,
		.s = s,
	};
	intersection_t i2 = {
		.t = 2,
		.s = s,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i1.t, h->t));
	ASSERT_TRUE(shape_eq(i1.s, h->s));
	shape_free(s);
	intersection_list_free(ilist);
}

TEST_CASE(hit_some_intersections_negative) {
	shape_t s = sphere();
	intersection_t i1 = {
		.t = -1,
		.s = s,
	};
	intersection_t i2 = {
		.t = 1,
		.s = s,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i2.t, h->t));
	ASSERT_TRUE(shape_eq(i2.s, h->s));
	shape_free(s);
	intersection_list_free(ilist);
}

TEST_CASE(hit_all_intersections_negative) {
	shape_t s = sphere();
	intersection_t i1 = {
		.t = -2,
	};
	intersection_t i2 = {
		.t = -1,
		.s = s,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(h == NULL);
	shape_free(s);
	intersection_list_free(ilist);
}

TEST_CASE(hit_is_lowest_nonnegative) {
	shape_t s = sphere();
	intersection_t i1 = {
		.t = 5,
		.s = s,
	};
	intersection_t i2 = {
		.t = 7,
		.s = s,
	};
	intersection_t i3 = {
		.t = -3,
		.s = s,
	};
	intersection_t i4 = {
		.t = 2,
		.s = s,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i1);
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i3);
	add_intersection(&ilist, i4);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i4.t, h->t));
	ASSERT_TRUE(shape_eq(i4.s, h->s));
	shape_free(s);
	intersection_list_free(ilist);
}
