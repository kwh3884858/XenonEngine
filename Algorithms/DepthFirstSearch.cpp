#include "DepthFirstSearch.h"
#include "LinkedList.h"

namespace Algorithm
{
	Search::Search(DirectedGraph graph, int startVertex)
	{
		m_directedGraph = graph;
		m_originalVertexIndex = startVertex;

		m_marked.Initialize(graph.GetVertexCount());
		m_edgeTo.Initialize(graph.GetVertexCount());
	}

	void Search::DepthFirstSearch()
	{
		DFS(m_directedGraph, m_originalVertexIndex);
	}

	void Search::BreadthFirstSearch()
	{
		BFS(m_directedGraph, m_originalVertexIndex);
	}

	bool Search::HasPathTo(int terminalVertexIndex, Vector<int>& outPath)
	{
		if (IsMarked(terminalVertexIndex))
		{
			return false;
		}

		for (int index = terminalVertexIndex; index != m_originalVertexIndex; index = m_edgeTo[index])
		{
			outPath.Add(index);
		}
		outPath.Add(m_originalVertexIndex);
		return true;
	}

	void Search::DFS(DirectedGraph graph, int startVertex)
	{
		m_marked[startVertex] = true;
		for (const int& adjacentedVertexIndex : graph.GetAdjacentedvertices(startVertex))
		{
			if (!IsMarked(adjacentedVertexIndex))
			{
				m_edgeTo[adjacentedVertexIndex] = startVertex;
				DFS(graph, adjacentedVertexIndex);
			}
		}
	}

	void Search::BFS(DirectedGraph graph, int startVertex)
	{
		Queue<int> queue;
		queue.Enqueue(startVertex);
		m_marked[startVertex] = true;
		if (!queue.IsEmpty())
		{
			int curentVertex = queue.Dequeue();
			for (const int& adjacentedVertexIndex : graph.GetAdjacentedvertices(curentVertex))
			{
				if (!IsMarked(adjacentedVertexIndex))
				{
					m_edgeTo[adjacentedVertexIndex] = curentVertex;
					m_marked[adjacentedVertexIndex] = true;
					queue.Enqueue(adjacentedVertexIndex);
				}
			}
		}
	}

	bool Search::IsMarked(int startVertex)
	{
		return m_marked[startVertex];
	}

	Circle::Circle(DirectedGraph graph, int startVertex)
	{
		m_directedGraph = graph;
		m_originalVertexIndex = startVertex;

		m_marked.Initialize(graph.GetVertexCount());
		m_edgeTo.Initialize(graph.GetVertexCount());
	}

	bool Circle::DirectedGraphCircle(Vector<int>& outCircle)
	{
		return DirectedGraphDFS(m_directedGraph, m_originalVertexIndex, outCircle);
	}

	bool Circle::DirectedGraphDFS(DirectedGraph graph, int startVertex, Vector<int>& outCircle)
	{
		m_onStack[startVertex] = true;
		m_marked[startVertex] = true;
		for (const int& adjacentedVertexIndex : graph.GetAdjacentedvertices(startVertex))
		{
			if (!outCircle.Empty())
			{
				return true;
			}
			else if (!IsMarked(adjacentedVertexIndex))
			{
				m_edgeTo[adjacentedVertexIndex] = startVertex;
				DirectedGraphDFS(graph, adjacentedVertexIndex);
			}
			else if (m_onStack[adjacentedVertexIndex])
			{
				for (int index = startVertex; index != adjacentedVertexIndex; index = m_edgeTo[index])
				{
					outCircle.Add(index);
				}
				outCircle.Add(adjacentedVertexIndex);
				outCircle.Add(startVertex);
			}
		}
		m_onStack[startVertex] = false;
		return false;
	}

	bool Circle::IsMarked(int startVertex)
	{
		return m_marked[startVertex];
	}

}