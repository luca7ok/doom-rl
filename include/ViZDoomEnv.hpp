#pragma once

#include <opencv2/core/mat.hpp>
#include <vector>

#include "Config.hpp"
#include "Environment.hpp"
#include "ViZDoomGame.h"

namespace doomrl {
    class ViZDoomEnv : public Environment {
    private:
        cv::Mat currentFrame();

        vizdoom::DoomGame game;
        std::vector<std::vector<double>> actions;
        int frameSkip;
        bool render;

    public:
        ViZDoomEnv(const EnvConfig& config, unsigned seed);

        int numActions() const override;

        StepResult step(int action) override;

        cv::Mat reset() override;

        ~ViZDoomEnv() override;
    };
}  // namespace doomrl