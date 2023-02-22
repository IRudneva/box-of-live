#pragma once
#include <memory>
#include <TGUI/TGUI.hpp>
#include "cell.h"

class Bacterium: public Cell{
public:
	Bacterium(unsigned int id_type)
		:id_type_(id_type)
	{
		setCellType(TypeCell::BACTERIUM);
	}

	void setIdType(unsigned int id) { id_type_ = id; }

	const unsigned int getIdType() { return id_type_; }

	void update() override {}

private:
	unsigned int id_type_ = 0;
};




