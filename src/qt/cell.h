#ifndef SQUAREZ_QT_CELL_H
#define SQUAREZ_QT_CELL_H

#include <QObject>

namespace squarez {
namespace qt {

class Cell : public QObject
{
	Q_OBJECT
	Q_PROPERTY(int x READ getX NOTIFY xChanged)
	Q_PROPERTY(int y READ getY NOTIFY yChanged)
	Q_PROPERTY(unsigned int symbol READ getSymbol CONSTANT)
public:
	Cell(int x, int y, unsigned int symbol, QObject *parent = 0);

	int getX() const { return _x;}
	int getY() const { return _y;}

	void setX(int x);
	void setY(int y);
	unsigned int getSymbol() const { return _symbol;}
private:
	int _x;
	int _y;
	unsigned int _symbol;

signals:
	void xChanged(int x);
	void yChanged(int y);

public slots:

};

} // namespace qt
} // namespace squarez

#endif // SQUAREZ_QT_CELL_H
