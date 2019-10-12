#pragma once

#include <string>

#include <d3d11.h>

#include <VRPG/Win/Event.h>
#include <VRPG/Win/Keyboard.h>
#include <VRPG/Win/Mouse.h>

VRPG_WIN_BEGIN

struct WindowImplData;

struct WindowDesc
{
    int clientWidth  = 640;
    int clientHeight = 480;
    std::wstring windowTitle = L"VRPG Window";

    bool vsync = true;

    int sampleCount   = 1;
    int sampleQuality = 0;

    DXGI_FORMAT colorFormat        = DXGI_FORMAT_R8G8B8A8_UNORM;
    DXGI_FORMAT depthStencilFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
};

struct WindowResizeEvent
{
    int newClientWidth;
    int newClientHeight;
};

struct WindowCloseEvent { };

struct WindowGetFocusEvent { };

struct WindowLostFocusEvent { };

using WindowEventManager  = EventManager<
    WindowResizeEvent, WindowCloseEvent, WindowGetFocusEvent, WindowLostFocusEvent>;

using WindowResizeHandler    = FunctionalEventHandler<WindowResizeEvent>;
using WindowCloseHandler     = FunctionalEventHandler<WindowCloseEvent>;
using WindowGetFocusHandler  = FunctionalEventHandler<WindowGetFocusEvent>;
using WindowLostFocusHandler = FunctionalEventHandler<WindowLostFocusEvent>;

class Window : public agz::misc::uncopyable_t
{
    WindowImplData *data_ = nullptr;
    WindowEventManager eventMgr_; 

public:

    Window() = default;

    ~Window();

    void Initialize(const WindowDesc &windowDesc);

    bool IsAvailable() const noexcept;

    void Destroy();

    int GetClientSizeX() const noexcept;

    int GetClientSizeY() const noexcept;

    void SetVSync(bool vsync) noexcept;

    bool GetVSync() const noexcept;

    void UseDefaultViewport();

    void ClearRenderTarget();

    void ClearDepthStencil();

    void SwapBuffers();

    void DoEvents();

    template<typename Event>
    void Attach(EventHandler<Event> *handler) { eventMgr_.Attach(handler); }

    template<typename Event>
    void Detach(EventHandler<Event> *handler) { eventMgr_.Detach(handler); }

    void SetMouse(MouseEventManager *mouse) noexcept;

    void SetKeyboard(KeyboardEventManager *keyboard) noexcept;

    ID3D11Device *Device() const noexcept;

    ID3D11DeviceContext *DeviceContext() const noexcept;

    HWND NativeWindowHandle() const noexcept;

    HINSTANCE NativeProgramHandle() const noexcept;

    void SetCloseFlag(bool shouldClose) noexcept;

    bool GetCloseFlag() const noexcept;

    void _resize();

    void _close();

    void _getFocus();

    void _lostFocus();

    void _mouse_button_down(MouseButton button);

    void _mouse_button_up(MouseButton button);

    void _cursor_move(int x, int y);

    void _wheel_scroll(int offset);

    void _key_down(KeyCode key);

    void _key_up(KeyCode key);

    void _char_input(uint32_t ch);
};

VRPG_WIN_END
