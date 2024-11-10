#pragma once
#include <cassert>

namespace Algorithm
{
	template<typename T>
	class Node
	{
	public:
		Node(const T& value);
		~Node();

		Node* m_previous = nullptr;
		Node* m_next = nullptr;
		T m_value;
	};

	template<typename T>
	Algorithm::Node<T>::Node(const T& value)
	{
		m_value = value;
	}

	template<typename T>
	Algorithm::Node<T>::~Node()
	{
		m_previous = nullptr;
		m_next = nullptr;
	}

	template<typename T>
	class LinkedList
	{
	public:
		LinkedList() = default;
		~LinkedList();

		bool IsEmpty() const;
		void Destory();

		void AddFrontNode(const T& value);
		T RemoveFrontNode();
		void AddRearNode(const T& valeu);
		T RemoveRearNode();
	private:
		Node<T>* m_front = nullptr;
		Node<T>* m_rear = nullptr;
	};

	template<typename T>
	Algorithm::LinkedList<T>::~LinkedList()
	{
		Destory();
	}

	template<typename T>
	bool Algorithm::LinkedList<T>::IsEmpty() const
	{
		assert((m_front && !m_rear) == false);
		assert((!m_front && m_rear) == false);

		if (!m_front && !m_rear)
		{
			return true;
		}
		return false;
	}

	template<typename T>
	void Algorithm::LinkedList<T>::Destory()
	{
		for (Node<T>* current = m_front; current != nullptr; )
		{
			Node<T>* next = current->m_next;
			delete current;
			current = next;
		}
	}

	template<typename T>
	void Algorithm::LinkedList<T>::AddFrontNode(const T& value)
	{
		Node<T>* newFront = new Node<T>(value);
		newFront->m_next = m_front;
		if (m_front)
		{
			m_front->m_previous = newFront;
		}
		m_front = newFront;
	}

	template<typename T>
	T Algorithm::LinkedList<T>::RemoveFrontNode()
	{
		Node<T>* newFront = m_front->m_next;
		T returnValue = m_front->m_value;
		if (newFront)
		{
			newFront->m_previous = nullptr;
		}
		delete m_front;
		m_front = newFront;

		return returnValue;
	}

	template<typename T>
	void Algorithm::LinkedList<T>::AddRearNode(const T& value)
	{
		Node<T>* newRear = new Node<T>(value);
		newRear->m_previous = m_rear;
		if (m_rear)
		{
			m_rear->m_next = newRear;
		}
		m_rear = newRear;
	}

	template<typename T>
	T Algorithm::LinkedList<T>::RemoveRearNode()
	{
		Node* newRear = m_rear->m_previous;
		if (newRear)
		{
			newRear->m_next = nullptr;
		}
		delete m_rear;
		m_rear = newRear;
	}

	template<typename T>
	class Queue
	{
	public:
		void Enqueue(const T& value);
		T Dequeue();
		bool IsEmpty() const;

	private:
		LinkedList<T> m_linkedList;
	};

	template<typename T>
	void Algorithm::Queue<T>::Enqueue(const T& value)
	{
		m_linkedList.AddRearNode(value);
	}

	template<typename T>
	T Algorithm::Queue<T>::Dequeue()
	{
		return m_linkedList.RemoveFrontNode();
	}

	template<typename T>
	bool Algorithm::Queue<T>::IsEmpty() const
	{
		return m_linkedList.IsEmpty();
	}

}