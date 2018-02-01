#include "EcoCensus.h"
#include "ui_mainwindow.h"
#include "callScript.cpp"
#include <QFileDialog>
#include <QDebug>
#include <QDir>
#include <QImageReader>
#include "errordialog.h"
#include <algorithm>
#include <QtMath>
#include <QSettings>
#include <QDesktopServices>
#include <QThread>


int thumbsz = 200;

EcoCensus::EcoCensus(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EcoCensus)
{
    ui->setupUi(this);
    ui->grid->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->grid->verticalHeader()->setSectionResizeMode(QHeaderView::Stretch);

    //blockDialog->setWindowModality(Qt::WindowModal);
}

EcoCensus::~EcoCensus()
{
    delete ui;
}

void EcoCensus::on_openButton_clicked()
{
    QFileDialog* asdf = new QFileDialog();
    ui->urlLabel->setText(asdf->getExistingDirectory(this, tr("Open Dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void EcoCensus::on_getDirPos_clicked()
{
    QFileDialog* asdf = new QFileDialog();
    ui->lineEdit->setText(asdf->getExistingDirectory(this, tr("Open Dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void EcoCensus::populateGrid() {
    int thumbsize = thumbsz;
    QString d1 = "/Partitions";
    QString d2 = "/Predictions_v2";
    QString dp = "/Positive";
    QString dn = "/Negative";
}

void EcoCensus::on_button_predict_clicked()
{
    int thumbsize = thumbsz;
    QString d1 = "/Partitions";
    QString d2 = "/Predictions";
    QString dp = "/Positive";
    QString dn = "/Negative";

    // DIR/Partitions/Predictions_v2/OUTCOME/

    QString dirtext = ui->urlLabel->text();
    QByteArray array = dirtext.toLocal8Bit();
    char* buffer = array.data();
    QString dirtextoutput = ui->lineEdit->text();
    QByteArray array2 = dirtextoutput.toLocal8Bit();
    char* buffer2 = array2.data();
    predictions(buffer, buffer2);
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
        filters << "*.jpg";
        QStringList files = mydir.entryList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
        auto* capthis = (this);
        ui->listWidget->clear();
        std::for_each(
                    files.begin(),
                    files.end(),
                    [&dirtext, capthis, thumbsize, d1, d2, dp, dn](QString i) {
            QString cimgpath = dirtext + QString("/") + i;
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
                imgitem->setData(Qt::UserRole, cimgpath);
            }
        });

    } else {
        ErrorDialog error(this, "No File");
        error.setModal(true);
        error.exec();
    }
}

void EcoCensus::on_openButton_2_clicked()
{
   QListWidgetItem* citem = ui->dirList->currentItem();
   if (citem != NULL) {
       qDebug() << citem->data(Qt::UserRole).toString();
       QDesktopServices::openUrl(QUrl(citem->data(Qt::UserRole).toString(), QUrl::TolerantMode));
   }
}

void EcoCensus::on_listWidget_itemClicked(QListWidgetItem *item)
{
    int thumbsize = thumbsz;
    QString d1 = "Partitions";
    QString d2 = "Predictions";
    QString dp = "Positive";
    QString dn = "Negative";
    QFile thefile(item->data(Qt::UserRole).toString());
    QFileInfo info(thefile);
    QDir mydir(info.absoluteDir());
    mydir.cd(d1);
    QDir dirp(info.absoluteDir());
    dirp.cd(dp);
    QDir dirn(info.absoluteDir());
    dirn.cd(dn);
    //qDebug() << "dirp: " << dirp;
    //qDebug() << "dirn: " << dirn;
    //qDebug() << "mydir: " << mydir;
    QStringList filters;
    filters << QString("*" + info.fileName());
    //qDebug() << "filters: " << filters;
    QStringList pfiles = dirp.entryList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
    //qDebug() << "pfiles: " << pfiles;
    QStringList nfiles = dirn.entryList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
    //qDebug() << "nfiles: " << nfiles;
    QStringList files = pfiles << nfiles;
    files.sort(Qt::CaseInsensitive);
    //std::sort(files.begin(), files.end());
    //qDebug() << files;
    ui->grid->clear();
    int gx = 0;
    int gy = 0;
    auto* capthis = (this);

    blockDialog = new Dialog(this);
    //blockDialog->setWindowModality(Qt::WindowModal);
    blockDialog->show();
    QThread::sleep(2);

    std::for_each(
                files.begin(),
                files.end(),
                [capthis, &gx, &gy, &dirp, &dirn, thumbsize](QString s){
        // since I'm too lazy to fix my problems before they arrive
        // I'll check both folders for the image I want
        QString imgpath;
        QImage dat;
        QColor bg = Qt::white;
        // Negative Folder
        imgpath = "";
        imgpath = dirn.absoluteFilePath(s);
        //qDebug() << "Trying: " << imgpath;
        if (QFile(imgpath).exists()) {
            //qDebug() << "Found!";
            QImageReader img(imgpath);
            if (!img.canRead()) {
                ErrorDialog error(capthis, QString("No image: " + s));
                error.setModal(true);
                error.exec();
            } else {
                QSize sz = img.size();
                double scalar = double(thumbsize) / double(sz.width());
                int w = int(round(scalar * sz.width()));
                int h = int(round(scalar * sz.height()));
                img.setScaledSize(QSize(w, h));
                dat = img.read();
            }
        }
        // Positive folder
        imgpath = "";
        imgpath = dirp.absoluteFilePath(s);
        //qDebug() << "Trying: " << imgpath;
        if (QFile(imgpath).exists()) {
            //qDebug() << "Found!";
            QImageReader img(imgpath);
            if (!img.canRead()) {
                ErrorDialog error(capthis, QString("No image: " + s));
                error.setModal(true);
                error.exec();
            } else {
                bg = Qt::red;
                QSize sz = img.size();
                double scalar = double(thumbsize) / double(sz.width());
                int w = int(round(scalar * sz.width()));
                int h = int(round(scalar * sz.height()));
                img.setScaledSize(QSize(w, h));
                dat = img.read();
            }
        }
        //qDebug() << "===";
        QTableWidgetItem* cell = new QTableWidgetItem;
        cell->setIcon(QIcon(QPixmap::fromImage(dat)));
        cell->setData(Qt::ToolTipRole, s);
        cell->setBackgroundColor(bg);
        capthis->ui->grid->setItem(gx, gy, cell);
        gy++;
        if (gy >= 10) {
            gy = 0;
            gx++;
        }
    });

    blockDialog->close();

}
