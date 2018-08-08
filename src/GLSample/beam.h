#pragma once

#include "Line.h"

class beam
{
public:
	beam(void);
	beam(CLine beamLine);
	~beam(void);

private:
	CLine m_beamLine;
};
