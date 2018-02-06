#ifndef ECOCENSUS_H
#define ECOCENSUS_H

#include <QMainWindow>
#include <QDir>
#include <QImageReader>
#include <QListWidgetItem>
#include "dialog.h"

namespace Ui {
class EcoCensus;
}

class EcoCensus : public QMainWindow
{
    Q_OBJECT

public:
    explicit EcoCensus(QWidget *parent = 0);
    ~EcoCensus();

private slots:

    void on_openButton_clicked();

    void on_getDirPos_clicked();

    void on_button_predict_clicked();

    void populateGrid();

    void on_openButton_2_clicked();

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::EcoCensus *ui;
    Dialog* blockDialog;
};

#endif // ECOCENSUS_H
