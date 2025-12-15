#ifndef ARROWOBSERVER_H
#define ARROWOBSERVER_H

#include <vector>
#include <memory>
#include<algorithm>
#include"Observer.h"
#include<QPoint>
#include<QPointF>
#include<QObject>
#include "BaseShape6.h"



class ArrowObserver :public QObject, public Observer {
    Q_OBJECT
public: 
    explicit ArrowObserver(std::shared_ptr<BaseShape> source, std::shared_ptr<BaseShape> target, QObject* parent = nullptr);
    ~ArrowObserver();

    void update() override;

   // std::shared_ptr<BaseShape> getSource() const { return m_source; }
    //std::shared_ptr<BaseShape> getTarget() const { return m_target; }

    void draw(QPainter& painter) const;
    bool containsPoint(const QPoint& point) const;

    QColor getColor() const { return m_color; }

signals:
    void arrowUpdated();

private:
    std::shared_ptr<BaseShape> m_source;
    std::shared_ptr<BaseShape> m_target;
    QPoint m_lastSourcePos;
    QColor m_color;

    QPointF calculateConnectionPoint(std::shared_ptr<BaseShape> shape,
        bool isSource) const;
};

/*
//кто следит за изменениями
class Observer 
{   
public:
    virtual ~Observer() = default;
    virtual void update() = 0;  // Вызывается при изменении состояния
};


//за кем следят 
class Observable {
private:
    std::vector<Observer*> observers;

public:
    virtual ~Observable() = default;

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer),
            observers.end());
    }

    void notifyObservers() {
        for (Observer* observer : observers) {
            observer->update();
        }
    }

    bool hasObservers() const
    {
        return !observers.empty();
    }
};
*/

#endif // OBSERVER_H