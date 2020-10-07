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
#include <kiwano/render/Frame.h>

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
    /// @param frame 图像帧
    Sprite(FramePtr frame);

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

    virtual ~Sprite();

    /// \~chinese
    /// @brief 加载本地图片
    /// @param file_path 本地图片路径
    /// @param autoresize 是否自动调整自身大小为图像大小
    bool Load(const String& file_path, bool autoresize = true);

    /// \~chinese
    /// @brief 加载图像资源
    /// @param res 图片资源
    /// @param autoresize 是否自动调整自身大小为图像大小
    bool Load(const Resource& res, bool autoresize = true);

    /// \~chinese
    /// @brief 获取图像原宽度
    float GetSourceWidth() const;

    /// \~chinese
    /// @brief 获取图像原高度
    float GetSourceHeight() const;

    /// \~chinese
    /// @brief 获取图像原大小
    Size GetSourceSize() const;

    /// \~chinese
    /// @brief 获取裁剪矩形
    Rect GetCropRect() const;

    /// \~chinese
    /// @brief 使用矩形区域裁剪精灵
    /// @param crop_rect 裁剪矩形
    void SetCropRect(const Rect& crop_rect);

    /// \~chinese
    /// @brief 获取帧图像
    FramePtr GetFrame() const;

    /// \~chinese
    /// @brief 设置图像帧
    /// @param[in] frame 图像帧
    /// @param autoresize 是否自动调整自身大小为图像大小
    void SetFrame(FramePtr frame, bool autoresize = true);

    /// \~chinese
    /// @brief 重置精灵大小为图像帧大小
    void ResetSize();

    /// \~chinese
    /// @brief 获取图像帧属性
    inline Property<FramePtr, Function<void()>> FrameProperty()
    {
        return Property<FramePtr, Function<void()>>(std::addressof(frame_), Closure(this, &Sprite::ResetSize));
    }

    void OnRender(RenderContext& ctx) override;

protected:
    bool CheckVisibility(RenderContext& ctx) const override;

private:
    FramePtr frame_;
};

/** @} */

inline FramePtr Sprite::GetFrame() const
{
    return frame_;
}
}  // namespace kiwano
