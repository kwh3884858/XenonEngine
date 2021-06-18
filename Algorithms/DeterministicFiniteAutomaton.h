#pragma once
namespace Algorithm
{
    //For Kruth-Morris-Paratt Substring Find Algorithm
    template<typename T>
    class DeterministicFiniteAutomaton
    {
    public:
        int* m_next;
        T* m_character;

        int m_characterCount;
        int m_countentlength;

        DeterministicFiniteAutomaton();
        ~DeterministicFiniteAutomaton();

        int Get(int characterPos, int pos);
        void Set(int characterPos, int pos, int value);

        T GetCharacter(int pos);
        int GetCharacterPos(T character);
    };

    template<typename T>
    DeterministicFiniteAutomaton<T>::DeterministicFiniteAutomaton()
    {
        m_next = nullptr;
        m_character = nullptr;
        m_characterCount = 0;
        m_countentlength = 0;
    }

    template<typename T>
    DeterministicFiniteAutomaton<T>::~DeterministicFiniteAutomaton()
    {
        if (m_next) {
            delete m_next;
            m_next = nullptr;
        }

        if (m_character) {
            delete m_character;
            m_character = nullptr;
        }
    }

    template<typename T>
    int DeterministicFiniteAutomaton<T>::Get(int characterPos, int pos)
    {
        if (characterPos < 0) {
            return 0;
        }
        return m_next[characterPos * m_countentlength + pos];
    }

    template<typename T>
    void DeterministicFiniteAutomaton<T>::Set(int characterPos, int pos, int value)
    {
        m_next[characterPos * m_countentlength + pos] = value;
    }

    template<typename T>
    T DeterministicFiniteAutomaton<T>::GetCharacter(int pos)
    {
        return m_character[pos];
    }

    template<typename T>
    int DeterministicFiniteAutomaton<T>::GetCharacterPos(T character)
    {
        for (int i = 0; i < m_characterCount; i++) {
            if (m_character[i] == character) {
                return i;
            }
        }
        return -1;
    }

}
