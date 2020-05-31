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
#include <kiwano/core/IntrusiveList.h>
#include <kiwano/base/component/Component.h>

namespace kiwano
{

/**
 * \addtogroup Component
 * @{
 */

/// \~chinese
/// @brief ����б�
typedef IntrusiveList<ComponentPtr> ComponentList;

/**
 * \~chinese
 * @brief ���������
 */
class KGE_API ComponentManager
{
public:
    /// \~chinese
    /// @brief �������
    /// @param component ���
    Component* AddComponent(ComponentPtr component);

    /// \~chinese
    /// @brief ��ȡ�������
    ComponentList& GetAllComponents();

    /// \~chinese
    /// @brief ��ȡ�������
    const ComponentList& GetAllComponents() const;

    /// \~chinese
    /// @brief �Ƴ����
    void RemoveComponent(ComponentPtr component);

    /// \~chinese
    /// @brief �Ƴ����
    /// @param name �������
    void RemoveComponents(const String& name);

    /// \~chinese
    /// @brief �Ƴ��������
    void RemoveAllComponents();

    /// \~chinese
    /// @brief �������
    void Update(Duration dt);

    /// \~chinese
    /// @brief ��Ⱦ���
    void Render(RenderContext& ctx);

    /// \~chinese
    /// @brief �ַ��¼�
    void DispatchToComponents(Event* evt);

protected:
    ComponentManager(Actor* target);

private:
    Actor*        target_;
    ComponentList components_;
};

/** @} */

}  // namespace kiwano