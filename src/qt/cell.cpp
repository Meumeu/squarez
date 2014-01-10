#include "cell.h"

namespace squarez {
namespace qt {

Cell::Cell(int x, int y, unsigned int symbol, QObject *parent) :
	QObject(parent), _x(x), _y(y), _symbol(symbol)
{
}

void Cell::setX(int x)
{
	if (x == _x)
		return;
	_x = x;
	emit xChanged(x);
}

void Cell::setY(int y)
{
	if (y == _y)
		return;
	_y = y;
	emit yChanged(y);
}

} // namespace qt
} // namespace squarez
