#pragma once

#include <vector>

#include "ViZDoomGame.h"
#include "config/Config.hpp"
#include "env/Environment.hpp"

namespace doomrl {
    class ViZDoomEnv : public Environment {
    private:
        vizdoom::DoomGame game;
        std::vector<std::vector<double>> actions;
        int frameSkip;
        bool render;
        bool windowClosed = false;

        cv::Mat currentFrame();

        void show(const cv::Mat& frame);

    public:
        ViZDoomEnv(const EnvConfig& config, unsigned seed);

        int numActions() const override;

        StepResult step(int action) override;

        cv::Mat reset() override;

        bool closed() const override;

        ~ViZDoomEnv() override;
    };
}  // namespace doomrl