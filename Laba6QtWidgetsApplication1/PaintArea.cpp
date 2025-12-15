#include "PaintArea.h"
#include<QPainter>
#include<QMouseEvent>
#include <QKeyEvent>
#include<QMessageBox>

PaintArea::PaintArea(Storage* storage1, QWidget* parent) :QWidget(parent), storage(storage1), ctrlPressed(false)
{
    setFocusPolicy(Qt::StrongFocus);
    setMinimumSize(400, 300);

    setMouseTracking(true);
}

void PaintArea::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.fillRect(rect(), Qt::white);

    // Сохраняем рабочую область (вся область виджета)
    workArea = rect();

    // Рисуем все фигуры
    auto shapes = storage->getShapes();
    for (const auto& shape : shapes)
    {
        shape->draw(painter);
    }

    //рисуем все стрелки
    drawArrows(painter);

   
}
/*
void PaintArea::drawArrows(QPainter& painter)

{
    const auto& arrows = storage->getArrows();
    for (const auto& arrow : arrows)
    {
        arrow->draw(painter);

    }
}

void drawTemporaryArrows(QPainter& painter)
{
    if (!m_selectedForArrow)
        return;
    painter.save();

    //настройки временной стрелик 
    QPen tempPen(Qt::blue, 2, Qt::DashLine);


}
*/

//метод для создания стрелки 
void PaintArea::createArrowBetweenSelected()
{
    auto selected = storage->getSelectedShapes();

    if (selected.size() != 2)
    {
        QMessageBox::information(this, "Информация", "Для создания стрелки выделите 2 фигуры.");
        return;

    }
    // cоздаем стрелку : первая выделенная->источник, вторая->цель
    auto arrow = storage->createArrow(selected[0], selected[1]);

    if (arrow) {
        update();  // Перерисовываем
    }
}

void  PaintArea::mousePressEvent(QMouseEvent* event)
{
    //создание стрелки
    if (event->button() == Qt::LeftButton && m_isCreatingArrow)
    {
        //просто создаем стрелку при клике на второй объект 
        auto shapes = storage->getShapes();
        for (const auto& shape : shapes)
        {
            if (shape && shape->contains(event->pos()) && shape != m_selectedForArrow)
            {

                // Создаем стрелку
                auto arrow = storage->createArrow(m_selectedForArrow, shape);
                if (arrow) {
                    QMessageBox::information(this, "Стрелка создана",
                        QString("Создана стрелка: %1 → %2\n"
                            "При перемещении первого объекта второй будет следовать за ним.")
                        .arg(m_selectedForArrow->getType())
                        .arg(shape->getType()));
                }

                //storage->createArrow(m_selectedForArrow, shape);
                m_isCreatingArrow = false;
                m_selectedForArrow = nullptr;
                update();
                return;

            }
        
    }
    //QPoint clicksPos = event->pos();
    // Если кликнули не по фигуре - отменяем создание
    m_isCreatingArrow = false;
    m_selectedForArrow = nullptr;
    update();
    return;  // Важно вернуться, чтобы не выполнять код ниже
    }

    //метод вызываем при нажатии клавиши мыши выделение фигур
    if (event->button() == Qt::LeftButton)
    {
        QPoint clickPos = event->pos();
        bool found = false;
        int count = storage->getCount();

        //идем от последней фигуры к первой
        for (int i = count - 1; i >= 0; --i)
        {
            //получение фигуры
            auto shape = storage->getObject(i);
            //проверяем сущесвует ли фигура и находится ли точка клика внутри фигуры 
            if (shape && shape->contains(clickPos))
            {
                if (!ctrlPressed)
                {
                    //отменяем выделение
                    for (int j = 0; j < count; ++j)
                    {
                        auto currentShape = storage->getObject(j);
                        currentShape->setSelected(false);

                    }
                }
                shape->setSelected(true);
                found = true;

                //==========ЛР 6 если это группа, не выходим сразу
                if (!shape->isGroup())
                {
                    break;
                }
                
                //break;

            }
        }

        //если не нашли объект 
        if (!found && !ctrlPressed)
        {
            for (int i = 0; i < count; ++i) {
                auto shape = storage->getObject(i);
                shape->setSelected(false);
            }
        }
        update();
        setFocus();
        emit selectionChanged();
    }
}

void PaintArea::keyPressEvent(QKeyEvent* event)
{
    int step = 5;
    bool needUpdate = false;

    switch (event->key())
    {
    case Qt::Key_Control:
        ctrlPressed = true;
        break;

        //===============ЛР6 Группировка/Разгруппировка
    case Qt::Key_G:
        if (event->modifiers() & Qt::ControlModifier)
        {
            if (event->modifiers() & Qt::ShiftModifier)
            {
                //ctrl+shift+G Разгруппировать
                emit ungroupRequested();
            }
            else
            {
                // Ctrl+G - сгруппировать
                emit groupRequested();    // Можно сделать сигнал
            }
            needUpdate = true;
        }
        break;


        // ПЕРЕМЕЩЕНИЕ - Стрелки
    case Qt::Key_Left:
        if (event->modifiers() & Qt::ControlModifier)
        {
            handleResizing(-step, 0);
        }
        else
        {
            handleMovement(-step, 0);
        }

        needUpdate = true;
        break;

    case Qt::Key_Right:
        if (event->modifiers() & Qt::ControlModifier)
        {
            handleResizing(step, 0);
        }
        else
        {
            handleMovement(step, 0);
        }
        needUpdate = true;
        break;


        //ВВерх
    case Qt::Key_Up:
        if (event->modifiers() & Qt::ControlModifier)
        {
            handleResizing(0, -step);
        }
        else
        {
            handleMovement(0, -step);
        }
        needUpdate = true;
        break;


        //вниз
    case Qt::Key_Down:
        if (event->modifiers() & Qt::ControlModifier)
        {
            handleResizing(0, step);
        }
        else
        {
            handleMovement(0, step);
        }
        needUpdate = true;
        break;

        // УДАЛЕНИЕ - Delete
    case Qt::Key_Delete:
        storage->removeSelected();
        needUpdate = true;
        break;
    default:
        QWidget::keyPressEvent(event);
        return;
    }
    if (needUpdate) {
        update();
    }
}

void PaintArea::keyReleaseEvent(QKeyEvent* event) {
    if (event->key() == Qt::Key_Control) {
        ctrlPressed = false;
    }
    QWidget::keyReleaseEvent(event);
}

void PaintArea::handleMovement(int dx, int dy) {
    storage->moveSelected(dx, dy, workArea);
}

void PaintArea::handleResizing(int dw, int dh) {
    storage->resizeSelected(dw, dh, workArea);
}

void PaintArea::drawArrows(QPainter& painter) 
{
    const auto& arrows = storage->getArrows();
    for (const auto& arrow : arrows) 
    {
        arrow->draw(painter);
    }
}

void PaintArea::startCreatingArrow()
{
    //получаем выделенные фигуры 
    auto selected = storage->getAllSelectedShapes();

    if (selected.empty())
    {
        QMessageBox::information(this, "Информация", "Выделите объект.");
        return;

    }

    if (selected.size() > 1)
    {
        QMessageBox::information(this, "Информация",
            "Выделите только один объект.");
        return;
    }
    //сохраняем выбранную фигуру
    m_selectedForArrow = selected[0];
    m_isCreatingArrow = true;

    // Сообщение пользователю
    QMessageBox::information(this, "Создание стрелки",
        QString("Выбран источник: %1\n"
            "Теперь кликните на объект, который должен следовать за ним.")
        .arg(m_selectedForArrow->getType()));

    update();  // Перерисовываем
}