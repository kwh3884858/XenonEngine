//  Cube.h
//  XenonEngine
//
//  Created by whkong on 2021-10-12.
//  Copyright (c) 2021 whkong. All rights reserved.
#pragma once

#include "Engine/GameObject.h"
namespace Gameplay
{
    class Cube3D :public XenonEngine::GameObject
    {
    public:
        Cube3D(const Algorithm::String& name);
        virtual ~Cube3D();

        virtual Cube3D* Copy() const;
        virtual void Start() override;
        virtual void Update() override;
        virtual void OnTrigger(GameObject* gameobject);
    private:

    };
}