
#include "labrat.h"

#include <float.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "error.h"
#include "intersection.h"
#include "ray.h"
#include "shape.h"
#include "sphere.h"
#include "util.h"
#include "vec4.h"



intersection_list_t intersection_list() {
	intersection_list_t ilist;
	ilist.size = 0;
	ilist.cap = INTERSECTION_LIST_INITIAL_CAP;
	ilist.items = (intersection_t*)calloc(ilist.cap, sizeof(intersection_t));

	ilist.hit = NULL;

	return ilist;
}

void free_intersection_list(intersection_list_t ilist) {
	free(ilist.items);
	free(ilist.hit);
}

void add_intersection(intersection_list_t *ilist, intersection_t item) {
	// Update the hit if item is > 0 and less than the current hit
	if (item.t > 0 && (ilist->hit == NULL || item.t < ilist->hit->t)) {
		// If this is the first positive item, initialize the hit
		if (ilist->hit == NULL) {
			ilist->hit = (intersection_t*)calloc(1, sizeof(intersection_t));
		}
		ilist->hit->t = item.t;
		ilist->hit->type = item.type;
		ilist->hit->id = item.id;
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
		sphere_t s = sphere(1);
		intersection_list_t ilist = intersection_list();
		const int n = 100;
		for (int i = 0; i < n; i++) {
			intersection_t i1 = {
				.t = rand(),
				.type = TYPE_SPHERE,
				.id = s.id,
			};
			add_intersection(&ilist, i1);
		}

		for (int i = 0; i < n-1; i++) {
			ASSERT_TRUE(ilist.items[i].t <= ilist.items[i+1].t);
		}
	free_intersection_list(ilist);
	}
}

TEST_CASE(hit_all_intersections_positive) {
	sphere_t s = sphere(1);
	intersection_t i1 = {
		.t = 1,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i2 = {
		.t = 2,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i1.t, h->t));
	ASSERT_TRUE(req(i1.type, h->type));
	ASSERT_TRUE(req(i1.id, h->id));
	free_intersection_list(ilist);
}

TEST_CASE(hit_some_intersections_negative) {
	sphere_t s = sphere(1);
	intersection_t i1 = {
		.t = -1,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i2 = {
		.t = 1,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i2.t, h->t));
	ASSERT_TRUE(req(i2.type, h->type));
	ASSERT_TRUE(req(i2.id, h->id));
	free_intersection_list(ilist);
}

TEST_CASE(hit_all_intersections_negative) {
	sphere_t s = sphere(1);
	intersection_t i1 = {
		.t = -2,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i2 = {
		.t = -1,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i1);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(h == NULL);
	free_intersection_list(ilist);
}

TEST_CASE(hit_is_lowest_nonnegative) {
	sphere_t s = sphere(1);
	intersection_t i1 = {
		.t = 5,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i2 = {
		.t = 7,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i3 = {
		.t = -3,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_t i4 = {
		.t = 2,
		.type = TYPE_SPHERE,
		.id = s.id,
	};
	intersection_list_t ilist = intersection_list();
	add_intersection(&ilist, i1);
	add_intersection(&ilist, i2);
	add_intersection(&ilist, i3);
	add_intersection(&ilist, i4);

	intersection_t* h = ilist.hit;

	ASSERT_TRUE(req(i4.t, h->t));
	ASSERT_TRUE(req(i4.type, h->type));
	ASSERT_TRUE(req(i4.id, h->id));
	free_intersection_list(ilist);
}

