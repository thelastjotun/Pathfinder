#pragma once

#include <QObject>
#include <QRectF>
#include <QVector>

#include "Common.h"

class FieldGenerator : public QObject
{
    Q_OBJECT
public:
    explicit FieldGenerator(QObject *parent = nullptr);

    ///
    /// \brief Устанавливает параметры прямоугольнику с индексами X и Y
    /// \param rect - геометрия прямоугольника
    /// \param xIndex - позиция по x
    /// \param yIndex - позиция по y
    ///
    void setRect(const QRectF &rect, const quint16 &xIndex, const quint16 &yIndex);

    ///
    /// \brief Возвращает прямоугольник с индксами X и Y
    /// \param xIndex - позиция по x
    /// \param yIndex - позиция по y
    ///
    MyRect rect(const quint16 &xIndex, const quint16 &yIndex) const;

    ///
    /// \brief Возвращает поле прямоугольников
    ///
    QVector< QVector< MyRect > > field() const;

    ///
    /// \brief Возвращает ширину поля
    ///
    quint16 width() const;

    ///
    /// \brief Возвращает высоту поля
    ///
    quint16 height() const;

public slots:
    ///
    /// \brief Генерирует поле с припятствиями
    /// \param width - ширина поля
    /// \param height - высота поля
    ///
    void generate(const quint16 &width, const quint16 &height);

private:
    QVector< QVector< MyRect > > m_field;

    quint16 m_width{};
    quint16 m_height{};
};
