#include "PlayerPersonality.h"

namespace XenonEngine
{
    IComponent* PlayerPersonality::Copy(GameObject*const gameObject) const
    {
        PlayerPersonality* that = new PlayerPersonality(gameObject);
        that->m_velocity = m_velocity;
        that->m_jumpForce = m_jumpForce;
        return that;
    }
}