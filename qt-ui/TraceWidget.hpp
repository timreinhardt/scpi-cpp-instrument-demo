#pragma once

#include <QWidget>

#include <vector>

class TraceWidget : public QWidget
{
public:
    explicit TraceWidget(QWidget *parent = nullptr);

    void setTraceData(const std::vector<double>& values);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    std::vector<double> traceData_;
};