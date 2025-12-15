
#include "ArrowObserver.h"
#include"BaseShape6.h"
#include <QPainter>
#include<QtMath>
#include <QDebug>

ArrowObserver::ArrowObserver(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target, QObject* parent) :
	QObject(parent), m_source(source), m_target(target), m_color(Qt::red)
{
    //проверяем что объекты существуют
    if (!m_source || !m_target) 
    {
         return;
    }

    //сохраняем начальную позицию источника 
    m_lastSourcePos = m_source->getCenter();
    m_source->addObserver(this);
}

ArrowObserver::~ArrowObserver()
{
    if (m_source)
    {
        m_source->removeObserver(this);

    }
}

//основной меод паттерна Observer
void ArrowObserver::update()
{
    //вызывается автоматически при перемещении объекта-источника
    if (!m_source || !m_target)
    {
        return;

    }
    // 1. Получаем текущую позицию источника
    QPoint currentSourcePos = m_source->getCenter();

    // 2. Вычисляем смещение (разницу между текущей и предыдущей позицией)
    QPoint offset = currentSourcePos - m_lastSourcePos;
    // 3. Перемещаем цель на то же смещение
    QPoint targetPos = m_target->getCenter();
    m_target->setCenter(targetPos + offset);

    // 4. Обновляем последнюю позицию источника
    m_lastSourcePos = currentSourcePos;

    // 5. Сигнализируем об обновлении (для перерисовки сцены)
    emit arrowUpdated();
}

//рисуем стрелку 
void ArrowObserver::draw(QPainter& painter)
const
{
    if (!m_source || !m_target)
        return;

    painter.save();

    //настраиваем стрелки
    QPen arrowPen(m_color, 3); //красная толщина 3
    arrowPen.setStyle(Qt::SolidLine);
    arrowPen.setCapStyle(Qt::RoundCap);

        //заливка стрелки 
    painter.setPen(arrowPen);
    painter.setBrush(m_color);

    //точки для соединения 
    QPointF startPoint = calculateConnectionPoint(m_source, true);
    QPointF endPoint = calculateConnectionPoint(m_target, false);

    //рисуем линию от начала до конца 
    painter.drawLine(startPoint, endPoint);

    //размер стрелки 
    const qreal arrowSize = 12.0;

    //вычисляем направление 
    QPointF direction = endPoint - startPoint;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    //проверяем длину линии между объектами
    if (length > 0)
    {
        //вектор показывает напрвление, но не расстояние 
        direction /= length;
        //вектор перпендикулярный
        QPointF perpendicular(-direction.y(), direction.x());

        //вычисление точек треугольника вектора 
        QPointF arrowP1 = endPoint - direction * arrowSize + perpendicular * arrowSize * 0.3; //0.3 ДЛЯ ОПРЕДЕЛЕНИЯ ШИРИНЫ СТРЕЛКИ 
        QPointF arrowP2 = endPoint - direction * arrowSize - perpendicular * arrowSize * 0.3;

        QPolygonF arrowHead;
        arrowHead << endPoint << arrowP1 << arrowP2;


        //рисуем и заливаем треугольник стрелки 
        painter.drawPolygon(arrowHead);
        painter.setBrush(m_color);  // Устанавливаем кисть для заливки
        painter.drawPolygon(arrowHead);  // Рисуем залитый полигон
        //painter.fillPolygon(arrowHead, m_color);

    }

    painter.restore();

}

//проверка попадания точки на стрелку 
bool ArrowObserver::containsPoint(const QPoint& point) const
{
    if (!m_source || !m_target)
        return false;

    //получаем точки соединения 
    QPointF startPoint = calculateConnectionPoint(m_source, true);
    QPointF endPoint = calculateConnectionPoint(m_target, false);

    //поиск середины стрелки 
    QPointF middlePoint = (startPoint + endPoint) / 2;

    //расстояние от точки до середины стрелки 
    qreal distance = QLineF(middlePoint, point).length();

    return distance < 15.0;

}

//вычисление точки соединения на фигуре 
QPointF ArrowObserver::calculateConnectionPoint(std::shared_ptr<BaseShape> shape, bool isSource) const
{
    if (!shape)
        return QPointF();

    //получаем ограничивающий прямоугольник фигуры
    QRect bounds = shape->getBounRect();

    if (isSource)
    {
        //выходим с правой стороны 
        return QPointF(bounds.right(), bounds.center().y());

    }
    else
    {
        return QPointF(bounds.left(), bounds.center().y());
    }
}

/*
// Изменение объекта
void ArrowObserver::setSource(std::shared_ptr<BaseShape> source)
{
    if (m_source)
        m_source->removeObserver(this);

    //устанавливаем новй 
    m_source = source;
    if (m_source)
    {
        m_source->addObserver(this);
        m_lastSourcePos = m_source->getCenter(); //обновляем 
    }
}



//изменения объекта 
void ArrowObserver::setTarget(std::shared_ptr<BaseShape> target)
{
    m_target = target;
}*/