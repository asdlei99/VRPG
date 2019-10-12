#include <agz/utility/misc.h>
#include <agz/utility/system.h>

#include <VRPG/Win/D3DInit.h>

VRPG_WIN_BEGIN

std::pair<ID3D11Device*, ID3D11DeviceContext*> CreateD3D11Device()
{
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

    UINT createDeviceFlag = 0;
#ifdef AGZ_DEBUG
    createDeviceFlag |= D3D11_CREATE_DEVICE_DEBUG;
#endif

    ID3D11Device *device; ID3D11DeviceContext *deviceContext;
    HRESULT hr = D3D11CreateDevice(
        nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
        createDeviceFlag,
        &featureLevel, 1,
        D3D11_SDK_VERSION,
        &device,
        nullptr,
        &deviceContext);
    if(FAILED(hr))
        return { nullptr, nullptr };

    return { device, deviceContext };
}

IDXGISwapChain *CreateD3D11SwapChain(
    HWND windowHandle, int clientWidth, int clientHeight,
    DXGI_FORMAT swapChainBufferFormat, int sampleCount, int sampleQuality,
    ID3D11Device *device)
{
    DXGI_MODE_DESC bufDesc;
    bufDesc.Width                   = clientWidth;
    bufDesc.Height                  = clientHeight;
    bufDesc.RefreshRate.Numerator   = 60;
    bufDesc.RefreshRate.Denominator = 1;
    bufDesc.Format                  = swapChainBufferFormat;
    bufDesc.ScanlineOrdering        = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    bufDesc.Scaling                 = DXGI_MODE_SCALING_UNSPECIFIED;

    DXGI_SWAP_CHAIN_DESC desc;
    desc.BufferDesc         = bufDesc;
    desc.BufferUsage        = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    desc.BufferCount        = 1;
    desc.OutputWindow       = windowHandle;
    desc.Windowed           = true;
    desc.SwapEffect         = DXGI_SWAP_EFFECT_DISCARD;
    desc.Flags              = 0;
    desc.SampleDesc.Count   = sampleCount;
    desc.SampleDesc.Quality = sampleQuality;

    IDXGIDevice* dxgiDevice;
    HRESULT hr = device->QueryInterface<IDXGIDevice>(&dxgiDevice);
    if(FAILED(hr))
        return nullptr;

    IDXGIAdapter* dxgiAdapter = nullptr;
    hr = dxgiDevice->GetParent(__uuidof(IDXGIAdapter), (void**)&dxgiAdapter);
    ReleaseCOMObjects(dxgiDevice);
    if(FAILED(hr))
        return nullptr;

    IDXGIFactory* dxgiFactory = nullptr;
    hr = dxgiAdapter->GetParent(__uuidof(IDXGIFactory), (void**)&dxgiFactory);
    ReleaseCOMObjects(dxgiAdapter);
    if(FAILED(hr))
        return nullptr;

    IDXGISwapChain* swap_chain;
    hr = dxgiFactory->CreateSwapChain(device, &desc, &swap_chain);
    ReleaseCOMObjects(dxgiFactory);
    if(FAILED(hr))
        return nullptr;

    return swap_chain;
}

ID3D11RenderTargetView *CreateD3D11RenderTargetView(IDXGISwapChain *swapChain, ID3D11Device *device)
{
    ID3D11Texture2D *backgroundBuffer;
    HRESULT hr = swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)(&backgroundBuffer));
    if(FAILED(hr))
        return nullptr;

    ID3D11RenderTargetView* renderTargetView;
    hr = device->CreateRenderTargetView(backgroundBuffer, nullptr, &renderTargetView);
    ReleaseCOMObjects(backgroundBuffer);
    if(FAILED(hr))
        return nullptr;

    return renderTargetView;
}

std::pair<ID3D11Texture2D*, ID3D11DepthStencilView*> CreateD3D11DepthStencilBuffer(
    ID3D11Device *device, int clientWidth, int clientHeight,
    DXGI_FORMAT bufferFormat, int sampleCount, int sampleQuality)
{
    D3D11_TEXTURE2D_DESC buffer_desc;
    buffer_desc.ArraySize          = 1;
    buffer_desc.BindFlags          = D3D11_BIND_DEPTH_STENCIL;
    buffer_desc.CPUAccessFlags     = 0;
    buffer_desc.Format             = bufferFormat;
    buffer_desc.Width              = clientWidth;
    buffer_desc.Height             = clientHeight;
    buffer_desc.MipLevels          = 1;
    buffer_desc.MiscFlags          = 0;
    buffer_desc.SampleDesc.Count   = sampleCount;
    buffer_desc.SampleDesc.Quality = sampleQuality;
    buffer_desc.Usage              = D3D11_USAGE_DEFAULT;

    ID3D11Texture2D *buffer;
    HRESULT hr = device->CreateTexture2D(&buffer_desc, nullptr, &buffer);
    if(FAILED(hr))
        return { nullptr, nullptr };
    agz::misc::scope_guard_t buffer_guard([&] { ReleaseCOMObjects(buffer); });

    D3D11_DEPTH_STENCIL_VIEW_DESC view_desc;
    view_desc.Format             = bufferFormat;
    view_desc.ViewDimension      = sampleCount > 1 ? D3D11_DSV_DIMENSION_TEXTURE2DMS : D3D11_DSV_DIMENSION_TEXTURE2D;
    view_desc.Flags              = 0;
    view_desc.Texture2D.MipSlice = 0;

    ID3D11DepthStencilView *view;
    hr = device->CreateDepthStencilView(buffer, &view_desc, &view);
    if(FAILED(hr))
        return { nullptr, nullptr };

    buffer_guard.dismiss();
    return { buffer, view };
}

VRPG_WIN_END
