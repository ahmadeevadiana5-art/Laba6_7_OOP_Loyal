#include "ArrowShape.h"
#include "BaseShape6.h"
#include <QPainter>
#include <QTextStream>
#include <algorithm>

ArrowShape::ArrowShape()
    :BaseShape(QPoint(100, 100)),
    m_source(nullptr),
    m_target(nullptr),
    m_arrowColor(Qt::red),
    m_sourceId(-1),
    m_targetId(-1), m_hasSavedPoints(false)
{
    // Инициализация базовых полей
    fillColor = Qt::transparent;
    Colorborder = Qt::red;
    selected = false;
    width = 100;
    height = 50;
    m_lastSourcePos = QPoint(0, 0);
}

ArrowShape::ArrowShape(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target)
  
    : BaseShape(QPoint(0, 0)),
    m_source (source),
    m_target (target),
    m_arrowColor(Qt::red),
    m_sourceId(- 1),
    m_targetId(-1),
    m_hasSavedPoints(false)
{


    // Инициализация базовых полей
    fillColor = Qt::transparent;
    Colorborder = Qt::red;
    selected = false;
    width = 0;
    height = 0;

    if (source && target)
    {
        m_lastSourcePos = source->getCenter();
        source->addObserver(this);
        updatePosition();
    }
    else
    {
        width = 100;
        height = 50;
    }
}

ArrowShape::~ArrowShape()
{
    if (m_source)
    {
        m_source->removeObserver(this);
    }
}


void ArrowShape::draw(QPainter& painter) const
{
    if (!m_source || !m_target) //return;
    {
        painter.save();
        QPen arrowPen(m_arrowColor, 3);
        painter.setPen(arrowPen);
        painter.setBrush(m_arrowColor);
        QPointF start(position1.x() - width / 2, position1.y());
        QPointF end(position1.x() + width / 2, position1.y());
        painter.drawLine(start, end);

        const qreal arrowSize = 12.0;
        QPointF direction(1.0, 0.0);
        QPointF perpendicular(-direction.y(), direction.x());
        QPointF arrowP1 = end - direction * arrowSize + perpendicular * arrowSize * 0.3;
        QPointF arrowP2 = end - direction * arrowSize - perpendicular * arrowSize * 0.3;

        QPolygonF arrowHead;
        arrowHead << end << arrowP1 << arrowP2;
        painter.drawPolygon(arrowHead);

        painter.restore();
        return;
    
    }
    painter.save();
    QPen arrowPen(m_arrowColor, 3);  
    painter.setPen(arrowPen);
    painter.setBrush(m_arrowColor);



    //Точки соединения
    QPointF startPoint = calculateConnectionPoint(m_source, true);
    QPointF endPoint = calculateConnectionPoint(m_target, false);

    //рисуем линию
    painter.drawLine(startPoint, endPoint);

    //рисуем наконечник для стрелки
    const qreal arrowSize = 12.0;
    QPointF direction = endPoint - startPoint;
    qreal length = qSqrt(direction.x() * direction.x() + direction.y() * direction.y());

    if (length > 0)
    {
        direction /= length;
        QPointF perpendicular(-direction.y(), direction.x());

        QPointF arrowP1 = endPoint - direction * arrowSize + perpendicular * arrowSize * 0.3;
        QPointF arrowP2 = endPoint - direction * arrowSize - perpendicular * arrowSize * 0.3;

        QPolygonF arrowHead;
        arrowHead << endPoint << arrowP1 << arrowP2;
        painter.drawPolygon(arrowHead);
    }

    painter.restore();
}




QRect ArrowShape::getBounRect() const
{
    if (!m_source || !m_target) 
    
    {
        //если нет связей, используем базовые координаты
        return QRect(position1.x(), position1.y(), width, height);
    }

    //Гринцы стрелки между объектами
    QPointF startPoint = calculateConnectionPoint(m_source, true);
    QPointF endPoint = calculateConnectionPoint(m_target, false);

    int left = static_cast<int>(std::min(startPoint.x(), endPoint.x()));
    int top = static_cast<int>(std::min(startPoint.y(), endPoint.y()));
    int right = static_cast<int>(std::max(startPoint.x(), endPoint.x()));
    int bottom = static_cast<int>(std::max(startPoint.y(), endPoint.y()));

    //отступы для стрелки
    return QRect(left - 10, top - 10, right - left + 20, bottom - top + 20);
}



bool ArrowShape::contains(const QPoint& point) const
{
    QRect bounds = getBounRect();
    return bounds.contains(point);
}






void ArrowShape::move(int dx, int dy)
{
    Q_UNUSED(dx);
    Q_UNUSED(dy);
}

void ArrowShape::resize(int newWidth, int newHeight)
{
    Q_UNUSED(newWidth);
    Q_UNUSED(newHeight);
}

void ArrowShape::adjustToBounds(const QRect& allowedArea)
{
    //стрелка следует за объектами
    Q_UNUSED(allowedArea);
}


