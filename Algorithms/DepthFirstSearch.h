#pragma once

#include "Graph.h"

namespace Algorithm
{
	class Search
	{
	public:
		Search(DirectedGraph graph, int startVertex);
		~Search() = default;

		void DepthFirstSearch();
		void BreadthFirstSearch();

		bool HasPathTo(int terminalVertexIndex, Vector<int>& outPath);

	private:
		void DFS(DirectedGraph graph, int startVertex);
		void BFS(DirectedGraph graph, int startVertex);

		bool IsMarked(int startVertex);

		DirectedGraph m_directedGraph;
		Vector<bool> m_marked;
		Vector<int> m_edgeTo;
		int m_originalVertexIndex;
	};

	class Circle
	{
	public:
		Circle(DirectedGraph graph, int startVertex);
		~Circle() = default;
		
		bool DirectedGraphCircle(Vector<int>& outCircle);
	private:
		bool DirectedGraphDFS(DirectedGraph graph, int startVertex, Vector<int>& outCircle);

		bool IsMarked(int startVertex);
		DirectedGraph m_directedGraph;
		Vector<bool> m_marked;
		Vector<int> m_edgeTo;
		Vector<bool> m_onStack;
		int m_originalVertexIndex;
	};
}