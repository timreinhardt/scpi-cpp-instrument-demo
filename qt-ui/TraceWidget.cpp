#include "TraceWidget.hpp"

#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QColor>
#include <QString>

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

void TraceWidget::setFixedScale(double minDbm, double maxDbm)
{
    fixedMinDbm_ = minDbm;
    fixedMaxDbm_ = maxDbm;
    autoScale_ = false;
    update();
}

void TraceWidget::setAutoScale(bool enabled)
{
    autoScale_ = enabled;
    update();
}

void TraceWidget::setShowGrid(bool enabled)
{
    showGrid_ = enabled;
    update();
}

void TraceWidget::setFrequencyRangeMHz(double startMHz, double stopMHz)
{
    startFreqMHz_ = startMHz;
    stopFreqMHz_ = stopMHz;
    update();
}

void TraceWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    painter.fillRect(rect(), QColor(18, 22, 28));

    QRectF plotArea = rect().adjusted(85, 25, -25, -45);

    painter.setPen(QPen(QColor(120, 120, 120), 1));
    painter.drawRect(plotArea);

    double minVal = fixedMinDbm_;
    double maxVal = fixedMaxDbm_;

    if (autoScale_ && traceData_.size() >= 2)
    {
        auto minMax = std::minmax_element(traceData_.begin(), traceData_.end());
        minVal = *minMax.first;
        maxVal = *minMax.second;

        if (maxVal - minVal < 0.001)
        {
            maxVal += 1.0;
            minVal -= 1.0;
        }
    }

    if (showGrid_)
    {
        painter.setPen(QPen(QColor(55, 65, 75), 1));

        const int verticalLines = 10;
        const int horizontalLines = 6;

        for (int i = 1; i < verticalLines; ++i)
        {
            double x = plotArea.left() +
                       (plotArea.width() * i / verticalLines);

            painter.drawLine(QPointF(x, plotArea.top()),
                             QPointF(x, plotArea.bottom()));
        }

        for (int i = 1; i < horizontalLines; ++i)
        {
            double y = plotArea.top() +
                       (plotArea.height() * i / horizontalLines);

            painter.drawLine(QPointF(plotArea.left(), y),
                             QPointF(plotArea.right(), y));
        }
    }

    painter.setPen(QPen(QColor(210, 210, 210), 1));

    painter.drawText(12, 32, "Power");
    painter.drawText(18, 48, "(dBm)");
    painter.drawText(width() / 2 - 58, height() - 12, "Frequency (MHz)");

    painter.drawText(
        18,
        static_cast<int>(plotArea.top() + 5),
        QString::number(maxVal, 'f', 1)
    );

    painter.drawText(
        18,
        static_cast<int>(plotArea.bottom()),
        QString::number(minVal, 'f', 1)
    );

    double midFreqMHz = (startFreqMHz_ + stopFreqMHz_) / 2.0;

    painter.drawText(
        static_cast<int>(plotArea.left()) - 10,
        height() - 28,
        QString::number(startFreqMHz_, 'f', 0)
    );

    painter.drawText(
        static_cast<int>(plotArea.center().x()) - 18,
        height() - 28,
        QString::number(midFreqMHz, 'f', 0)
    );

    painter.drawText(
        static_cast<int>(plotArea.right()) - 30,
        height() - 28,
        QString::number(stopFreqMHz_, 'f', 0)
    );

    if (traceData_.size() < 2)
    {
        painter.setPen(QPen(QColor(170, 170, 170), 1));
        painter.drawText(plotArea.center(), "No trace data");
        return;
    }

    painter.setPen(QPen(QColor(0, 255, 120), 2));

    QPointF previousPoint;

    for (std::size_t i = 0; i < traceData_.size(); ++i)
    {
        double value = traceData_[i];

        if (value < minVal)
        {
            value = minVal;
        }

        if (value > maxVal)
        {
            value = maxVal;
        }

        double xNorm =
            static_cast<double>(i) /
            static_cast<double>(traceData_.size() - 1);

        double yNorm =
            (value - minVal) /
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

    painter.setPen(QPen(QColor(220, 220, 220), 1));

    painter.drawText(
        static_cast<int>(plotArea.left() + 8),
        static_cast<int>(plotArea.top() + 18),
        QString("Scale: %1 to %2 dBm")
            .arg(minVal, 0, 'f', 1)
            .arg(maxVal, 0, 'f', 1)
    );

    painter.drawText(
        static_cast<int>(plotArea.left() + 8),
        static_cast<int>(plotArea.top() + 36),
        QString("Points: %1").arg(traceData_.size())
    );

    painter.drawText(
        static_cast<int>(plotArea.left() + 8),
        static_cast<int>(plotArea.top() + 54),
        QString("Span: %1-%2 MHz")
            .arg(startFreqMHz_, 0, 'f', 0)
            .arg(stopFreqMHz_, 0, 'f', 0)
    );
}