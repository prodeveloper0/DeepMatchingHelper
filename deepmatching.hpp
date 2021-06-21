/*
* Copyright (C) 2021 prodeveloper0
* 
* DeepMatching C++ Wrapper with OpenCV
*/

#pragma once
#ifndef LIBDEEPMATCHING_HELPER_CPP_CV
#define LIBDEEPMATCHING_HELPER_CPP_CV
#include <vector>
#include <thread>

#include <dm_helper.h>

#include <opencv2/core/mat.hpp>
#include <opencv2/core/types.hpp>
#include <opencv2/imgproc/imgproc.hpp>


class DeepMatching
{
public:
	using Pair = std::pair<cv::Point2f, cv::Point2f>;

private:
	dm_params __MyParams;

public:
	DeepMatching()
	{
		dm_alloc_params(&this->__MyParams);
		int cpu_count = std::thread::hardware_concurrency();
		this->nthread(cpu_count > 1 ? (int)(cpu_count / 2) : 1);
	}

	DeepMatching(int downscale_factor, int neighbor_radius)
	{
		dm_alloc_params(&this->__MyParams);
		this->downscale_factor(downscale_factor);
		this->neighbor_radius(neighbor_radius);
		int cpu_count = std::thread::hardware_concurrency();
		this->nthread(cpu_count > 1 ? (int)(cpu_count / 2) : 1);
	}

	DeepMatching(const DeepMatching& other)
	{
		dm_copy_params(&other.__MyParams, &this->__MyParams);
	}

	DeepMatching(DeepMatching&& other) noexcept
	{
		this->__MyParams = other.__MyParams;
		other.__MyParams.inner_ptr = nullptr;
	}

	virtual ~DeepMatching()
	{
		dm_free_params(&this->__MyParams);
	}

public:
	DeepMatching& operator=(const DeepMatching& other)
	{
		dm_copy_params(&other.__MyParams, &this->__MyParams);
		return *this;
	}

	DeepMatching& operator=(DeepMatching&& other) noexcept
	{
		dm_move_params(&other.__MyParams, &this->__MyParams);
		return *this;
	}

public:
	bool is_verbose() const
	{
		return this->__MyParams.verbose != 0;
	}

	void is_verbose(bool verbose)
	{
		this->__MyParams.verbose = verbose ? 1 : 0;
	}

	int downscale_factor() const
	{
		return this->__MyParams.downscale_factor;
	}

	void downscale_factor(int factor)
	{
		this->__MyParams.downscale_factor = factor;
	}

	int neighbor_radius() const
	{
		return this->__MyParams.neighbor_radius;
	}

	void neighbor_radius(int radius)
	{
		this->__MyParams.neighbor_radius = radius;
	}

	int nthread() const
	{
		return this->__MyParams.nthread;
	}

	void nthread(int thread)
	{
		this->__MyParams.nthread = thread;
	}

private:
	static void convert_image(const cv::Mat& src, cv::Mat& dst)
	{
		if (src.channels() == 1)
		{
			dst = src;
			return;
		}

		cv::Mat output;
		cv::cvtColor(src, output, cv::COLOR_BGR2GRAY);
		output.convertTo(output, CV_32FC1);
		dst = output;
	}

	static void make_dm_image_unsafe(const cv::Mat& src, dm_image* pdmi)
	{
		pdmi->width = src.cols;
		pdmi->stride = src.cols;
		pdmi->height = src.rows;
		pdmi->data = (float*)src.data;
	}

public:
	static void filter_matches(const std::vector<Pair>& pts1, const std::vector<float>& scores, int n_top_score, std::vector<Pair>& pts2)
	{
		std::vector<std::pair<int, float>> indices(scores.size());

		// Convert scores array to index, score pairs array
		for (size_t i = 0; i < scores.size(); ++i)
			indices[i] = std::make_pair((int)i, scores[i]);

		// Sort by score
		std::sort(indices.begin(), indices.end(), [](auto& a, auto& b)
		{
			return a.second > b.second;
		});

		// Copy Nth top score corresspondences
		int min_index = n_top_score > indices.size() ? (int)indices.size() : n_top_score;
		std::vector<Pair> output(min_index);
		for (int i = 0; i < min_index; ++i)
			output[i] = pts1[indices[i].first];

		pts2 = output;
	}

	static void draw_matches(const cv::Mat& im1, const cv::Mat& im2, const std::vector<DeepMatching::Pair>& pts, const std::vector<float> scores, cv::Mat& dst, bool point_only=false, int thickness=1, int n_top_score=0)
	{
		cv::Mat output;
		cv::hconcat(im1, im2, output);

		std::vector<Pair> npts = pts;
		if (n_top_score != 0)
			filter_matches(npts, scores, n_top_score, npts);

		for (size_t i = 0; i < npts.size(); ++i)
		{
			cv::Point2f im1_pt = npts[i].first;
			cv::Point2f im2_pt = npts[i].second;

			// Translate im2_pt.x as im1's width
			im2_pt.x += im1.cols;

			if (!point_only)
				cv::line(output, im1_pt, im2_pt, cv::Scalar(255, 0, 255), thickness, cv::LINE_AA);
			cv::circle(output, im1_pt, thickness * 1.5f, cv::Scalar(0, 0, 255), thickness, cv::LINE_AA);
			cv::circle(output, im2_pt, thickness * 1.5f, cv::Scalar(0, 255, 0), thickness, cv::LINE_AA);
		}

		dst = output;
	}

public:
	bool compute(const cv::Mat& im1, const cv::Mat& im2, std::vector<Pair>& pts, std::vector<float>& scores)
	{
		cv::Mat fim1, fim2;
		dm_image dm_im1, dm_im2;
		dm_corres* corres;
		int len;

		// Convert image to 32FC1 imae
		convert_image(im1, fim1);
		convert_image(im2, fim2);

		// Convert cv::Mat to dm_image
		make_dm_image_unsafe(fim1, &dm_im1);
		make_dm_image_unsafe(fim2, &dm_im2);

		// Update parameters
		dm_update_params(&this->__MyParams);

		// Compute matches
		if (!dm_compute_unsafe(&this->__MyParams, &dm_im1, &dm_im2, &corres, &len))
			return false;

		// Convert results
		std::vector<Pair> dm_pts(len);
		std::vector<float> dm_scores(len);

		for (int i = 0; i < len; ++i)
		{
			cv::Point2f ref_pt(corres[i].x0, corres[i].y0);
			cv::Point2f tar_pt(corres[i].x1, corres[i].y1);

			dm_pts[i] = std::make_pair(ref_pt, tar_pt);
			dm_scores[i] = corres[i].score;
		}

		pts = dm_pts;
		scores = dm_scores;

		// Free results
		dm_free(corres);
		return true;
	}
};
#endif
