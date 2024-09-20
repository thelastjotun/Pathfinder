#include "FieldGenerator.h"

#include <QDebug>
#include <QRandomGenerator64>

FieldGenerator::FieldGenerator(QObject *parent)
    : QObject{ parent }
{}

void FieldGenerator::setRect(const QRectF &rect, const quint16 &xIndex, const quint16 &yIndex)
{
    m_field[yIndex][xIndex].setRect(rect.x(), rect.y(), rect.width(), rect.height());
    m_field[yIndex][xIndex].setXIndex(xIndex);
    m_field[yIndex][xIndex].setYIndex(yIndex);
}

MyRect FieldGenerator::rect(const quint16 &xIndex, const quint16 &yIndex) const
{
    return m_field[yIndex][xIndex];
}

QVector< QVector< MyRect > > FieldGenerator::field() const
{
    return m_field;
}

quint16 FieldGenerator::width() const
{
    return m_width;
}

quint16 FieldGenerator::height() const
{
    return m_height;
}

void FieldGenerator::generate(const quint16 &width, const quint16 &height)
{
    m_width = width;
    m_height = height;

    m_field.clear();

    m_field.resize(height);

    std::uniform_int_distribution< int > distribution(0, 1);

    for (auto &line : m_field) {
        line.resize(width);

        for (auto &item : line) {
            item.setObstacle(distribution(*QRandomGenerator::global()));
        }
    }
}
