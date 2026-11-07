#include "env/ViZDoomEnv.hpp"

#include <chrono>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <thread>

#include "util/Utils.hpp"

namespace {
    constexpr const char* windowName = "doom-rl";
}  // namespace

namespace doomrl {
    ViZDoomEnv::ViZDoomEnv(const EnvConfig& config, unsigned seed)
        : frameSkip{config.frameSkip}, render{config.render} {
        if (!game.loadConfig(getScenarioPath(config.scenario))) {
            throw std::runtime_error("failed to load scenario: " + config.scenario);
        }

        game.setViZDoomPath(VIZDOOM_ENGINE_PATH);
        game.setDoomGamePath(VIZDOOM_IWAD_PATH);
        game.setScreenFormat(vizdoom::RGB24);
        game.setWindowVisible(false);
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

    void ViZDoomEnv::show(const cv::Mat& frame) {
        cv::Mat bigWindow;
        cv::resize(frame, bigWindow, {}, 3.0, 3.0, cv::INTER_NEAREST);
        cv::cvtColor(bigWindow, bigWindow, cv::COLOR_RGB2BGR);
        cv::imshow(windowName, bigWindow);
        cv::waitKey(1);

        if (cv::getWindowProperty(windowName, cv::WND_PROP_VISIBLE) < 1) {
            windowClosed = true;
        }
    }

    int ViZDoomEnv::numActions() const {
        return static_cast<int>(actions.size());
    }

    bool ViZDoomEnv::closed() const {
        return windowClosed;
    }

    cv::Mat ViZDoomEnv::reset() {
        game.newEpisode();

        cv::Mat frame = currentFrame();
        if (render && !windowClosed) {
            show(frame);
        }
        return frame;
    }

    StepResult ViZDoomEnv::step(int action) {
        const double reward = game.makeAction(actions.at(action), frameSkip);
        const bool done = game.isEpisodeFinished();
        cv::Mat frame = currentFrame();

        if (render && !windowClosed) {
            show(frame);
            std::this_thread::sleep_for(
                std::chrono::milliseconds(frameSkip * 1000 / vizdoom::DEFAULT_TICRATE));
        }

        return StepResult{static_cast<float>(reward), done, frame};
    }

    ViZDoomEnv::~ViZDoomEnv() {
        if (render && !windowClosed) {
            cv::destroyWindow(windowName);
        }
        game.close();
    }

}  // namespace doomrl