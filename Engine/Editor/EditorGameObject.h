#pragma once
#include "MathLab/Vector3.h"
#include "MathLab/Vector4.h"

namespace XenonEngine
{
    class GameObject;
	class IComponent;

    class EditorGameObject {

    public:
        void Update(const GameObject* data = nullptr);
    protected:
    private:
		void ContextMenu(GameObject* gameobject, IComponent* component)const;
        bool DragFloat3(const char* label, MathLab::Vector3f& v, float v_speed = 1.0f, float v_min = 0.0f, float v_max = 0.0f, const char* format = "%.3f", float power = 1.0f);
        bool ColorEdit4(const char* label, MathLab::Vector4f& col);
        void Text(const MathLab::Vector2f& value);
        void Text(const MathLab::Vector3f& value);
    };

}