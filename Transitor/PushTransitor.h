#pragma once
#include "TransitorBase.h"

/// <summary>
/// ‰æ–ÊØ‚è‘Ö‚¦‰‰o
/// ‰Ÿ‚µo‚µ
/// </summary>
class PushTransitor final : public TransitorBase
{
public:
	PushTransitor(TransitDirection dir = TransitDirection::UP, int interval = 60);
	~PushTransitor();

	void Update() override final;
	void Draw() override final;

private:
	// •ûŒü
	TransitDirection m_direction;
};

