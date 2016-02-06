// Autotracker
//
// Copyright (C) 2008
// Center for Perceptual Systems
// University of Texas at Austin
//
// jsp Sun Apr 27 09:33:49 CDT 2008

#ifndef AUTOTRACKER_H
#define AUTOTRACKER_H

#include "pyramid.hpp"
#include <algorithm>
#include <functional>
#include <numeric>
#include <iostream>
#include <stdexcept>

namespace flying_dragon
{

template<typename T>
class Autotracker
{
    public:
    Autotracker (size_t level = 3)
        : level_ (level)
        , total_frames_ (16)
        , frame_diffs_ (total_frames_)
        , n_frame_ (0)
        , superpixel_size_ (1 << level_)
    {
    }
    void get_fixation (const jsp::pyramid<T> &p, int &fx, int &fy)
    {
        assert (level_ < p.levels ());
        const size_t ROWS = p[level_].rows ();
        const size_t COLS = p[level_].cols ();
        const jsp::raster<T> &f = p[level_];
        if (f.size () != last_frame_.size ())
        {
            for (size_t i = 0; i < total_frames_; ++i)
                frame_diffs_[i].resize (ROWS, COLS, 0);
            total_energy_.resize (ROWS, COLS, 0);
            last_frame_ = f;
            last_fx_ = COLS / 2;
            last_fy_ = ROWS / 2;
            return;
        }
        assert (f.size () == last_frame_.size ());
        assert (f.size () == frame_diffs_[n_frame_].size ());
        assert (f.size () == total_energy_.size ());
        // Get the diff at each pixel
        transform (f.begin (),
            f.end (),
            last_frame_.begin (),
            frame_diffs_[n_frame_].begin (),
            std::minus<int> ());
        // Square each pixel
        transform (frame_diffs_[n_frame_].begin (),
            frame_diffs_[n_frame_].end (),
            frame_diffs_[n_frame_].begin (),
            frame_diffs_[n_frame_].begin (),
            std::multiplies<int> ());
        // Keep a running total at each pixel
        transform (frame_diffs_[n_frame_].begin (),
            frame_diffs_[n_frame_].end (),
            total_energy_.begin (),
            total_energy_.begin (),
            std::plus<int> ());
        // Attenuate energy over time
        transform (total_energy_.begin (),
            total_energy_.end (),
            total_energy_.begin (),
            bind2nd (std::divides<int> (), 2));
        jsp::raster<int>::iterator i = max_element (total_energy_.begin (), total_energy_.end ());
        int new_fx = static_cast<int> (total_energy_.col (i) * superpixel_size_ + superpixel_size_ / 2);
        int new_fy = static_cast<int> (total_energy_.row (i) * superpixel_size_ + superpixel_size_ / 2);
        int dx = new_fx - last_fx_;
        int dy = new_fy - last_fy_;
        fx = last_fx_ + dx / 10;
        fy = last_fy_ + dy / 10;
        last_fx_ = fx;
        last_fy_ = fy;
        last_frame_ = f;
        ++n_frame_;
        n_frame_ %= total_frames_;
    }
    private:
    const size_t level_;
    const size_t total_frames_;
    std::vector<jsp::raster<int> > frame_diffs_;
    jsp::raster<int> total_energy_;
    jsp::raster<T> last_frame_;
    size_t n_frame_;
    const size_t superpixel_size_;
    int last_fx_, last_fy_;
};

} // namespace flying_dragon

#endif // AUTOTRACKER_H
