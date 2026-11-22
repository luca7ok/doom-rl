#include "env/ViZDoomEnv.hpp"

#include <chrono>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <thread>

#include "util/Utils.hpp"

namespace {
    constexpr const char* windowName = "doom-rl";
    constexpr double windowScale = 3.0;
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
        cv::resize(frame, bigWindow, {}, windowScale, windowScale, cv::INTER_NEAREST);
        cv::cvtColor(bigWindow, bigWindow, cv::COLOR_RGB2BGR);
        cv::imshow(windowName, bigWindow);

        const int key = cv::waitKey(1);
        if (key == 'q') {
            cv::destroyWindow(windowName);
            windowClosed = true;
            return;
        }

        if (cv::getWindowProperty(windowName, cv::WND_PROP_VISIBLE) < 1) {
            windowClosed = true;
        }
    }

    void ViZDoomEnv::waitForNextTic() {
        nextTic += std::chrono::microseconds(1'000'000 / vizdoom::DEFAULT_TICRATE);
        std::this_thread::sleep_until(nextTic);
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
            nextTic = std::chrono::steady_clock::now();
        }
        return frame;
    }

    StepResult ViZDoomEnv::step(int action) {
        if (!render || windowClosed) {
            const double reward = game.makeAction(actions.at(action), frameSkip);
            return StepResult{static_cast<float>(reward), game.isEpisodeFinished(), currentFrame()};
        }

        game.setAction(actions.at(action));
        double reward = 0.0;
        for (int tic = 0; tic < frameSkip && !windowClosed; tic++) {
            game.advanceAction(1);
            reward += game.getLastReward();

            if (game.isEpisodeFinished()) {
                break;
            }
            show(currentFrame());
            waitForNextTic();
        }

        return StepResult{static_cast<float>(reward), game.isEpisodeFinished(), currentFrame()};
    }

    ViZDoomEnv::~ViZDoomEnv() {
        if (render && !windowClosed) {
            cv::destroyWindow(windowName);
        }
        game.close();
    }

}  // namespace doomrl