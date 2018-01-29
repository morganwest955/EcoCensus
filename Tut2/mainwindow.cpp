#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "Test.h"
#include "ecpy.h"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QImageReader>
#include "errordialog.h"
#include <algorithm>
#include <QtMath>
#include <QSettings>
#include <QDesktopServices>

int thumbsz = 200;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_openButton_clicked()
{
    QFileDialog* asdf = new QFileDialog();
    ui->urlLabel->setText(asdf->getExistingDirectory(this, tr("Open Dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void MainWindow::on_dirtestButton_clicked()
{
    QByteArray array = ui->urlLabel->text().toLocal8Bit();
    char* buffer = array.data();
    ui->dirtestNum->setValue(ECPy::Functions::predictions(
                                 buffer
                                 ));
}

void MainWindow::on_button_train_clicked()
{
    QByteArray array = ui->urlLabel->text().toLocal8Bit();
    char* buffer = array.data();
    int val = ECPy::Functions::train(
                buffer, ui->trainiters->value()
                );
    qDebug() << val;

    ui->dirtestNum->setValue(val);
}

void MainWindow::on_button_predict_clicked()
{
    int thumbsize = thumbsz;
    QString d1 = "/Partitions";
    QString d2 = "/Predictions_v2";
    QString dp = "/Positive";
    QString dn = "/Negative";

    // DIR/Partitions/Predictions_v2/OUTCOME/

    QString dirtext = ui->urlLabel->text();
    QByteArray array = dirtext.toLocal8Bit();
    char* buffer = array.data();
    ui->dirtestNum->setValue(ECPy::Functions::predictions(
                                 buffer
                                 ));
    QString d3 = dirtext + d2;
    ui->dirList->clear();
    QListWidgetItem* Pos = new QListWidgetItem(dp, ui->dirList);
    Pos->setData(Qt::UserRole, d3 + dp);
    //Pos->setText(dp);
    //ui->dirList->addItem(Pos);
    QListWidgetItem* Neg = new QListWidgetItem(dn, ui->dirList);
    Neg->setData(Qt::UserRole, d3 + dn);
    //Pos->setText(dn);
    //ui->dirList->addItem(Neg);

    QDir mydir(dirtext);
    if (mydir.exists()) {
        QStringList filters;
        filters << "*.png" << "*.jpg";
        QStringList files = mydir.entryList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
        auto* capthis = (this);
        ui->listWidget->clear();
        std::for_each(
                    files.begin(),
                    files.end(),
                    [&dirtext, capthis, thumbsize, d1, d2, dp, dn](QString i) {
            QString cimgpath = dirtext + QString("/") + i;
            qDebug() << "cimgpath: " << cimgpath;
            QImageReader cimg(cimgpath);
            if (!cimg.canRead()) {
                ErrorDialog error(capthis, "No Image!!");
                error.setModal(true);
                error.exec();
            } else {
                // fix size
                int newwd = thumbsize;
                int newht = thumbsize;
                double scalar = 1.0;
                QSize cimgsize = cimg.size();
                if (cimgsize.width() >= cimgsize.height()) {
                    // Width Bigger or equal
                    if (cimgsize.width() > thumbsize) {
                        scalar = double(thumbsize) / double(cimgsize.width());
                        newht = int(round(scalar * cimgsize.height()));
                    }
                } else {
                    // Height Bigger
                    if (cimgsize.height() > thumbsize) {
                        scalar = double(thumbsize) / double(cimgsize.height());
                        newwd = int(round(scalar * cimgsize.width()));
                    }
                }
                cimg.setScaledSize(QSize(newwd, newht));
                // end fix size
                QImage cimgdata = cimg.read();
                QListWidgetItem* imgitem = new QListWidgetItem(i, capthis->ui->listWidget);
                imgitem->setData(Qt::DecorationRole, cimgdata);
            }
        });

    } else {
        ErrorDialog error(this, "No File");
        error.setModal(true);
        error.exec();
    }
}







void MainWindow::on_openButton_2_clicked()
{
   QListWidgetItem* citem = ui->dirList->currentItem();
   if (citem != NULL) {
       qDebug() << citem->data(Qt::UserRole).toString();
       QDesktopServices::openUrl(citem->data(Qt::UserRole).toString());
    }
}
