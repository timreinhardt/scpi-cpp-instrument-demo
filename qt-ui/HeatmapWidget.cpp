#include "HeatmapWidget.hpp"

#include <QPainter>
#include <QPen>
#include <QRectF>
#include <QColor>

#include <algorithm>

HeatmapWidget::HeatmapWidget(QWidget *parent)
    : QWidget(parent)
{
    setMinimumHeight(220);
}

void HeatmapWidget::setGridData(const FieldGrid& grid)
{
    grid_ = grid;
    update();
}

void HeatmapWidget::setFixedScale(double minDbm, double maxDbm)
{
    minDbm_ = minDbm;
    maxDbm_ = maxDbm;
    update();
}

QColor HeatmapWidget::colourForValue(double value) const
{
    double norm = (value - minDbm_) / (maxDbm_ - minDbm_);

    if (norm < 0.0)
    {
        norm = 0.0;
    }

    if (norm > 1.0)
    {
        norm = 1.0;
    }

    int red = static_cast<int>(255 * norm);
    int blue = static_cast<int>(255 * (1.0 - norm));
    int green = static_cast<int>(180 * (1.0 - std::abs(norm - 0.5) * 2.0));

    return QColor(red, green, blue);
}

void HeatmapWidget::paintEvent(QPaintEvent *event)
{
    Q_UNUSED(event);

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing, false);

    painter.fillRect(rect(), QColor(18, 22, 28));

    QRectF plotArea = rect().adjusted(55, 25, -25, -35);

    painter.setPen(QPen(QColor(120, 120, 120), 1));
    painter.drawRect(plotArea);

    if (grid_.empty() || grid_.front().empty())
    {
        painter.setPen(QPen(QColor(180, 180, 180), 1));
        painter.drawText(plotArea.center(), "No field grid data");
        return;
    }

    std::size_t rows = grid_.size();
    std::size_t cols = grid_.front().size();

    double cellW = plotArea.width() / static_cast<double>(cols);
    double cellH = plotArea.height() / static_cast<double>(rows);

    for (std::size_t y = 0; y < rows; ++y)
    {
        for (std::size_t x = 0; x < grid_[y].size(); ++x)
        {
            QRectF cell(
                plotArea.left() + x * cellW,
                plotArea.top() + y * cellH,
                cellW + 1,
                cellH + 1
            );

            painter.fillRect(cell, colourForValue(grid_[y][x]));
        }
    }

    painter.setPen(QPen(QColor(220, 220, 220), 1));

    painter.drawText(
        static_cast<int>(plotArea.left() + 8),
        static_cast<int>(plotArea.top() + 18),
        QString("Field Heatmap: %1 x %2").arg(cols).arg(rows)
    );

    painter.drawText(
        static_cast<int>(plotArea.left() + 8),
        static_cast<int>(plotArea.top() + 36),
        QString("Scale: %1 to %2 dBm")
            .arg(minDbm_, 0, 'f', 1)
            .arg(maxDbm_, 0, 'f', 1)
    );
}