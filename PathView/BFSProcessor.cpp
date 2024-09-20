#include "BFSProcessor.h"
#include <QPoint>

#include <queue>

BFSProcessor::BFSProcessor(QObject *parent)
    : QObject{ parent }
{}

void BFSProcessor::process(
    const QPoint &A,
    const QPoint &B,
    const QVector< QVector< MyRect > > field,
    const quint16 &fieldWidth,
    const quint16 &fieldHeight)
{
    QVector< QVector< quint32 > > distances(fieldHeight, QVector< quint32 >(fieldWidth, INF));
    QVector< QVector< QPoint > > from(fieldHeight, QVector< QPoint >(fieldWidth, QPoint(INF, INF)));

    std::queue< QPoint > q;
    q.push(A);

    distances[A.x()][A.y()] = 0;

    QVector< QPoint > nearVertices = { { -1, 0 }, { +1, 0 }, { 0, -1 }, { 0, +1 } };

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();

        // просматриваем всех соседей
        for (auto [dx, dy] : nearVertices) {
            // считаем координаты соседа
            int _x = x + dx, _y = y + dy;
            // и проверяем, что он свободен и не был посещен ранее
            if (_y > -1 && _y < fieldWidth && _x > -1 && _x < fieldHeight) {
                if (!field[_x][_y].isObstacle() && distances[_x][_y] == INF) {
                    distances[_x][_y] = distances[x][y] + 1;
                    from[_x][_y] = QPoint{ x, y };

                    if (QPoint{ _x, _y } == B) {
                        auto path = getPath(A, B, from);

                        auto line = getLine(field, path);

                        emit lineChanged(line);
                        return;
                    }

                    q.push(QPoint{ _x, _y });
                }
            }
        }
    }

    emit lineChanged({});
}

QVector< QPoint > BFSProcessor::getPath(const QPoint &A, const QPoint &B, const QVector< QVector< QPoint > > &field)
{
    QVector< QPoint > path;

    auto currentPos = B;
    auto nextPos = field[B.x()][B.y()];

    if (nextPos.x() == INF || nextPos.y() == INF)
        return {};

    path.push_back(currentPos);
    path.push_back(nextPos);

    if (field[B.x()][B.y()] != QPoint(INF, INF)) {
        while (nextPos != A) {
            if (field[nextPos.x()][nextPos.y()] == QPoint(INF, INF))
                continue;

            currentPos = nextPos;
            nextPos = field[nextPos.x()][nextPos.y()];

            path.push_back(nextPos);
        }
    }

    return path;
}

QPolygon BFSProcessor::getLine(const QVector< QVector< MyRect > > field, const QVector< QPoint > &path)
{
    QPolygon line;

    for (auto &itemPos : path) {
        line << field[itemPos.x()][itemPos.y()].center();
    }

    return line;
}
