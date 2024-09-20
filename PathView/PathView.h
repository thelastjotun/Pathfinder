#pragma once

#include <QGraphicsPathItem>
#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QGraphicsView>
#include <QScrollBar>
#include <QWheelEvent>

#include <QThread>

#include "BFSProcessor.h"
#include "FieldGenerator.h"

class PathView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit PathView(QWidget *parent = nullptr);
    ~PathView();

public slots:
    ///
    /// \brief Отображает рандомно сгенерированное поле с припятствиями
    /// \param width - Ширина поля
    /// \param height - Высота поля
    ///
    void generateField(const quint16 &width, const quint16 &height);

    ///
    /// \brief Устанавливает состояние переменной для автоматической отрисовки линии
    /// \param state - состояние переменной
    ///
    void setAutoBFS(const bool &state);

signals:
    void errorChanged(const QString &);

private:
    ///
    /// \brief Вызывает поиск в ширину от точки A до B
    ///
    void bfsProcess();

    ///
    /// \brief Отображает линию
    /// \param line - линия на поле
    ///
    void drawLine(const QPolygon &line);

protected:
    void wheelEvent(QWheelEvent *event) override;

    void mousePressEvent(QMouseEvent *event) override;

    void mouseReleaseEvent(QMouseEvent *event) override;

    void mouseMoveEvent(QMouseEvent *event) override;

private:
    QGraphicsScene m_scene;

    bool m_autoBFS{ false };

    qint64 m_originX{};
    qint64 m_originY{};

    double m_scaleFactor{ 1.25 };

    QPoint m_APoint;
    QPoint m_BPoint;

    QGraphicsTextItem m_AItem;
    QGraphicsTextItem m_BItem;

    MyRect m_currentFocusRect;

    QPainterPath m_painterPath;
    QGraphicsPathItem m_bfsLine;

    FieldGenerator m_generator;
    BFSProcessor m_bfs;

    QThread m_thread;
};
