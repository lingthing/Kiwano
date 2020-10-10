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
#include <kiwano/2d/Actor.h>
#include <kiwano/2d/animation/KeyFrame.h>

namespace kiwano
{

KGE_DECLARE_SMART_PTR(Sprite);

/**
 * \addtogroup Actors
 * @{
 */

/**
 * \~chinese
 * @brief 精灵
 */
class KGE_API Sprite : public Actor
{
public:
    Sprite();

    /// \~chinese
    /// @brief 创建精灵
    /// @param file_path 本地图片路径
    Sprite(const String& file_path);

    /// \~chinese
    /// @brief 创建精灵
    /// @param res 图片资源
    Sprite(const Resource& res);

    /// \~chinese
    /// @brief 创建精灵
    /// @param texture 图像
    Sprite(TexturePtr texture);

    /// \~chinese
    /// @brief 创建精灵
    /// @param file_path 本地图片路径
    /// @param crop_rect 裁剪矩形
    Sprite(const String& file_path, const Rect& crop_rect);

    /// \~chinese
    /// @brief 创建精灵
    /// @param res 图片资源
    /// @param crop_rect 裁剪矩形
    Sprite(const Resource& res, const Rect& crop_rect);

    /// \~chinese
    /// @brief 创建精灵
    /// @param texture 图像
    /// @param crop_rect 裁剪矩形
    Sprite(TexturePtr texture, const Rect& crop_rect);

    virtual ~Sprite();

    /// \~chinese
    /// @brief 加载本地图片并重置裁剪矩形
    /// @param file_path 本地图片路径
    bool Load(const String& file_path);

    /// \~chinese
    /// @brief 加载图像资源并重置裁剪矩形
    /// @param res 图片资源
    bool Load(const Resource& res);

    /// \~chinese
    /// @brief 获取图像
    TexturePtr GetTexture() const;

    /// \~chinese
    /// @brief 获取裁剪矩形
    Rect GetCropRect() const;

    /// \~chinese
    /// @brief 设置图像并重置裁剪矩形
    /// @param[in] texture 图像
    void SetTexture(TexturePtr texture);

    /// \~chinese
    /// @brief 使用矩形区域裁剪精灵
    /// @param crop_rect 裁剪矩形
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief 设置关键帧
    /// @param[in] frame 关键帧
    void SetKeyFrame(KeyFramePtr frame);

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    TexturePtr texture_;
    Rect       crop_rect_;
};

/** @} */

inline TexturePtr Sprite::GetTexture() const
{
    return texture_;
}

inline Rect Sprite::GetCropRect() const
{
    return crop_rect_;
}

inline void Sprite::SetCropRect(const Rect& crop_rect)
{
    crop_rect_ = crop_rect;
    SetSize(crop_rect.GetSize());
}

inline void Sprite::SetKeyFrame(KeyFramePtr frame)
{
    if (frame)
    {
        SetTexture(frame->GetTexture());
        SetCropRect(frame->GetCropRect());
    }
}

}  // namespace kiwano
