#ifndef COLORTEST_H
#define COLORTEST_H

#include <QWidget>
#include <QColorDialog>

namespace Ui {
class ColorTest;
}

class ColorTest : public QWidget
{
    Q_OBJECT

    QColor colorValue = QColor(255, 255, 255, 255);

public:
    explicit ColorTest(QWidget *parent = 0);
    ~ColorTest();
    void setButtonColor(QColor);
    void setColorValue(QColor);
    QColor getColorValue();
    void setLabelText(QString);
    QString getLabelText();

private slots:
    void on_colorSwatch_clicked();

    void on_pushButton_clicked();

private:
    Ui::ColorTest *ui;
};

#endif // COLORTEST_H
