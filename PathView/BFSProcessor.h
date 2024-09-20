#pragma once

#include "Common.h"
#include <QObject>
#include <QPolygon>

class BFSProcessor : public QObject
{
    Q_OBJECT
public:
    explicit BFSProcessor(QObject *parent = nullptr);

public slots:
    ///
    /// \brief Вычисляет кратчайший путь от точки А до точки B и фиксирует пройденные вершины
    /// \param A - точка начала пути
    /// \param B - точка конца пути
    /// \param field - поле с припятствиями
    /// \param fieldWidth - ширина поля
    /// \param fieldHeight - высота поля
    ///
    void process(
        const QPoint &A,
        const QPoint &B,
        const QVector< QVector< MyRect > > field,
        const quint16 &fieldWidth,
        const quint16 &fieldHeight);

private:
    ///
    /// \brief Возвращает путь от конечной точки к начальной
    /// \param A - начальная точка
    /// \param B - конечноая точка
    /// \param field - поле точек из которых был произведён переход
    ///
    QVector< QPoint > getPath(const QPoint &A, const QPoint &B, const QVector< QVector< QPoint > > &field);

    ///
    /// \brief Возвращает линию после отработки BFS
    /// \param field - поле с припятствиями
    /// \param path - путь от конца к началу
    ///
    QPolygon getLine(const QVector< QVector< MyRect > > field, const QVector< QPoint > &path);

signals:
    void lineChanged(const QPolygon &);
};
