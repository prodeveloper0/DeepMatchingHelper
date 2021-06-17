/*
* Copyright (C) 2021 prodeveloper0
*
* DeepMatching Helper Library
*/
#pragma once
#ifndef LIBDEEPMATCHING_HELPER_H
#define LIBDEEPMATCHING_HELPER_H
#endif
#ifdef _MSC_VER
#define DECLEARE_EXPORT __declspec(dllexport)
#define DECLEARE_IMPORT __declspec(dllimport)
#else
#define DECLEARE_EXPORT __attribute__((visibility("default")))
#define DECLEARE_IMPORT __attribute__((visibility("default")))
#endif

#ifdef LIBDEEPMATCHING_HELPER_EXPORTS
#define DM_EXPORT DECLEARE_EXPORT
#else
#define DM_EXPORT DECLEARE_IMPORT
#endif

#include <cstddef>


// Equivalent to corres_t
typedef struct {
	float x0, y0;
	float x1, y1;
	float maxima;
	float score;
} dm_corres;


// Equivalent to image_t
typedef struct {
	int width;
	int height;
	int stride;
	float* data;
} dm_image;


// Equilvalent to dm_params_t
typedef struct {
	void* inner_ptr;
	int downscale_factor;
	int neighbor_radius;
	int max_patch_size;
	int verbose;
	int nthread;
} dm_params;


extern "C" DM_EXPORT void* dm_alloc(size_t size);
extern "C" DM_EXPORT void dm_free(void* ptr);

extern "C" DM_EXPORT int dm_alloc_params(dm_params* params);
extern "C" DM_EXPORT int dm_update_params(dm_params* params);
extern "C" DM_EXPORT void dm_free_params(dm_params* params);
extern "C" DM_EXPORT int dm_copy_params(const dm_params* sparams, dm_params* dparams);
extern "C" DM_EXPORT int dm_move_params(dm_params* sparams, dm_params* dparams);

extern "C" DM_EXPORT int dm_compute_unsafe(dm_params* params, dm_image* im1, dm_image* im2, dm_corres** corres, int* count);
extern "C" DM_EXPORT int dm_compute(dm_params* params, dm_image* im1, dm_image* im2, dm_corres** corres, int* count);
