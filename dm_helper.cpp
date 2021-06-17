#include "dm_helper.h"
#include "image.h"
#include "io.h"


int compute(dm_params_t* dm_params, image_t* im1, image_t* im2, corres_t** corres, int* count)
{
	float_image* dm_results;
	corres_t* dm_corres;
	int ret;

	// Compute matches
	dm_results = deep_matching(im1, im2, dm_params, NULL);

	// Allocate output memory
	dm_corres = (corres_t*)malloc(sizeof(corres_t) * dm_results->ty);
	if (dm_corres == NULL)
	{
		ret = 0;
		goto eof;
	}

	// Copy correspodences
	for (int i = 0; i < dm_results->ty; ++i)
		dm_corres[i] = ((corres_t*)dm_results->pixels)[i];

	// Return output
	*corres = dm_corres;
	*count = dm_results->ty;
	ret = 1;

eof:
	free_image(dm_results);
	return ret;
}


void* dm_alloc(size_t size)
{
	return malloc(size);
}


void dm_free(void* p)
{
	free(p);
}


int dm_alloc_params(dm_params* params)
{
	// Allocate memory for dm_params_t
	dm_params_t* dm_params = (dm_params_t*)malloc(sizeof(dm_params_t));
	if (dm_params == NULL)
		return 0;

	// Set default parameters
	set_default_dm_params(dm_params);
	#ifdef _DEBUG
	dm_params->verbose = 1;
	#endif

	// Copy values from dm_params
	params->downscale_factor = dm_params->prior_img_downscale;
	params->neighbor_radius = dm_params->ngh_rad;
	params->max_patch_size = dm_params->max_psize;
	params->verbose = dm_params->verbose;
	params->nthread = dm_params->n_thread;

	// Assign dm_params to inner_ptr to use deep_matching() 
	params->inner_ptr = dm_params;

	return 1;
}


int dm_update_params(dm_params* params)
{
	// prams is empty, do nothing
	if (params->inner_ptr == NULL)
		return 0;

	// Unlike dm_alloc_params, copy values of dm_params from params
	dm_params_t* dm_params = (dm_params_t*)params->inner_ptr;
	dm_params->prior_img_downscale = params->downscale_factor;
	dm_params->ngh_rad = params->neighbor_radius;
	dm_params->max_psize = params->max_patch_size;
	dm_params->verbose = params->verbose;
	dm_params->n_thread = params->nthread;

	return 1;
}


void dm_free_params(dm_params* params)
{
	if (params->inner_ptr != NULL)
		free(params->inner_ptr);
	params->inner_ptr = NULL;
}


int dm_copy_params(const dm_params* sparams, dm_params* dparams)
{
	// sparams is empty, do nothing
	if (sparams->inner_ptr == NULL)
		return 0;

	// Reuse inner_ptr of dparams if available
	dm_params_t* dm_params = (dm_params_t*)dparams->inner_ptr;

	// inner_ptr of dparams is empty, allocate new memory of dm_params_t
	if (dm_params == NULL)
	{
		if ((dm_params = (dm_params_t*)malloc(sizeof(dm_params_t))) == NULL)
			return 0;
	}
	
	// Copy inner_ptr values from sparams
	*dm_params = *((dm_params_t*)sparams->inner_ptr);

	// Copy sparams to dparams deeply
	*dparams = *sparams;
	dparams->inner_ptr = dm_params;

	return 1;
}


int dm_move_params(dm_params* sparams, dm_params* dparams)
{
	// Copy deeply
	if (dm_copy_params(sparams, dparams) == 1)
	{
		// Make sparams used
		sparams->inner_ptr = NULL;
		return 1;
	}

	return 0;
}


int dm_compute_unsafe(dm_params* params, dm_image* im1, dm_image* im2, dm_corres** corres, int* count)
{
	return compute((dm_params_t*)params->inner_ptr, (image_t*)im1, (image_t*)im2, (corres_t**)corres, count);
}


int dm_compute(dm_params* params, dm_image* im1, dm_image* im2, dm_corres** corres, int* count)
{
	int ret;
	image_t* _im1;
	image_t* _im2;

	// Copy image from original
	_im1 = image_cpy((image_t*)im1);
	_im2 = image_cpy((image_t*)im2);

	// Compute matches
	ret = compute((dm_params_t*)params->inner_ptr, _im1, _im2, (corres_t**)corres, count);

	// Delete copied image
	image_delete(_im1);
	image_delete(_im2);

	return ret;
}
