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
#include <QPainter>
#include "colortest.h"

EcoCensus::EcoCensus(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EcoCensus)
{
    ui->setupUi(this);
}

EcoCensus::~EcoCensus()
{
    delete ui;
}

void EcoCensus::addColorListWidget(QString label, QColor color)
{
    QListWidgetItem* lwi = new QListWidgetItem(ui->dirList);
    ColorTest* ct = new ColorTest;
    ct->setLabelText(label);
    ct->setColorValue(color);
    lwi->setSizeHint(ct->sizeHint());
    ui->dirList->setItemWidget(lwi, ct);
}

void EcoCensus::addColorListWidget(QString label, QString userdata, QColor color)
{
    QListWidgetItem* lwi = new QListWidgetItem(ui->dirList);
    ColorTest* ct = new ColorTest;
    ct->setLabelText(label);
    ct->setColorValue(color);
    lwi->setData(Qt::UserRole, userdata);
    lwi->setSizeHint(ct->sizeHint());
    ui->dirList->setItemWidget(lwi, ct);
}

void EcoCensus::addPicListWidget(QString label, QString filePath)
{
    static int thumbsize = 100;
    // load an image
    QImageReader cimg(filePath);
    if (!cimg.canRead()) {
        ErrorDialog error(this, "No Image!!");
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
        QListWidgetItem* imgitem = new QListWidgetItem(label, ui->listWidget);
        imgitem->setData(Qt::DecorationRole, cimgdata);
        imgitem->setData(Qt::UserRole, filePath);
    }
}

QString EcoCensus::getALabel()
{
    static int num = 0;
    return QStringLiteral("Label%1").arg(num++);
}

QColor EcoCensus::getAColor()
{
    static int h = 0;
    static int s = 255;
    static int v = 255;
    static int a = 255;
    QColor result;
    result.setHsv(h, s, v, a);
    h += 53;
    h = h % 360;
    return result;
}

