#ifndef ARROWSHAPE_H
#define ARROWSHAPE_H

#include "BaseShape6.h"
#include "Observer.h"
#include <QPoint>
#include <memory>

class ArrowShape : public BaseShape, public Observer
{
public:
    ArrowShape();
    ArrowShape(std::shared_ptr<BaseShape> source ,
        std::shared_ptr<BaseShape> target);

    ~ArrowShape() override;

    // BaseShape методы
    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;
    void move(int dx, int dy) override;
    void resize(int newWidth, int newHeight) override;
    void adjustToBounds(const QRect& allowedArea) override;

    // Сохранение/загрузка
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;
    std::shared_ptr<BaseShape> clone() const override;


    QString getType() const override { return "Arrow"; }
    bool isGroup() const override { return false; }

    // Observer метод
    void update() override;

    // Специфичные методы
    void setSource(std::shared_ptr<BaseShape> source);
    void setTarget(std::shared_ptr<BaseShape> target);
    std::shared_ptr<BaseShape> getSource() const { return m_source; }
    std::shared_ptr<BaseShape> getTarget() const { return m_target; }

    // Методы для работы с цветом стрелки
    void setArrowColor(const QColor & color) { m_arrowColor = color; }
    QColor getArrowColor() const { return m_arrowColor; }


    // Методы для индексов (для сохранения/загрузки)
    void setSourceId(int id) { m_sourceId = id; }
    void setTargetId(int id) { m_targetId = id; }
    int getSourceId() const { return m_sourceId; }
    int getTargetId() const { return m_targetId; }



    void updatePosition();

    void updateArrowPositionAndSize();

private:

    std::shared_ptr<BaseShape> m_source; //начало стрелки
    std::shared_ptr<BaseShape> m_target; //конец стрелки
    QPoint m_lastSourcePos;
    QColor m_arrowColor;

    int m_sourceId;
    int m_targetId;
    QPointF calculateBestConnectionPoint(const QRect& bounds, const QPoint& targetPoint) const;


    QPointF calculateConnectionPoint(std::shared_ptr<BaseShape> shape, bool isSource) const;

    QPointF m_savedStartPoint;
    QPointF m_savedEndPoint;
    bool m_hasSavedPoints;


};

#endif // ARROWSHAPE_H