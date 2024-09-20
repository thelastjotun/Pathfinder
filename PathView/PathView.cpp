#include <QMessageBox>

#include "Common.h"
#include "PathView.h"

PathView::PathView(QWidget *parent)
    : QGraphicsView{ parent }
{
    m_AItem.setPlainText("A");
    m_BItem.setPlainText("B");

    this->setMinimumSize(200, 200);

    this->setMouseTracking(true);

    this->setScene(&m_scene);

    connect(&m_bfs, &BFSProcessor::lineChanged, this, &PathView::drawLine);

    m_bfs.moveToThread(&m_thread);
    m_thread.start();
}

PathView::~PathView()
{
    if (m_thread.isRunning()) {
        m_thread.quit();
        m_thread.wait();
    }
}

void PathView::generateField(const quint16 &width, const quint16 &height)
{
    for (auto &item : m_scene.items()) {
        if (item->scene() != nullptr)
            m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(item));
    }

    m_scene.clear();

    m_generator.generate(width, height);

    qsizetype currentLine{};

    quint16 wRect{ 60 };
    quint16 hRect{ 60 };

    QPen blackPen{ Qt::black };
    QBrush blackBrush{ Qt::black };
    QBrush grayBrush{ Qt::gray };

    for (qsizetype currentLine{}; currentLine < m_generator.height(); ++currentLine) {
        for (qsizetype currentColumn{}; currentColumn < m_generator.width(); ++currentColumn) {
            m_generator.setRect(QRectF(currentColumn * wRect, currentLine * hRect, wRect, hRect), currentColumn, currentLine);

            m_generator.rect(currentColumn, currentLine).isObstacle() == true
                ? m_scene.addRect(m_generator.rect(currentColumn, currentLine), blackPen, blackBrush)
                : m_scene.addRect(m_generator.rect(currentColumn, currentLine), blackPen, grayBrush);
        }
    }
}

void PathView::setAutoBFS(const bool &state)
{
    m_autoBFS = state;
}

void PathView::bfsProcess()
{
    if (m_AItem.scene() == nullptr || m_BItem.scene() == nullptr)
        return;

    QMetaObject::invokeMethod(&m_bfs, "process", m_APoint, m_BPoint, m_generator.field(), m_generator.width(), m_generator.height());
}

void PathView::drawLine(const QPolygon &line)
{
    if (m_bfsLine.scene() != nullptr) {
        m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(&m_bfsLine));
    }

    m_painterPath.clear();

    m_painterPath.addPolygon(line);
    m_bfsLine.setPath(m_painterPath);

    m_scene.addItem(qgraphicsitem_cast< QGraphicsItem * >(&m_bfsLine));
}

void PathView::wheelEvent(QWheelEvent *event)
{
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);

    if (event->angleDelta().y() > 0) {
        scale(m_scaleFactor, m_scaleFactor);
    } else {
        scale(1 / m_scaleFactor, 1 / m_scaleFactor);
    }
}

void PathView::mousePressEvent(QMouseEvent *event)
{
    if (event->button() & Qt::LeftButton) {
        for (auto &line : m_generator.field()) {
            for (auto &rect : line) {
                if (rect.contains(mapToScene(event->pos()))) {
                    if (rect.isObstacle()) {
                        emit errorChanged("Не получится поставить точку А на препятствие!");
                        return;
                    }

                    if (m_AItem.scene() != nullptr) {
                        m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(&m_AItem));
                    }

                    m_AItem.setPlainText("A");
                    m_AItem.setPos(rect.x() + rect.width() / 2, rect.y() + rect.width() / 2);

                    m_scene.addItem(qgraphicsitem_cast< QGraphicsItem * >(&m_AItem));

                    m_APoint = QPoint{ rect.yIndex(), rect.xIndex() };

                    bfsProcess();
                }
            }
        }

        event->accept();
        return;
    }

    if (event->button() & Qt::RightButton) {
        for (auto &line : m_generator.field()) {
            for (auto &rect : line) {
                if (rect.contains(mapToScene(event->pos()))) {
                    if (rect.isObstacle()) {
                        emit errorChanged("Не получится поставить точку B на препятствие!");
                        return;
                    }

                    if (m_BItem.scene() != nullptr) {
                        m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(&m_BItem));
                    }

                    m_BItem.setPlainText("B");
                    m_BItem.setPos(rect.x() + rect.width() / 4, rect.y() + rect.width() / 2);

                    m_scene.addItem(qgraphicsitem_cast< QGraphicsItem * >(&m_BItem));

                    m_BPoint = QPoint{ rect.yIndex(), rect.xIndex() };

                    bfsProcess();
                }
            }
        }

        event->accept();
        return;
    }

    if (event->button() & Qt::MiddleButton) {
        setTransformationAnchor(QGraphicsView::NoAnchor);

        m_originX = event->pos().x();
        m_originY = event->pos().y();

        setCursor(Qt::ClosedHandCursor);

        event->accept();
        return;
    }
    event->ignore();
}

void PathView::mouseReleaseEvent(QMouseEvent *event)
{
    setCursor(Qt::ArrowCursor);
}

void PathView::mouseMoveEvent(QMouseEvent *event)
{
    if (m_autoBFS) {
        for (auto &line : m_generator.field()) {
            for (auto &rect : line) {
                if (rect.contains(mapToScene(event->pos()))) {
                    if (m_currentFocusRect == rect) {
                        return;
                    }

                    auto abXDistance = abs(m_BPoint.x() - m_APoint.x());
                    auto abYDistance = abs(m_BPoint.y() - m_APoint.y());

                    if (rect.isObstacle() || abXDistance < 30 && abYDistance < 30) {
                        if (m_bfsLine.scene() != nullptr) {
                            m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(&m_bfsLine));
                        }
                    }

                    if (m_BItem.scene() != nullptr) {
                        m_scene.removeItem(qgraphicsitem_cast< QGraphicsItem * >(&m_BItem));
                    }

                    m_BItem.setPlainText("B");
                    m_BItem.setPos(rect.x() + rect.width() / 4, rect.y() + rect.width() / 2);

                    m_scene.addItem(qgraphicsitem_cast< QGraphicsItem * >(&m_BItem));

                    m_BPoint = QPoint{ rect.yIndex(), rect.xIndex() };

                    bfsProcess();

                    m_currentFocusRect = rect;
                }
            }
        }
    }

    if (event->buttons() & Qt::MiddleButton) {
        setTransformationAnchor(QGraphicsView::NoAnchor);

        horizontalScrollBar()->setValue(horizontalScrollBar()->value() - (event->pos().x() - m_originX));
        verticalScrollBar()->setValue(verticalScrollBar()->value() - (event->pos().y() - m_originY));
        m_originX = event->pos().x();
        m_originY = event->pos().y();
    }
}
