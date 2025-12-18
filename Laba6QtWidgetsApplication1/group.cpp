#include "group.h"
#include"Rectangle6.h"
#include<Triangle7.h>
#include<Ellipse6.h>
#include<Line6.h>
#include<Square6.h>
#include<Circle.h>
#include "ShapeFactory.h"
#include<QDebug>


#include<QPainter>
#include<QTextStream>

Group::Group(const QPoint& position) : BaseShape(position)
{
    fillColor = Qt::transparent;
    Colorborder = Qt::red;
    selected = false;
    width = 0;
    height = 0;
}

void Group::draw(QPainter& painter) const
{
    //рисует фон группы
    painter.setBrush(QBrush(fillColor));
    painter.setPen(Qt::NoPen);
    painter.drawRect(getBounRect());

    //рисуем все фигуры в группе
    for (const auto& shape : shapes)
    {
        shape->draw(painter);

    }

    //если группа выделена, рисуем вокруг рамку
    if (selected)
    {
        painter.setBrush(Qt::NoBrush);
            QPen pen(Colorborder);
            pen.setWidth(2);
            pen.setStyle(Qt::DashLine);
            painter.setPen(pen);

            QRect bounds = getBounRect();
            painter.drawRect(bounds);

    }
}

//вычисляем ограничивающий прямоугольник группы
QRect Group::getBounRect() const
{
    if (shapes.empty())
    {
        return QRect(position1.x() - width / 2, position1.y() - height / 2, width, height);

    }

    //Находим общие границы всех фигур в группе
    QRect bounds = shapes[0]->getBounRect();

    for (const auto& shape : shapes) {
        bounds = bounds.united(shape->getBounRect());
    }
    return bounds;
}

//проверяем, попадает ли точка в любую из фигур группы
bool Group::contains(const QPoint& point) const
{
    //проверяем попадает ли точка в любую из фигур 
    for (const auto& shape : shapes)
    {
        if (shape->contains(point))
        {
            return true; //точка попала в одну из фигур
        }
    }
    return false;
}

//перемещаем всю группу на указанное смещение
void Group::move(int dx, int dy)
{
    BaseShape::move(dx, dy);
    //перемещаем фигуры в группе
    for (auto& shape : shapes)  //shape - BaseShape
    {
        shape->move(dx, dy);  //виртуальный вызов
    }

    //обновлем позицию группы
    //position1.setX(position1.x() + dx);
    //position1.setY(position1.y() + dy);

}

//масштабирование всех фигур в группе 
void Group::resize(int newWidth, int newHeight)
{
    QRect currentBounds = getBounRect();

    //проверяем есть ли у группы размер
    if (currentBounds.width() == 0 || currentBounds.height() == 0)
    {
        return; // выходим

    }

    //находим центр группы ДО изменений
    QPoint oldCenter = currentBounds.center();


    //вычисляем коэффициенты масштабирования (во сколько раз увеличиваем или уменьшаем) 
    double scaleX = static_cast<double> (newWidth) / currentBounds.width();
    double scaleY = static_cast<double> (newHeight) / currentBounds.height();

    //масштабируем фигуру
    //перебор фигур 
    for (auto& shape : shapes)
    {
        //границы текущей фигуры
        QRect shapeBounds = shape->getBounRect();
        //центр текущей фигуры
        QPoint shapeCenter = shapeBounds.center();

        // Новая позиция относительно старого центра группы
        int newX = oldCenter.x() + (shapeCenter.x() - oldCenter.x()) * scaleX;
        int newY = oldCenter.y() + (shapeCenter.y() - oldCenter.y()) * scaleY;

        // Новые размеры
        int newShapeWidth = static_cast<int>(shapeBounds.width() * scaleX);
        int newShapeHeight = static_cast<int>(shapeBounds.height() * scaleY);

        //применяем изменения 
        shape->setPosition(QPoint(newX, newY));
        shape->resize(newShapeWidth, newShapeHeight);
    }
    width = newWidth;
    height = newHeight;
    position1 = oldCenter;

    }
    


//добавление фигур в группу

void Group::addShape(std::shared_ptr<BaseShape> shape)  //параметр BaseShape
{
    shapes.push_back(shape); //добавляем группу в вектор
    updateBounds();//обновляем границы группы

}

//удаление фигуры из группы
void Group::removeShape(std::shared_ptr<BaseShape> shapeToRemove)
{
    for (size_t i = 0; i < shapes.size(); i++)
    {
        //сравниваем указатели: эта та же фигура? 
        if (shapes[i].get() == shapeToRemove.get())
        {
            //нашли
            shapes.erase(shapes.begin() + i);
            break;
        }
    }
    //updateBounds();
    
 }


std::vector<std::shared_ptr<BaseShape>> Group::getShapes() const
{
    return shapes; //возвращаем копию вектора 
}

//обновляем границы после изменений
void Group::updateBounds()
{
    //Обновляем размеры и позицию группы
    QRect bounds = getBounRect(); 
    position1 = bounds.center();
    width = bounds.width();
    height = bounds.height();
}