//СОХРАНЕНИЕ И ЗАГРУЗКА
void ArrowShape::save(QTextStream& out) const
{
    out << "Arrow ";

    // Всегда сохраняем координаты как у обычной фигуры
    out << position1.x() << " " << position1.y() << " ";
    out << width << " " << height << " ";

    // Остальные параметры
    out << fillColor.name() << " ";
    out << Colorborder.name() << " ";
    out << m_arrowColor.name() << " ";
    out << (selected ? "1" : "0") << " ";
    out << m_sourceId << " " << m_targetId << "\n";
}

void ArrowShape::load(QTextStream& in)
{
    
    QString type;
    int x, y, w, h;
    QString fillName, borderName, colorName;
    int selectedFlag;

    in >> type >> x >> y >> w >> h >> fillName >> borderName >> colorName >> selectedFlag >> m_sourceId >> m_targetId;

    position1 = QPoint(x, y);
    width = w;
    height = h;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    m_arrowColor = QColor(colorName);
    selected = (selectedFlag == 1);

    m_source = nullptr;
    m_target = nullptr;
}

std::shared_ptr<BaseShape> ArrowShape::clone() const
{
    //создаем копию стрелки
    auto newArrow = std::make_shared<ArrowShape>(m_source, m_target);

    //копируем свойства
    newArrow->position1 = this->position1;
    newArrow->width = this->width;
    newArrow->height = this->height;
    newArrow->fillColor = this->fillColor;
    newArrow->Colorborder = this->Colorborder;
    newArrow->selected = this->selected;
    newArrow->m_arrowColor = this->m_arrowColor;
    newArrow->m_sourceId = this->m_sourceId;
    newArrow->m_targetId = this->m_targetId;
    newArrow->m_lastSourcePos = this->m_lastSourcePos;
    newArrow->m_savedStartPoint = this->m_savedStartPoint;
    newArrow->m_savedEndPoint = this->m_savedEndPoint;
    newArrow->m_hasSavedPoints = this->m_hasSavedPoints;


    return newArrow;
}


void ArrowShape::update()
{
    if (m_source && m_target) 
    {
        QPoint currentPos = m_source->getCenter();
        QPoint offset = currentPos - m_lastSourcePos;

        QPoint targetPos = m_target->getCenter();
        m_target->setCenter(targetPos + offset);
        
        m_lastSourcePos = currentPos;
        updatePosition();

    }
}

void ArrowShape::updateArrowPositionAndSize()
{
    if (m_source && m_target)
    {
        QPoint sourceCenter = m_source->getCenter();
        QPoint targetCenter = m_target->getCenter();

        // Позиция стрелки - середина между объектами
        position1 = QPoint((sourceCenter.x() + targetCenter.x()) / 2,
            (sourceCenter.y() + targetCenter.y()) / 2);

        // Размеры стрелки
        width = qAbs(targetCenter.x() - sourceCenter.x()) + 40;
        height = qAbs(targetCenter.y() - sourceCenter.y()) + 40;
    }
}


//для стрелки 
void ArrowShape::setSource(std::shared_ptr<BaseShape> source)
{
    if (m_source) 
    {
        m_source->removeObserver(this);
    }
    //новый источнки
    m_source = source;
    if (m_source) {
        m_source->addObserver(this);
        m_lastSourcePos = m_source->getCenter();
        updatePosition();
    }
}

void ArrowShape::setTarget(std::shared_ptr<BaseShape> target)
{
    m_target = target;
    if (m_source && m_target)
    {
        updatePosition();
    }
}

QPointF ArrowShape::calculateConnectionPoint(std::shared_ptr<BaseShape> shape, bool isSource) const
{
    if (!shape) return QPointF();

    QRect bounds = shape->getBounRect();
    if (isSource)
    {
        //соединяем с правой стороны источника
        return QPointF(bounds.right(), bounds.center().y());
    }
    else
    {
        return QPointF(bounds.left(), bounds.center().y());
    }
}

void ArrowShape::updatePosition()
{
    if (m_source && m_target)
    {
        // Устанавливаем позицию стрелки как середину между объектами
       /* QPoint sourceCenter = m_source->getCenter();
        QPoint targetCenter = m_target->getCenter();*/

        QPointF startPoint = calculateConnectionPoint(m_source, true);
        QPointF endPoint = calculateConnectionPoint(m_target, false);

        position1 = QPoint((startPoint.x() + endPoint.x()) / 2,
            (startPoint.y() +endPoint.y()) / 2);

        // Размеры стрелки - расстояние между точками плюс отступы для стрелки
            width = (int)qAbs(endPoint.x() - startPoint.x()) + 40; // +20 с каждой стороны
        height = (int)qAbs(endPoint.y() - startPoint.y()) + 40;

        if (m_source) {
            m_lastSourcePos = m_source->getCenter();
        }

        /*
        // Обновляем размеры
        QRect bounds = getBounRect();
        width = bounds.width();
        height = bounds.height();
        */
    }
}










