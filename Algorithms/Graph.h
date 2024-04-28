#pragma once

#include "Vector.h"

namespace Algorithm
{
	template<bool IsDirected = false>
	class Graph
	{
	public:
		using Adjacention = Vector<int>;
		Graph() = default;
		~Graph() = default;

		bool HasVertex(int vertexIndex) const;
		const Vector<int>& GetAdjacentedvertices(int vertexIndex) const;

		void AddEdge(int vertexFrom, int vertexTo);
		int GetVertexCount() const;
	private:
		void AddVertex(int vertexIndex);
		void AddAdjacent(int originalVertexIndex, int adjacentedVertexIndex);

		int m_edgeCount = 0;
		Vector<Adjacention> m_adjacents;
	};

	template<bool IsDirected /*= false*/>
	bool Algorithm::Graph<IsDirected>::HasVertex(int vertexIndex) const
	{
		return vertexIndex < m_adjacents.Count();
	}

	template<bool IsDirected /*= false*/>
	const Vector<int>& Algorithm::Graph<IsDirected>::GetAdjacentedvertices(int vertexIndex) const
	{
		return m_adjacents[vertexIndex];
	}

	template<bool IsDirected /*= false*/>
	void Algorithm::Graph<IsDirected>::AddEdge(int vertexFrom, int vertexTo)
	{
		AddVertex(vertexFrom);
		AddVertex(vertexTo);
		AddAdjacent(vertexFrom, vertexTo);
		if (IsDirected)
		{
			AddAdjacent(vertexTo, vertexFrom);
		}
		++m_edgeCount;
	}

	template<bool IsDirected /*= false*/>
	int Algorithm::Graph<IsDirected>::GetVertexCount() const
	{
		return m_adjacents.Count();
	}

	template<bool IsDirected /*= false*/>
	void Algorithm::Graph<IsDirected>::AddVertex(int vertexIndex)
	{
		if (!HasVertex(vertexIndex))
		{
			m_adjacents.ResetCount(vertexIndex + 1);
		}
	}

	template<bool IsDirected /*= false*/>
	void Algorithm::Graph<IsDirected>::AddAdjacent(int originalVertexIndex, int adjacentedVertexIndex)
	{
		m_adjacents[originalVertexIndex].Add(adjacentedVertexIndex);
	}

	typedef Graph</*IsDirected = */true> DirectedGraph;
	typedef Graph</*IsDirected = */false> UndirectedGraph;
}