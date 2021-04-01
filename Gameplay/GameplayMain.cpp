#include "Gameplay/GameplayMain.h"

#include "CrossPlatform/Primitive/Primitive2D.h"
#include "CrossPlatform/Database.h"
#include "CrossPlatform/SColorRGBA.h"

#include "MathLab/Vector3f.h"
#include "MathLab/MathLib.h"
#include "MathLab/MathLabDefinition.h"
#include <cmath> // sin, cos


using MathLab::Vector3f;
using CrossPlatform::Database;

namespace Gameplay {
    using Primitive::Primitive2D;

    void GameplayMain()
    {
        printf("/////////////Line///////////////");
        Primitive2D::get().DrawLine(Vector2i(10, 10), Vector2i(200, 500));
        unsigned int width = Database::get().engineConfig.m_width;
        unsigned int height = Database::get().engineConfig.m_height;
        for (int i = 0 ;i < height; i++)
        {
            Primitive2D::get().DrawPixel(0, i);
        }

        //Primitive2D::get().DrawLine(Vector2i(10, 10), Vector2i(500, 200));
        //Primitive2D::get().DrawLine(Vector2i(10, 50), Vector2i(500, 200));
        //Primitive2D::get().DrawLine(Vector2i(90, 90), Vector2i(700, 600));
        
        //printf("/////////////////Donut////////////////////");
        //Donut();
    }

    void Donut()
    {
        Vector3f cameraPosition();
        static int tmpdegree = 90;

        Vector3f lightDirection(0, 1, 0);
        int donutRadius = 120;
        int circleRadius = 40;

        float degree = 0;    //(0 ~ 360)

        float X = 0;
        float Y = 0;
        float Z = 0;

        for (int thetaDegree = 0; thetaDegree < 360; thetaDegree += 5)
        {
            Vector3f circle;
            Vector3f circleNormal;
            float posX = 0;
            float posY = 0;
            float posZ = 0;

            float theta = thetaDegree * TO_RADIAN;
            circle.x = donutRadius + circleRadius * sin(theta);
            circle.y = circleRadius * cos(theta);
            circleNormal.x = sin(theta);
            circleNormal.y = cos(theta);

            //To donut, rotate with y-axis
            //(x,y,z) * |cos  , 0 ,sin|
            //          |0    ,1  ,0  |       
            //          |-sin ,0  ,cos|

            for (int phiDegree = 0; phiDegree < 360; phiDegree += 5)
            {
                Vector3f donutVertex = MathLab::RotateYAxis(circle, (float)phiDegree);
                Vector3f donutVertexNormal = MathLab::RotateYAxis(circleNormal, (float)phiDegree);
                //float phi = phiDegree * TO_RADIAN;
                //X = posX * cos(phi) - posZ * sin(phi);
                //Y = posY;
                //Z = posY * sin(phi) + posZ * cos(phi);

                //local
                //Vector3 donutVertex(X, Y, Z);

                donutVertex = MathLab::RotateXAxis(donutVertex, (float)tmpdegree);
                donutVertexNormal = MathLab::RotateXAxis(donutVertexNormal, (float)tmpdegree);
                //local to world
                donutVertex = MathLab::RotateZAxis(donutVertex, tmpdegree);
                donutVertexNormal = MathLab::RotateZAxis(donutVertexNormal, (float)tmpdegree);
                donutVertex += Vector3f(0, 0, -170);

                //local to view 


                //view to projection
                int screenX = 0;
                int screenY = 0;

                if (donutVertex.z >= -1)
                {
                    continue;
                }
                float reciprocalOfZ = -1 / donutVertex.z;

                unsigned int screenWidth = Database::get().engineConfig.m_width;
                unsigned int screenHight = Database::get().engineConfig.m_height;
                // Rasterized
                if (Database::get().engineConfig.m_isPerspectiveProjection)
                {
                    screenX = (int)screenWidth / 2 + donutVertex.x * reciprocalOfZ * 100;
                    screenY = (int)screenHight / 2 - donutVertex.y * reciprocalOfZ * 100;
                }
                else {
                    screenX = (int)donutVertex.x + screenWidth / 2;
                    screenY = (int)-(donutVertex.y) + screenHight / 2;
                }

                // Z buffer
                //float curretZBuffer = m_windowDrawer->GetFrameBuffer()->GetZBuffer(screenX, screenY);
                float curretZBuffer = static_cast<float>(Primitive2D::get().GetZbuffer(Vector2i( screenX, screenY)));

                if (curretZBuffer < reciprocalOfZ)
                {
                    float L = donutVertexNormal.dot(lightDirection);
                    L += 0.5f;
                    if (L > 0)
                    {
                        L *= 255; //scale up to 255
                        L += 20;
                        L = MathLab::clamp(L, 0.0f, 255.0f);
                        Primitive2D::get().SetZBuffer(Vector2i( screenX, screenY), static_cast<unsigned int>(reciprocalOfZ));
                        Primitive2D::get().DrawPixel(screenX, screenY, CrossPlatform::SColorRGBA(L, L, L));
                    }
                }
            }
        }

        tmpdegree += 1;
    }

}