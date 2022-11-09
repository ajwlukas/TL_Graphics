#pragma once

#include <functional>
#include <queue>

/// <summary>
/// Canvas���� Layer�� ������ �� Object
/// </summary>
class CanvasManager
{
public:
	CanvasManager();
	~CanvasManager();

	using Rend = pair <UINT, function<void()>>;

	//void Reserve(pair<UINT, function<void()>> drawCall);
	void Reserve(Rend drawCall);

	void Render();


	struct comparePair {
		bool operator()(const Rend& lhs, const Rend& rhs) const {
			return lhs.first > rhs.first;
		};
	};

	//priority_queue<Rend, vector<Rend>, comparePair> renderQueue;
	priority_queue<Rend, vector<Rend>, comparePair> renderQueue;
};