//ЛР6 

//сохраняем группу и все ее фигуры в файл
void Group::save(QTextStream& out) const
{
    // 1. Сохраняем основные свойства группы
    out << "Group "
        << position1.x() << " "
        << position1.y() << " "
        << width << " "
        << height << " "
        << fillColor.name() << " "
        << Colorborder.name() << " "
        << (selected ? "1" : "0") << " "
        << shapes.size() << "\n";
    //созраняем все фигуры в группе
    for (const auto& shape : shapes)
    {
        shape->save(out); //рекурсивное сохранение
    } 
}


void Group::load(QTextStream& in)
{
    //очищаем текущие фигуры 
    shapes.clear();

    //читаем количество вершин
    
    int x, y; //координаты
    int  groupwidht, groupheight;
    QString fillName; //имя цвета заливки
    QString borderName; //имя цвета границы
    int selectedFlag; 
    int shapesCount; //количество фигур в группе

    in >> x >> y>> groupwidht >> groupheight >> fillName >> borderName >> selectedFlag>>shapesCount;
    in.readLine();
    
    // Устанавливаем свойства
    position1 = QPoint(x, y);
    width = groupwidht;
    height = groupheight;
    fillColor = QColor(fillName);
    Colorborder = QColor(borderName);
    selected = (selectedFlag == 1);


    //читаем каждую фигуру в группе
    for (int i = 0; i < shapesCount; ++i) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) break;

        // Определяем тип фигуры (первое слово)
        QStringList parts = line.split(' ', Qt::SkipEmptyParts);
        if (parts.isEmpty()) continue;

        QString type = parts[0]; //тип фигуры


       //для создания фигуры
        std::shared_ptr<BaseShape> shape = ShapeFactory::createShape(type);

        if (shape)
        {
            QTextStream shapeStream(&line);
            //загружаем фигуры 
            shape->load(in);
            addShape(shape);

        }
       

        /*
        if (type == "Circle") {
            shape = std::make_shared<Circle>();
        }
        else if (type == "Rectangle") {
            shape = std::make_shared<Rectangle>();
        }
        else if (type == "Square") {
            shape = std::make_shared<Square>();
        }
        else if (type == "Triangle") {
            shape = std::make_shared<Triangle>();
        }
        else if (type == "Ellipse") {
            shape = std::make_shared<Ellipse>();
        }
        else if (type == "Line") {
            shape = std::make_shared<Line>();
        }
        else if (type == "Group") {
            shape = std::make_shared<Group>();
            shape->load(in);  // Рекурсивная загрузка вложенных групп
            addShape(shape);
            continue;
        }
        else {
            continue;
        }

        if (shape) {
            shape->load(in);
            addShape(shape);
        }
        */
    }
    updateBounds(); //обновляем границы группы

}


//создаем копию фигуры
std::shared_ptr<BaseShape>Group::clone() const
{
    //создаем новую группу
    auto newGroup = std::make_shared<Group>();

    // Клонируем все фигуры
    for (const auto& shape : shapes) {
        newGroup->addShape(shape->clone());  // Каждая фигура клонирует себя
    }

    // Копируем свойства
    newGroup->setfillColor(fillColor);
    newGroup->setColorborder(Colorborder);
    newGroup->setSelected(selected);
    newGroup->updateBounds();

    return newGroup;
}

//метод контроля границ группы
void Group::adjustToBounds(const QRect& allowedArea)
{
    // Получаем границы всей группы
    QRect bounds = getBounRect();

    // Если группа пустая - выходим
    if (bounds.isEmpty()) return;

    // Вычисляем, насколько группа выходит за границы
    int overflowLeft = allowedArea.left() - bounds.left();
    int overflowRight = bounds.right() - allowedArea.right();
    int overflowTop = allowedArea.top() - bounds.top();
    int overflowBottom = bounds.bottom() - allowedArea.bottom();

    // Вычисляем смещение для корректировки
    int dx = 0;
    int dy = 0;

    // Корректируем по горизонтали
    if (overflowLeft > 0) {
        // Вылезли за левую границу - двигаем ВПРАВО
        dx = overflowLeft;
    }
    else if (overflowRight > 0) {
        // Вылезли за правую границу - двигаем ВЛЕВО
        dx = -overflowRight;
    }

    // Корректируем по вертикали
    if (overflowTop > 0) {
        // Вылезли за верхнюю границу - двигаем ВНИЗ
        dy = overflowTop;
    }
    else if (overflowBottom > 0) {
        // Вылезли за нижнюю границу - двигаем ВВЕРХ
        dy = -overflowBottom;
    }

    // Если нужно корректировать - перемещаем всю группу
    if (dx != 0 || dy != 0) {
        move(dx, dy);
    }
}

void Group::setfillColor(const QColor& color)
{
    BaseShape::setfillColor(color);

    for (auto& shape : shapes)
    {
        shape->setfillColor(color);
    }
}