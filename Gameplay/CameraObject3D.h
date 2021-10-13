//  3DCameraObject.h
//  XenonEngine
//
//  Created by whkong on 2021-10-13.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "Engine/GameObject.h"
namespace Gameplay
{
    class CameraObject3D :public XenonEngine::GameObject
    {
    public:
        CameraObject3D(const Algorithm::String& name);
        virtual ~CameraObject3D();

        virtual CameraObject3D* Copy() const;
        virtual void Start() override;
        virtual void Update() override;
        virtual void OnTrigger(GameObject* gameobject);
    private:

    };
}