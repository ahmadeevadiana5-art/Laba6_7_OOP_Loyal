#include "baseshape6.h"
#include<algorithm>

BaseShape::BaseShape(const QPoint& position)
	: position1(position), width(40), height(40), fillColor(Qt::blue), Colorborder(Qt::black), selected(false)
{
}

//перемещение фигуры
void BaseShape::move(int dx, int dy)
{
	position1.setX(position1.x() + dx);
	position1.setY(position1.y() + dy);
	notifyObservers();  //  уведомляем всех наблюдателей
}

//изменение размера

void BaseShape::resize(int newwidth, int newheight)
{
	width = newwidth;
	height = newheight;
	notifyObservers();
}

//установка позиция
void BaseShape::setPosition(const QPoint& newPosit)
{
	//position1 = newPosit;
	// ЛР7: При изменении позиции тоже уведомляем наблюдателей
	int dx = newPosit.x() - position1.x();
	int dy = newPosit.y() - position1.y();
	move(dx, dy);  // Используем move для уведомления наблюдателей

}

//Получение позиции
QPoint BaseShape::getPosition() const
{
	return position1;


}

//установка цвета заливки 
void BaseShape::setfillColor(const QColor& color)
{
	fillColor = color;
	notifyObservers();
}

//установка цвета границы 
void BaseShape::setColorborder(const QColor& color)
{
	Colorborder = color;
	notifyObservers();
}

//получение цвета заливки
QColor BaseShape::getfillColor() const
{
	return fillColor;

}

//получение цвета границы 
QColor BaseShape::getColorborder() const
{
	return Colorborder;
}

// установка флага выделения 
bool BaseShape::isSelected() const
{
	return selected;
}

//проверяем выделение находится ли в границах 
bool BaseShape::isOutOfBounds(const QRect& allowedArea) const
{
	//прямоугольник который полностью описывает фигуру
	QRect bounds = getBounRect();
	//проверяем полностью ли фигура находится в разрешенной области
	bool isFullyInside = allowedArea.contains(bounds);
	//узнаем вышла ли фигура из области или осталась внутри
	return !isFullyInside;

}

//корректируем позицию фигуры 
void BaseShape::adjustToBounds(const QRect& allowedArea)
{
	QRect bounds = getBounRect();

	int dx = 0; 
	//левая граница
	if (bounds.left() < allowedArea.left())
	{
		/*
		//вычисляем насколько мы вылезаем за левую границу
		int overflowAmount = allowedArea.left() - bounds.left();
		int newX = position1.x() + overflowAmount;
		position1.setX(newX);
		*/
		dx = allowedArea.left() - bounds.left();
	}
	else if (bounds.right() > allowedArea.right())
	{
		/*
		int overflowAmount = bounds.right() - allowedArea.right();
		int newX = position1.x() - overflowAmount;
		position1.setX(newX);
		*/
		dx = allowedArea.right() - bounds.right();
	}


	int dy = 0;

	//верхняя граница 
	if (bounds.top() < allowedArea.top())
	{
		/*
		int overflowAmount = allowedArea.top() - bounds.top();
		int newY = position1.y() + overflowAmount;
		position1.setY(newY);*/

		dy = allowedArea.top() - bounds.top();

	}

	//правая граница 

	//нижняя граница 
	if (bounds.bottom() > allowedArea.bottom())
	{
		/**
		int overflowAmount = bounds.bottom() - allowedArea.bottom();

		// Сдвигаем фигуру ВВЕРХ на вычисленное количество пикселей
		int newY = position1.y() - overflowAmount;
		position1.setY(newY);*/
		dy = allowedArea.bottom() - bounds.bottom();
	}
	if (dx != 0 || dy != 0) {
		move(dx, dy);
	}
}

void  BaseShape::setSelected(bool isSelected)
{
	selected = isSelected;
	notifyObservers();
}

// Реализация методов для паттерна Observer
QPoint BaseShape::getCenter() const {
	return position1 +QPoint(width/2, height/2);
}

void BaseShape::setCenter(const QPoint& center) 
{
	/*
	QPoint currentCenter = getCenter();

	QPoint offset = center - position1;
	move(offset.x(), offset.y());*/
	//position1 = center;
	position1 = QPoint(center.x() - width / 2,
		center.y() - height / 2);
	notifyObservers();

}
void BaseShape::save(QTextStream& out) const
{
	out << getType() << " "
		<< position1.x() << " "
		<< position1.y() << " "
		<< width << " "
		<< height << " "
		<< fillColor.name() << " "
		<< Colorborder.name() << " "
		<< (selected ? "1" : "0");
}


void BaseShape::load(QTextStream& in)
{
	QString type;
	int x, y, w, h;
	QString fillName, borderName;
	int selectedFlag;

	in >> type >> x >> y >> w >> h >> fillName >> borderName >> selectedFlag;

	position1 = QPoint(x, y);
	width = w;
	height = h;
	fillColor = QColor(fillName);
	Colorborder = QColor(borderName);
	selected = (selectedFlag == 1);
}