void EcoCensus::on_openButton_clicked()
{
    QFileDialog* asdf = new QFileDialog();
    ui->rootUrl->setText(asdf->getExistingDirectory(this, tr("Open Dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void EcoCensus::on_getDirPos_clicked()
{
    QFileDialog* asdf = new QFileDialog();
    ui->destUrl->setText(asdf->getExistingDirectory(this, tr("Open Dir"), "./", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks));
}

void EcoCensus::on_button_predict_clicked()
{
    // get a copy of "this" for the lambda functions to use
    auto* capthis = (this);
    // ROOT/Partitions
    // DEST/Predictions/RESULT
    // QByteArray array = str.toLocal8Bit();

    QString d1 = "/Partitions";
    QString d2 = "/Predictions";

    // Root URL to char*
    QString root = ui->rootUrl->text();
    char* rootbuf = root.toLocal8Bit().data();
    // Destination URL to char*
    QString dest = ui->destUrl->text();
    if (dest.isEmpty()) {
        dest = root;
    }
    char* destbuf = dest.toLocal8Bit().data();

    // Predict on the directories
    predictions(rootbuf, destbuf);
    // collect folder names in a list
    QString resultDir = dest + d2;
    QDir dir(resultDir);
    QStringList results = dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << results;

    // create all of the result folder widgets
    // clear previous
    ui->dirList->clear();
    std::for_each(
                results.begin(),
                results.end(),
                [&resultDir, capthis](QString item){
        // item is the folder name, stored in the Label
        // store the full folder path in the UserRole data
        if (item == "Negative") {
            capthis->addColorListWidget(item, resultDir + "/" + item, Qt::transparent);
        } else {
            capthis->addColorListWidget(item, resultDir + "/" + item, capthis->getAColor());
        }
    });

    // populate the picture list
    QDir mydir(root);
    if (mydir.exists()) {
        QStringList filters;
        filters << "*.jpg";
        QStringList files = mydir.entryList(filters, QDir::Files, QDir::Name | QDir::IgnoreCase);
        ui->listWidget->clear();
        std::for_each(
                    files.begin(),
                    files.end(),
                    [&root, capthis, d1, d2](QString i) {
            // i is the file name, stored in the label
            // store the full filepath in the UserRole data
            capthis->addPicListWidget(i, root + "/" + i);
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

void EcoCensus::populateList_Partitions(vector<BoxInfo> &list, QString fileName) {
    // get a copy of "this" for the lambda function to use
    auto* capthis = this;

    // loop the children of our QListWidget
    for (int i = 0; i < ui->dirList->count(); i++) {
        // get the current item in various forms in case we need them
        // Widget Item
        QListWidgetItem* item = ui->dirList->item(i);
        // Widget itself
        QWidget* widget = ui->dirList->itemWidget(item);
        // Custom Class with needed information
        ColorTest* ct = (ColorTest*)widget;

        // get the current directory
        QString dirPath = item->data(Qt::UserRole).toString();
        QDir curDir(dirPath);
        // get the list of files inside
        QStringList filters;
        filters << QString("*" + fileName);
        QStringList images = curDir.entryList(filters, QDir::Files);
        std::for_each(
                    images.begin(),
                    images.end(),
                    [capthis, &list, &ct, dirPath](QString file){
            // blank result
            BoxInfo info;
            // color
            info.pencolor = ct->getColorValue();
            // xy
            QStringList pieces = file.split('_');
            int offset = 1;
            info.x = pieces.at(1 + offset).toInt();
            info.y = pieces.at(0 + offset).toInt();
            // wh
            //QString curFile = dirPath + "/" + file;
            //qDebug() << curFile;
            //QImageReader curImg(curFile);
            //QSize sz = curImg.size();
            //qDebug() << "size: " << sz;
            //info.w = sz.width();
            //info.h = sz.height();
            // until we get the jpg headers fixed in the partitioning, the
            // automatic size getting won't work
            info.w = 300;
            info.h = 300;
            // push the result to the list
            list.push_back(info);
        });
    }

    // get the destination folder
    QString dest = ui->destUrl->text();
    QString resultDir = dest + "/" + "Predictions";
    QDir dir(resultDir);
    // get the folder list
    QStringList results = dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    qDebug() << "results: " << results;

    std::for_each(
                results.begin(),
                results.end(),
                [resultDir, capthis, fileName](QString folder){
        if (folder == "Negative") {
            return;
        }
        QString curDir = resultDir + "/" + folder;
        QDir dir(curDir);
        // get the file list
        QStringList filters;
        filters << QString("*" + fileName);
        QStringList files = dir.entryList(filters, QDir::Files);

    });
}

void EcoCensus::on_listWidget_itemClicked(QListWidgetItem *item)
{
    // get full file path and information
    QString filepath = item->data(Qt::UserRole).toString();

    // create our copy of the image in memory
    QImage imgdata(item->data(Qt::UserRole).toString());
    // attack a painter to the image data
    QPainter painter(&imgdata);

    // need to populate a list of the areas to draw shapes around
    // need the colors to draw each type
    vector<BoxInfo> boxes;
    // GET ALL THE BOX INFO
    populateList_Partitions(boxes, item->text());

    //
    //
    //
    // convert to work on the directory/color list
    // change the box info list userdata to have the referenced directory or the
    // parent directory so it's all stored in memory where the user can't create bugs
    // by changing the folders (no ui->destUrl allowed anymore once the predictions are done)
    //
    //
    //

    // DRAW ALL THE BOXES start
    std::for_each(
                boxes.begin(),
                boxes.end(),
                [&painter](BoxInfo item){

        qDebug() <<
                    item.pencolor << ", " <<
                    item.x << ", " <<
                    item.y << ", " <<
                    item.w << ", " <<
                    item.h << ", " <<
                    "";
        // create a pen
        // insert the correct color here
        painter.setPen(item.pencolor);
        //painter.
        // insert the correct posx, posy, szx, szy
        painter.drawRect(item.x, item.y, item.w, item.h);
    });
    // DRAW ALL THE BOXES end
    // update the label with our painted image
    ui->imageLabel->setPixmap(QPixmap::fromImage(imgdata));
}
