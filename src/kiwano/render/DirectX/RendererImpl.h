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

#pragma once
#include <kiwano/render/Renderer.h>
#include <kiwano/render/DirectX/FontCollectionLoader.h>
#include <kiwano/render/DirectX/RenderContextImpl.h>

#if defined(KGE_USE_DIRECTX10)
#include <kiwano/render/DirectX/D3D10DeviceResources.h>
#else
#include <kiwano/render/DirectX/D3D11DeviceResources.h>
#endif

namespace kiwano
{

#if defined(KGE_USE_DIRECTX10)
typedef ID3D10DeviceResources ID3DDeviceResources;
#else
typedef ID3D11DeviceResources ID3DDeviceResources;
#endif

class KGE_API RendererImpl
    : public Renderer
{
public:
    static RendererImpl& GetInstance();

    void CreateTexture(Texture& texture, const String& file_path) override;

    void CreateTexture(Texture& texture, const Resource& resource) override;

    void CreateGifImage(GifImage& gif, const String& file_path) override;

    void CreateGifImage(GifImage& gif, const Resource& resource) override;

    void CreateGifImageFrame(GifImage::Frame& frame, const GifImage& gif, size_t frame_index) override;

    void CreateFontCollection(Font& font, const String& file_path) override;

    void CreateFontCollection(Font& font, const Resource& res) override;

    void CreateTextLayout(TextLayout& layout, const String& content, const TextStyle& style) override;

    void CreateLineShape(Shape& shape, const Point& begin_pos, const Point& end_pos) override;

    void CreateRectShape(Shape& shape, const Rect& rect) override;

    void CreateRoundedRectShape(Shape& shape, const Rect& rect, const Vec2& radius) override;

    void CreateEllipseShape(Shape& shape, const Point& center, const Vec2& radius) override;

    void CreateShapeSink(ShapeMaker& maker) override;

    void CreateBrush(Brush& brush, const Color& color) override;

    void CreateBrush(Brush& brush, const LinearGradientStyle& style) override;

    void CreateBrush(Brush& brush, const RadialGradientStyle& style) override;

    void CreateBrush(Brush& brush, TexturePtr texture) override;

    void CreateStrokeStyle(StrokeStyle& stroke_style) override;

    RenderContextPtr CreateTextureRenderContext(Texture& texture, const Size* desired_size = nullptr) override;

    void SetResolution(uint32_t width, uint32_t height, bool fullscreen) override;

    Vector<Resolution> GetResolutions() override;

public:
    void Clear() override;

    void Present() override;

    /// \~chinese
    /// @brief 获取Direct2D设备资源
    ID2DDeviceResources* GetD2DDeviceResources();

    /// \~chinese
    /// @brief 获取Direct3D设备资源
    ID3DDeviceResources* GetD3DDeviceResources();

    /// \~chinese
    /// @brief 重设渲染输出大小
    void Resize(uint32_t width, uint32_t height) override;

protected:
    RendererImpl();

    void MakeContextForWindow(WindowPtr window) override;

    void Destroy() override;

private:
    ComPtr<ID2DDeviceResources>           d2d_res_;
    ComPtr<ID3DDeviceResources>           d3d_res_;
    ComPtr<IFontCollectionLoader>         font_collection_loader_;
    ComPtr<IResourceFontFileLoader>       res_font_file_loader_;
    ComPtr<IResourceFontCollectionLoader> res_font_collection_loader_;

    Vector<Resolution> resolutions_;
};

/** @} */

inline ID2DDeviceResources* RendererImpl::GetD2DDeviceResources()
{
    KGE_ASSERT(d2d_res_);
    return d2d_res_.Get();
}

inline ID3DDeviceResources* RendererImpl::GetD3DDeviceResources()
{
    KGE_ASSERT(d3d_res_);
    return d3d_res_.Get();
}

}  // namespace kiwano
