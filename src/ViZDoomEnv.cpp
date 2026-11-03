#include "ViZDoomEnv.hpp"

#include <chrono>
#include <opencv2/core/mat.hpp>
#include <stdexcept>
#include <thread>

#include "Config.hpp"
#include "Environment.hpp"
#include "Utils.hpp"
#include "ViZDoomConsts.h"
#include "ViZDoomTypes.h"

namespace doomrl {
    ViZDoomEnv::ViZDoomEnv(const EnvConfig& config, unsigned seed)
        : frameSkip{config.frameSkip}, render{config.render} {
        if (!game.loadConfig(getScenarioPath(config.scenario))) {
            throw std::runtime_error("failed to load scenario: " + config.scenario);
        }

        game.setViZDoomPath(VIZDOOM_ENGINE_PATH);
        game.setDoomGamePath(VIZDOOM_IWAD_PATH);
        game.setScreenFormat(vizdoom::RGB24);
        game.setWindowVisible(render);
        game.setSeed(seed);

        if (!game.init()) {
            throw std::runtime_error("failed to initialize ViZDoom");
        }

        const size_t n = game.getAvailableButtonsSize();
        for (size_t i = 0; i < n; i++) {
            std::vector<double> action(n, 0.0);
            action[i] = 1.0;
            actions.push_back(action);
        }
    }

    cv::Mat ViZDoomEnv::currentFrame() {
        const int height = game.getScreenHeight();
        const int width = game.getScreenWidth();

        vizdoom::GameStatePtr state = game.getState();
        if (!state || !state->screenBuffer) {
            return cv::Mat::zeros(height, width, CV_8UC3);
        }

        return cv::Mat(height, width, CV_8UC3, state->screenBuffer->data()).clone();
    }

    int ViZDoomEnv::numActions() const {
        return static_cast<int>(actions.size());
    }

    cv::Mat ViZDoomEnv::reset() {
        game.newEpisode();
        return currentFrame();
    }

    StepResult ViZDoomEnv::step(int action) {
        const double reward = game.makeAction(actions.at(action), frameSkip);

        if (render) {
            std::this_thread::sleep_for(
                std::chrono::milliseconds(frameSkip * 1000 / vizdoom::DEFAULT_TICRATE));
        }

        const bool done = game.isEpisodeFinished();

        return StepResult{static_cast<float>(reward), done, currentFrame()};
    }

    ViZDoomEnv::~ViZDoomEnv() {
        game.close();
    }

}  // namespace doomrl