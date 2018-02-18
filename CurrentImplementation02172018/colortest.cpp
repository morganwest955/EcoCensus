#include "colortest.h"
#include "ui_colortest.h"
#include <QDebug>

ColorTest::ColorTest(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ColorTest)
{
    ui->setupUi(this);
    setButtonColor(colorValue);
}

ColorTest::~ColorTest()
{
    delete ui;
}

void ColorTest::setButtonColor(QColor targetColor)
{
    QPushButton* b = ui->colorSwatch;
    QPalette pal = b->palette();
    pal.setColor(QPalette::Button, targetColor);
    b->setAutoFillBackground(true);
    b->setPalette(pal);
    b->update();
}

void ColorTest::setColorValue(QColor color)
{
    colorValue = color;
    setButtonColor(colorValue);
}

QColor ColorTest::getColorValue()
{
    return colorValue;
}

void ColorTest::setLabelText(QString s)
{
    ui->lineEdit->setText(s);
}

QString ColorTest::getLabelText()
{
    return ui->lineEdit->text();
}

void ColorTest::on_colorSwatch_clicked()
{
    QColor color = QColorDialog::getColor(getColorValue(), NULL, "Choose Pen Color", QColorDialog::ShowAlphaChannel);
    if (color.isValid()) {
        setColorValue(color);
    } else {
        // I don't THINK there's any reason to do this
        //setColorValue(getColorValue());
    }
}

void ColorTest::on_pushButton_clicked()
{
    qDebug() << this;
    qDebug() << parentWidget();
    deleteLater();
}
