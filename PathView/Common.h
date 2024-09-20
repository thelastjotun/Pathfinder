#pragma once

#include <QRectF>

static constexpr quint32 INF = 1e9;

class MyRect : public QRectF
{
public:
    MyRect() {};

    void setObstacle(const bool &obstacle) { m_isObstacle = obstacle; }
    bool isObstacle() const { return m_isObstacle; }

    void setXIndex(const quint16 &x) { m_xIndex = x; }
    quint16 xIndex() const { return m_xIndex; }

    void setYIndex(const quint16 &y) { m_yIndex = y; }
    quint16 yIndex() const { return m_yIndex; }

    QPoint center() const { return QPoint(x() + width() / 2, y() + height() / 2); }

private:
    quint16 m_xIndex{};
    quint16 m_yIndex{};

    bool m_isObstacle{ false };
};
