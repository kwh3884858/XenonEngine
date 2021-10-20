//  SceneDirectionLight.h
//  XenonEngine
//
//  Created by whkong on 2021-10-20.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "Engine/GameObject.h"
namespace Gameplay
{
    class SceneDirectionLight :public XenonEngine::GameObject
    {
    public:
        SceneDirectionLight(const Algorithm::String& name);
        virtual ~SceneDirectionLight();

        virtual SceneDirectionLight* Copy() const;
        virtual void Start() override;
        virtual void Update() override;
        virtual void OnTrigger(GameObject* gameobject);
    private:

    };
}