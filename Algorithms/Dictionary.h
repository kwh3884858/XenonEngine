#ifndef SAINTMATHEMATICS_DICTIONARY_H
#define SAINTMATHEMATICS_DICTIONARY_H

#include "Stack.h"

namespace Algorithm
{
	template<typename Key, typename Value>
	class Dictionary
	{
	private:
		struct Node {

		public:
			Key mKey;
			Value mValue;
			Node* mLeft;
			Node* mRight;
			Node* mParent;
			int mCount;

			Node();
			Node(const Key& key, const Value& value);
			Node(const Node& node);
			~Node();
		};

	public:
		Dictionary();
		~Dictionary();

		void add(const Key& key, const Value& value);
		Value get(const Key& key)const;
		void remove(const Key& key);

		const void keys(Key* keyArray)const;

		bool contain(Key key);
		bool isEmpty();

	private:
		bool findNode(const Key& key, Node* node)const;

		Node* mRoot;
	};


	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::Node::Node()
	{
		mKey = nullptr;
		mValue = nullptr;
		mLeft = nullptr;
		mRight = nullptr;
		mParent = nullptr;
		mCount = 0;
	}

	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::Node::Node(const Key & key, const Value & value)
	{
		mKey = key;
		mValue = value;
		mLeft = nullptr;
		mRight = nullptr;
		mParent = nullptr;
		mCount = 0;
	}

	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::Node::Node(const Node & node)
	{
		mKey = node.mKey;
		mValue = node.mValue;
		mLeft = nullptr;
		mRight = nullptr;
		mParent = nullptr;
	}

	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::Node::~Node()
	{
		//Delete action should be the repsonsibility of the programmer.

	}

	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::Dictionary()
	{
		mRoot = nullptr;
	}

	template<typename Key, typename Value>
	inline Dictionary<Key, Value>::~Dictionary()
	{
	}

	template<typename Key, typename Value>
	inline void Dictionary<Key, Value>::add(const Key & key, const Value & value)
	{
		Node* node = mRoot;
		bool result = findNode(key, node);

		if (result == true)
		{
			node->mValue = value;
		}
		else
		{
			Node* newNode = new Node(key, value);

			//parent node
			if (node != nullptr && key < node->mKey)
			{
				node->mLeft = newNode;
			}
			if (node != nullptr && key > node->mKey)
			{
				node->mRight = newNode;
			}

			//child node
			newNode->mParent = node;

			if (mRoot == nullptr)
			{
				mRoot = newNode;
			}
		}

		//node->mValue = value;
	}

	template<typename Key, typename Value>
	inline Value Dictionary<Key, Value>::get(const Key & key) const
	{
		Node* node = mRoot;
		bool result = findNode(key, node);

		if (result)
		{
			return node->mValue;
		}
		else
		{
			return nullptr;
		}

	}

	template<typename Key, typename Value>
	inline void Dictionary<Key, Value>::remove(const Key& key)
	{
		Node* deletedNode = mRoot;
		bool result = findNode(key, deletedNode);
		if (result == false)
		{
			return;
		}

		//Ready delete
		Node* parentNode = deletedNode->mParent;

		Node* newNode = nullptr;

		if (deletedNode->mLeft != nullptr)
		{
			newNode = deletedNode->mLeft;
			while (newNode->mRight != nullptr)
			{
				newNode = newNode->mRight;
			}
		}
		if (deletedNode->mRight != nullptr)
		{
			newNode = deletedNode->mRight;
			while (newNode->mLeft != nullptr)
			{
				newNode = newNode->mLeft;
			}
		}


		delete deletedNode;
		deletedNode = nullptr;
		if (key < parentNode->mKey)
		{
			parentNode->mLeft = newNode;
		}
		else
		{
			parentNode->mRgiht = newNode;
		}

	}

	template<typename Key, typename Value>
	inline const void Dictionary<Key, Value>::keys(Key * keyArray) const
	{
		Node* node;;

		Stack<Node*> stack;
		stack.push(mRoot);

		int currentPos = 0;
		while (stack.count() != 0)
		{
			node = stack.pop();

			if (node->mLeft != nullptr)
			{
				stack.push(node->mLeft);
			}
			if (node->mRight != nullptr)
			{
				stack.push(node->mRight);
			}

			keyArray[currentPos] = node->mKey;
			currentPos++;
		}
		//return Key[]();

	}

	//template<typename Key, typename Value>
	//inline void Dictionary<Key, Value>::keys(const Key* keyArray) const
	//{


	template<typename Key, typename Value>
	inline bool Dictionary<Key, Value>::findNode(const Key& key, Node* root)const

	{
		//root = mRoot;
		while (root != nullptr)
		{
			if (root->mKey == key)
			{
				return true;
			}
			if (key < root->mKey)
			{
				if (root->mLeft == nullptr)
				{
					return false;
				}
				root = root->mLeft;
			}
			else
			{
				if (root->mRight == nullptr)
				{
					return false;
				}
				root = root->mRight;
			}
		}
		return false;
	}

}

#endif

