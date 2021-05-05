//  Physics2D.h
//  XenonEngine
//
//  Created by whkong on 2021-5-4.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once
namespace XenonPhysics
{
    class Rigidbody2D;

    class Physics2D
    {
    public:
        enum CollisionType
        {
            NoCollision,
            IsCollision,
            Penetrating
        };

        Physics2D();
        ~Physics2D();

        bool FixedUpdate();

        bool AddRigidbody2D(const Rigidbody2D*const rigidbody);
        bool RemoveRigidbody2D(const Rigidbody2D* const rigidbody);

    private:
        static const float TIMESTEP = 0.1f;
        static const float EPSILON = 0.01f;
        Vector<Rigidbody2D*> mDynamicRigidbodys;
        Vector<Rigidbody2D*> mStaticRigidbodys;
    };

}