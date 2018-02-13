#ifndef ECOCENSUS_H
#define ECOCENSUS_H

#include <QMainWindow>
#include <QDir>
#include <QImageReader>
#include <QListWidgetItem>
#include "dialog.h"
#include "boxinfo.h"
#include <vector>
using std::vector;

namespace Ui {
class EcoCensus;
}

class EcoCensus : public QMainWindow
{
    Q_OBJECT

public:
    explicit EcoCensus(QWidget *parent = 0);
    ~EcoCensus();
    void addColorListWidget(QString label, QColor color);
    void addColorListWidget(QString label, QString userData, QColor color);
    void addPicListWidget(QString label, QString filePath);
    QString getALabel();
    QColor getAColor();

private slots:

    void on_openButton_clicked();

    void on_getDirPos_clicked();

    void on_button_predict_clicked();

    void on_openButton_2_clicked();

    void populateList_Partitions(vector<BoxInfo> &list, QString fileName);

    void on_listWidget_itemClicked(QListWidgetItem *item);

private:
    Ui::EcoCensus *ui;
};

#endif // ECOCENSUS_H
