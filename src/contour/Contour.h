/**
 * This file is part of the "libterminal" project
 *   Copyright (c) 2019 Christian Parpart <christian@parpart.family>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#pragma once

#include "Config.h"
#include "UIWindow.h"
#include <ground/FileChangeWatcher.h>
#include <ground/stdfs.h>

#include <terminal/InputGenerator.h>

#include <terminal_view/FontManager.h>
#include <terminal_view/GLLogger.h>
#include <terminal_view/TerminalView.h>

#include <atomic>
#include <chrono>
#include <fstream>
#include <functional>
#include <mutex>
#include <string>

class Contour {
  public:
    explicit Contour(Config const& _config);
    ~Contour();

    int main();

	void post(std::function<void()> _fn);

  private:
    void render();
    void onResize();
    void onKey(int _key, int _scanCode, int _action, int _mods);
    void onChar(char32_t _char);
    void onMouseScroll(double _xOffset, double _yOffset);
    void onMouseButton(int _button, int _action, int _mods);
    void onMousePosition(double _x, double _y);
    void onContentScale(float _xs, float _ys);
    void onScreenUpdate();
    void onWindowTitleChanged();
    void doResize(unsigned _width, unsigned _height, bool _inPixels);
    void onConfigReload(ground::FileChangeWatcher::Event _event);
    bool reloadConfigValues();
    bool setFontSize(unsigned _fontSize, bool _resizeWindowIfNeeded);
    terminal::view::Font const& regularFont() const noexcept { return terminalView_.regularFont(); }
    void executeInput(terminal::InputEvent const& _inputEvent);
    void executeAction(Action const& _action);
    std::string extractSelectionText();

  private:
	std::chrono::steady_clock::time_point now_;
    std::ofstream loggingSink_;
    Config config_;
    terminal::view::GLLogger logger_;
    terminal::view::FontManager fontManager_;
    std::reference_wrapper<terminal::view::Font> regularFont_;
    UIWindow window_;
    terminal::view::TerminalView terminalView_;
    bool keyHandled_ = false;
    std::atomic<bool> configReloadPending_ = false;
    ground::FileChangeWatcher configFileChangeWatcher_;
    terminal::Modifier modifier_{};
    bool screenDirty_ = true;

	std::mutex queuedCallsLock_;
	std::deque<std::function<void()>> queuedCalls_;
};
