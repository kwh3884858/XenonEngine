//  GameplayMain.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

namespace CrossPlatform { class Polygon2D; }
namespace Gameplay {
    void GameplayInitialize();
    void GameplayMain();
    void GameplayShutdown();

    void Donut();
    void DrawPolygon(const CrossPlatform::Polygon2D& polygon);
}
