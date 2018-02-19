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
#include<QtCore/QFile>
#include<QtCore/QTextStream>
#include <QProcess>
#include "colortest.h"

int DEBUG = 2;
// @cleanup quick fix for header mismatch
bool RELEASE = true;
bool MODE = true;

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

void EcoCensus::fillDirList(QString dest)
{
    auto* capthis = this;
    // collect folder names in a list
    QString resultDir = dest;// + d2;
    QDir dir(resultDir);
    QStringList results = dir.entryList(QStringList(), QDir::Dirs | QDir::NoDotAndDotDot);
    if (DEBUG >= 3) qDebug() << results;

    // create all of the result folder widgets
    // clear previous
    ui->dirList->clear();
    std::for_each(
                results.begin(),
                results.end(),
                [&dir, capthis](QString item){
        // item is the folder name, stored in the Label
        // store the full folder path in the UserRole data
        if (item == "Partitions") {
            // do nothing
        } else if (item == "Negative") {
            capthis->addColorListWidget(item, dir.path() + "/" + item, Qt::transparent);
        } else {
            capthis->addColorListWidget(item, dir.path() + "/" + item, capthis->getAColor());
        }
    });
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
    // is empty
    if (dest.isEmpty()) {
        dest = root;
    }
    // STILL empty even thought it was set to root dir
    if (dest.isEmpty()) {
        // error here that dir was not chosen
    }
    char* destbuf = dest.toLocal8Bit().data();

    // Predict on the directories
    QString swd = "cd /Users/bound_to_love/Downloads/EcoCensus-master-01292018/CurrentImplementation02052018";
    QString params1 = "/Users/bound_to_love/tensorflow/bin/python ";
    QString params2 = "/Users/bound_to_love/tensorflow/bin/python ";
    QString params3 = "/Users/bound_to_love/tensorflow/bin/python ";
    QString params4 = "rmdir ";
    params1 += "/Users/bound_to_love/Downloads/EcoCensus-master-01292018/CurrentImplementation02052018/image_partition.py ";
    params1 += root;
    params1 += " ";
    params1 += dest;
    params2 += "/Users/bound_to_love/Downloads/EcoCensus-master-01292018/CurrentImplementation02052018/predictions.py ";
    params2 += dest;
    params3 += "/Users/bound_to_love/Downloads/EcoCensus-master-01292018/CurrentImplementation02052018/imageReader.py ";
    params3 += dest;
    params4 += dest;
    params4 += "/Partitions";
    system(qPrintable(swd));
    system(qPrintable(params1));
    system(qPrintable(params2));
    system(qPrintable(params3));
    system(qPrintable(params4));

    fillDirList(dest);

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
       if (DEBUG >= 2) qDebug() << citem->data(Qt::UserRole).toString();
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
            // pen wd
            info.penwidth = 10;
            // xy
            QStringList pieces = file.split('_');
            // if there's junk at the start of the filename in the future, we need to fix this
            int offset = 0;
            double pwscalar = 0;

            // tests I was doing with making sure the pen lines don't overlap
            // the x and y positions add half the pen width so it doesn't exit out of the image
            // the width and height subtract the whole pen width so it doesn't overlap the next one over
            if (MODE) {
                pwscalar = (1.0 / 2.0);
            }
            info.x = pieces.at(0 + offset).toInt() + floor(info.penwidth * pwscalar);
            info.y = pieces.at(1 + offset).toInt() + floor(info.penwidth * pwscalar);
            // wh
            // GETTING THE WIDTH AND HEIGHT FROM IMAGE DATA
            // until we get the jpg headers fixed in the partitioning, the
            // automatic size getting won't work
            QString curFile = dirPath + "/" + file;
            if (DEBUG >= 4) qDebug() << curFile;
            QImageReader curImg(curFile);
            QSize sz = curImg.size();
            if (DEBUG >= 4) qDebug() << "size: " << sz;
            // remove divide by 10 when the headers work properly
            info.w = (sz.width() / 10) - (ceil(info.penwidth * (2 * pwscalar)));
            info.h = (sz.height() / 10) - (ceil(info.penwidth * (2 * pwscalar)));
            // WH FROM DATA END

            // WH HARDCODED
            if (RELEASE) {
                info.w = 300 - ceil(info.penwidth * (2 * pwscalar));
                info.h = 300 - ceil(info.penwidth * (2 * pwscalar));
            }
            // WH HARDCODED END

            // push the result to the list
            list.push_back(info);
        });
    }
}

void EcoCensus::imageUpdate(QListWidgetItem *item) {
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

    // DRAW ALL THE BOXES start
    std::for_each(
                boxes.begin(),
                boxes.end(),
                [&painter](BoxInfo item){

        if (DEBUG >= 4) qDebug() <<
                    item.pencolor << ", " <<
                    item.x << ", " <<
                    item.y << ", " <<
                    item.w << ", " <<
                    item.h << ", " <<
                    "";

        // create a pen
        QPen pen;
        pen.setColor((item.pencolor));
        pen.setWidth(item.penwidth);
        pen.setCapStyle(Qt::RoundCap);
        pen.setJoinStyle(Qt::MiterJoin);
        // insert the correct color here
        painter.setPen(pen);
        //painter.
        // insert the correct posx, posy, szx, szy
        painter.drawRect(item.x, item.y, item.w, item.h);
    });
    // DRAW ALL THE BOXES end
    // update the label with our painted image
    ui->imageLabel->setPixmap(QPixmap::fromImage(imgdata));
}

void EcoCensus::on_listWidget_itemClicked(QListWidgetItem *item)
{
    imageUpdate(item);
}

void EcoCensus::on_pushButton_clicked()
{
    // Root URL to char*
    QString root = ui->rootUrl->text();
    // Destination URL to char*
    QString dest = ui->destUrl->text();
    // is empty
    if (dest.isEmpty()) {
        dest = root;
    }
    // STILL empty even though set to root
    if (dest.isEmpty()) {
        // error here saying that dir was not chosen
    } else {
        fillDirList(dest);
    }
    //addColorListWidget(getALabel(), "C:/", getAColor());
}
