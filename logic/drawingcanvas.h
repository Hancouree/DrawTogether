#ifndef DRAWINGCANVAS_H
#define DRAWINGCANVAS_H

#include <QQuickPaintedItem>
#include <QImage>
#include <QPainter>
#include <QPen>
#include <QPointF>
#include <QList>
#include <QtQml/qqmlregistration.h>

class DrawingCanvas : public QQuickPaintedItem {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QColor penColor MEMBER m_color NOTIFY colorChanged)
public:
    explicit DrawingCanvas(QQuickItem *parent = nullptr);

    void paint(QPainter *painter);

    Q_INVOKABLE void startDrawing(qreal x, qreal y);

    Q_INVOKABLE void addPoint(qreal x, qreal y);

    Q_INVOKABLE void finishDrawing();

    Q_INVOKABLE void drawRemoteBatch(const QList<QPointF> &points, QColor color);
signals:
    void pointsBatched(QList<QPointF> points);
    void colorChanged();
private:
    void localDraw(const QPointF &p1, const QPointF &p2, const QColor &color);

    QPointF normalize(const QPointF &p);

    QPointF denormalize(const QPointF &p);

    QImage canvasImage;
    QPointF lastPoint;
    QList<QPointF> pointBuffer;
    QColor m_color = Qt::black;
};

#endif // DRAWINGCANVAS_H
