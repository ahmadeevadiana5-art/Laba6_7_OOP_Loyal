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
    //отрисовывет группу и ее фигуры 
    void draw(QPainter& painter) const override;

    QRect getBounRect() const override;

    bool contains(const QPoint& point) const override;


    //ЛР6 
    //перемещение всей группы 
    void move(int dx, int dy) override;

    //соответственное масштабирование всех фигур в группе
    void resize(int newWidth, int newHeight) override;

    // Методы для сохранения/загрузки
    void save(QTextStream& out) const override;

    //восстанавливаем группу из текстового формата
    void load(QTextStream& in) override;

    //для управления  фигурами
    std::shared_ptr<BaseShape> clone() const override;

    //метод для контроля границ
    void adjustToBounds(const QRect& allowedArea) override;

    // Специальные методы для группы 
    bool isGroup() const override { return true; }

    // Методы для работы с дочерними фигурами
    void addShape(std::shared_ptr<BaseShape> shape) override;

    //метод для работы с удалением фигур из группы 
    void removeShape(std::shared_ptr<BaseShape> shape) override;

    std::vector<std::shared_ptr<BaseShape>> getShapes() const override;

    // Вспомогательные методы
    //пересчитывает размеры и позицию группы на основе дочерних фигур 
    void updateBounds();

    //проверяет пустая ли группа
    bool isEmpty() const { return shapes.empty(); }

    //возвращает количество фигур в группе
    int getShapeCount() const { return static_cast<int>(shapes.size()); }

    //устанавливает цвет заливки для всех фигур в группе
    void setfillColor(const QColor& color);

    //возвращает строковый идентификатор типа
    QString getType() const override

    {
        return "Group";

    }


    //======= ЛР7: Методы для паттерна Observer
    //вычисляет центр группы как среднее арифмитическое центров всех фигур 
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

    //устанавливае центр группы перемещая все фигуры
    void setCenter(const QPoint& center) override {
        QPoint currentCenter = getCenter();
        QPoint offset = center - currentCenter;
        move(offset.x(), offset.y());
    }




private:
    //vector хранит указатели на базовый класс
    std::vector<std::shared_ptr<BaseShape>> shapes;
};
#endif 