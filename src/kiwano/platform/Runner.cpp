// Copyright (c) 2016-2018 Kiwano - Nomango
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include <kiwano/utils/Logger.h>
#include <kiwano/platform/Runner.h>
#include <kiwano/platform/Input.h>
#include <kiwano/platform/Application.h>
#include <kiwano/render/Renderer.h>
#include <kiwano/base/Director.h>

namespace kiwano
{

RunnerPtr Runner::Create(Settings settings)
{
    RunnerPtr ptr = memory::New<Runner>();
    if (ptr)
    {
        ptr->SetSettings(settings);
    }
    return ptr;
}

RunnerPtr Runner::Create(Settings settings, Function<void()> on_ready, Function<void()> on_destroy)
{
    class CallbackRunner : public Runner
    {
    public:
        void OnReady() override
        {
            if (on_ready)
                on_ready();
        }

        void OnDestroy() override
        {
            if (on_destroy)
                on_destroy();
        }

        Function<void()> on_ready;
        Function<void()> on_destroy;
    };

    SmartPtr<CallbackRunner> ptr = memory::New<CallbackRunner>();
    if (ptr)
    {
        ptr->on_ready   = on_ready;
        ptr->on_destroy = on_destroy;
        ptr->SetSettings(settings);
    }
    return ptr;
}

Runner::Runner() {}

Runner::~Runner() {}

void Runner::InitSettings()
{
    if (settings_.debug_mode)
    {
        // Show console window before creating main window
        Logger::GetInstance().ShowConsole(true);
    }

    // Create game window
    WindowPtr window =
        Window::Create(settings_.title, settings_.width, settings_.height, settings_.icon, settings_.resizable);
    SetWindow(window);

    // Update renderer settings
    Renderer::GetInstance().MakeContextForWindow(window);
    Renderer::GetInstance().SetClearColor(settings_.bg_color);
    Renderer::GetInstance().SetVSyncEnabled(settings_.vsync_enabled);

    // Use defaut modules
    Application::GetInstance().Use(Input::GetInstance());
    Application::GetInstance().Use(Director::GetInstance());

    // Enable debug mode
    if (settings_.debug_mode)
    {
        Director::GetInstance().ShowDebugInfo(true);
        Renderer::GetInstance().GetContext().SetCollectingStatus(true);
    }
}

bool Runner::MainLoop(Duration dt)
{
    if (!main_window_)
        return false;

    if (main_window_->ShouldClose())
    {
        if (this->OnClose())
            return false;

        main_window_->SetShouldClose(false);
    }

    Application& app = Application::GetInstance();

    // Update modules before poll events
    app.Update(dt);

    // Poll events
    main_window_->PumpEvents();
    while (EventPtr evt = main_window_->PollEvent())
    {
        app.DispatchEvent(evt.Get());
    }

    app.Render();

    if (app.IsPaused())
    {
        // Slow down when the application is paused
        Duration(5).Sleep();
    }
    return true;
}

}  // namespace kiwano
