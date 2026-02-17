#include "drawingcanvas.h"

DrawingCanvas::DrawingCanvas(QQuickItem *parent) : QQuickPaintedItem(parent) {
    setAntialiasing(true);
    setMipmap(true);
    setOpaquePainting(false);
    setRenderTarget(QQuickPaintedItem::FramebufferObject);
}

void DrawingCanvas::paint(QPainter *painter) {
    if (width() <= 0 || height() <= 0) return;
    if (canvasImage.isNull()) {
        canvasImage = QImage(size().toSize(), QImage::Format_ARGB32);
        canvasImage.fill(Qt::transparent);
    }
    painter->setRenderHint(QPainter::SmoothPixmapTransform);
    painter->drawImage(0, 0, canvasImage);
}

void DrawingCanvas::startDrawing(qreal x, qreal y)
{
    lastPoint = QPointF(x, y);
    pointBuffer.clear();
    pointBuffer.append(normalize(lastPoint));
}

void DrawingCanvas::addPoint(qreal x, qreal y)
{
    if (canvasImage.isNull()) return;

    QPointF currentPoint(x, y);
    localDraw(lastPoint, currentPoint, _width, _color);
    lastPoint = currentPoint;

    pointBuffer.append(normalize(currentPoint));

    if (pointBuffer.size() >= 5) {
        emit pointsBatched(pointBuffer);
        pointBuffer.clear();
        pointBuffer.append(normalize(lastPoint));
    }
}

void DrawingCanvas::finishDrawing()
{
    if (pointBuffer.size() > 1) {
        emit pointsBatched(pointBuffer);
    }
    pointBuffer.clear();
}

void DrawingCanvas::drawRemoteBatch(const QList<QPointF> &points, const QColor& color, const int& penWidth)
{
    if (points.size() < 2) return;
    for (int i = 1; i < points.size(); ++i) {
        localDraw(denormalize(points[i-1]), denormalize(points[i]), penWidth, color);
    }
}

void DrawingCanvas::localDraw(const QPointF &p1, const QPointF &p2, const int& width, const QColor &color)
{
    QPainter imgPainter(&canvasImage);
    imgPainter.setRenderHint(QPainter::Antialiasing);
    QPen pen(color, width, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    imgPainter.setPen(pen);
    imgPainter.drawLine(p1, p2);

    QRect dirtyRect = QRectF(p1, p2).normalized().toAlignedRect();
    dirtyRect.adjust(-5, -5, 5, 5);
    update(dirtyRect);
}

QPointF DrawingCanvas::normalize(const QPointF &p)
{
    return QPointF(p.x() / width(), p.y() / height());
}

QPointF DrawingCanvas::denormalize(const QPointF &p)
{
    return QPointF(p.x() * width(), p.y() * height());
}

