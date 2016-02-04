#include "counter.h"

/*MyClass(QObject *parent) : QObject(parent), Counter() {
    connect(this, SIGNAL(mySignal()), SLOT(mySlot()));
}*/

void Counter::last_file_reached() {
    lastFile = true;
}

void Counter::setValue(int value)
{
    if (value != m_value) {
        m_value = value;
        emit valueChanged(value);
    }
}

void Counter::increment() {
    /*if(!lastFile) {
        setValue(m_value + m_generations_per_step);
    }*/

    if (m_value + m_generations_per_step < m_last_generation)
    {
        setValue(m_value + m_generations_per_step);
    }
    else if (m_value < m_last_generation)
    {
        setValue(m_last_generation);
    }
}
