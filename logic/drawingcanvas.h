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
    Q_PROPERTY(QColor penColor MEMBER _color NOTIFY colorChanged)
    Q_PROPERTY(int penWidth MEMBER _width NOTIFY widthChanged)
    Q_PROPERTY(Tool tool MEMBER _tool NOTIFY toolChanged)
public:
    enum Tool {
        Brush,
        Eraser,
        Line
    };
    Q_ENUM(Tool)
    explicit DrawingCanvas(QQuickItem *parent = nullptr);

    void paint(QPainter *painter);

    Q_INVOKABLE void startDrawing(qreal x, qreal y);
    Q_INVOKABLE void addPoint(qreal x, qreal y);
    Q_INVOKABLE void finishDrawing();
    Q_INVOKABLE void drawRemoteBatch(const QList<QPointF> &points, const QColor& color, const int& penWidth);
signals:
    void pointsBatched(QList<QPointF> points);
    void colorChanged();
    void widthChanged();
    void toolChanged();
private:
    void localDraw(const QPointF &p1, const QPointF &p2, const int& width, const QColor &color);

    QPointF normalize(const QPointF &p);
    QPointF denormalize(const QPointF &p);

    QImage canvasImage;
    QPointF lastPoint;
    QList<QPointF> pointBuffer;

    QColor _color = Qt::black;
    int _width = 3;
    Tool _tool = Tool::Brush;
};

#endif // DRAWINGCANVAS_H
