#ifndef GROUP_H
#define GROUP_H
#include "baseshape6.h"
#include< vector>
#include< memory>
#include<algorithm>

class Group : public BaseShape
{
public:
	Group(const QPoint& position = QPoint(0, 0));

    // Переопределенные методы BaseShape
    void draw(QPainter& painter) const override;
    QRect getBounRect() const override;
    bool contains(const QPoint& point) const override;

    void move(int dx, int dy) override;
    void resize(int newWidth, int newHeight) override;

    // Методы для сохранения/загрузки
    void save(QTextStream& out) const override;
    void load(QTextStream& in) override;

    std::shared_ptr<BaseShape> clone() const override;

    void adjustToBounds(const QRect& allowedArea) override;

    // Специальные методы для группы 
    bool isGroup() const override { return true; }

    // Методы для работы с дочерними фигурами
    void addShape(std::shared_ptr<BaseShape> shape) override;
    void removeShape(std::shared_ptr<BaseShape> shape) override;
    std::vector<std::shared_ptr<BaseShape>> getShapes() const override;

    // Вспомогательные методы
    void updateBounds();
    bool isEmpty() const { return shapes.empty(); }
    int getShapeCount() const { return static_cast<int>(shapes.size()); }

    void setfillColor(const QColor& color);


    QString getType() const override

    {
        return "Group";

    }


    //======= ЛР7: Методы для паттерна Observer
    QPoint getCenter() const override 
    {
        if (shapes.empty()) return position1;

        // Центр группы - среднее центров всех фигур
        int sumX = 0, sumY = 0;
        for (const auto& shape : shapes) {
            QPoint center = shape->getCenter();
            sumX += center.x();
            sumY += center.y();
        }

        return QPoint(sumX / static_cast<int>(shapes.size()), sumY / static_cast<int>(shapes.size()));
    }

    void setCenter(const QPoint& center) override {
        QPoint currentCenter = getCenter();
        QPoint offset = center - currentCenter;
        move(offset.x(), offset.y());
    }



private:
    std::vector<std::shared_ptr<BaseShape>> shapes;
};
#endif 