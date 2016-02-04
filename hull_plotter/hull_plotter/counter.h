#ifndef COUNTER_H
#define COUNTER_H

#include <QObject>

class Counter : public QObject
{
    Q_OBJECT
    int m_value = 0;
    const int m_last_generation = 3000000;
    const int m_generations_per_step = 10000;
    bool lastFile = false;
public:
    Counter() {m_value = 0;}
//    Counter(QObject *parent);

    int value() const { return m_value; }
    void last_file_reached();
public slots:
    void setValue(int value);
    void increment();

signals:
    void valueChanged(int newValue);
};

#endif // COUNTER_H

