//  GameplayMain.h
//  MacWindow
//
//  Created by whkong on 2021-3-15.
//  Copyright (c) 2018 whkong. All rights reserved.
#pragma once

namespace CrossPlatform { class Polygon2D; }
namespace Gameplay {
    void GameplayInitialize();
    void GameplayUpdate(long timeInterval);
    void GameplayShutdown();

    void Donut();
}
