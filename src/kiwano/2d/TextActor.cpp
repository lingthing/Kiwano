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

#include <kiwano/2d/TextActor.h>
#include <kiwano/core/Logger.h>
#include <kiwano/renderer/Renderer.h>

namespace kiwano
{
	namespace
	{
		TextStyle text_default_style;
	}

	void TextActor::SetDefaultStyle(TextStyle const & style)
	{
		text_default_style = style;
	}

	TextActor::TextActor()
		: TextActor(String())
	{
	}

	TextActor::TextActor(String const& text)
		: TextActor(text, text_default_style)
	{
	}

	TextActor::TextActor(String const& text, const TextStyle & style)
	{
		SetText(text);
		SetStyle(style);
	}

	TextActor::~TextActor()
	{
	}

	void TextActor::OnRender(RenderTarget* rt)
	{
		rt->DrawTextLayout(text_layout_);
	}

	void TextActor::OnUpdate(Duration dt)
	{
		if (text_layout_.IsDirty())
		{
			text_layout_.Update();
			SetSize(text_layout_.GetLayoutSize());
		}
	}

	bool TextActor::CheckVisibilty(RenderTarget* rt) const
	{
		return text_layout_.IsValid() && Actor::CheckVisibilty(rt);
	}
}
