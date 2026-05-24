#include "TraceWidget.hpp"
#include "TraceWidget.hpp"

#include <QPainter>
#include <QPen>
#include <QBrush>
#include <QRectF>

#include <algorithm>

TraceWidget::TraceWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(220);
}

void TraceWidget::setTraceData(const std::vector<double>& values)
{
    traceData_ = values;
    update();
}

void TraceWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(20, 20, 20));

    QRectF plotArea = rect().adjusted(40, 20, -20, -35);

    painter.setPen(QPen(QColor(90, 90, 90), 1));
    painter.drawRect(plotArea);

    painter.setPen(QPen(QColor(120, 120, 120), 1));
    painter.drawText(8, 20, "dBm");
    painter.drawText(width() / 2 - 40, height() - 8, "Sample Index");

    if (traceData_.size() < 2)
    {
        painter.setPen(QPen(QColor(180, 180, 180), 1));
        painter.drawText(plotArea.center(), "No trace data");
        return;
    }

    auto minMax = std::minmax_element(traceData_.begin(), traceData_.end());
    double minVal = *minMax.first;
    double maxVal = *minMax.second;

    if (maxVal - minVal < 0.001)
    {
        maxVal += 1.0;
        minVal -= 1.0;
    }

    painter.setPen(QPen(QColor(0, 255, 120), 2));

    QPointF previousPoint;

    for (std::size_t i = 0; i < traceData_.size(); ++i)
    {
        double xNorm =
            static_cast<double>(i) /
            static_cast<double>(traceData_.size() - 1);

        double yNorm =
            (traceData_[i] - minVal) /
            (maxVal - minVal);

        double x =
            plotArea.left() + xNorm * plotArea.width();

        double y =
            plotArea.bottom() - yNorm * plotArea.height();

        QPointF currentPoint(x, y);

        if (i > 0)
        {
            painter.drawLine(previousPoint, currentPoint);
        }

        previousPoint = currentPoint;
    }

    painter.setPen(QPen(QColor(180, 180, 180), 1));

    painter.drawText(
        45,
        18,
        QString("Max: %1 dBm").arg(maxVal, 0, 'f', 2)
    );

    painter.drawText(
        45,
        height() - 18,
        QString("Min: %1 dBm").arg(minVal, 0, 'f', 2)
    );
}