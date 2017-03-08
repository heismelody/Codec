#include <QtWidgets>

#include "ma_jpeg_viewer.h"

static void initializeImageFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList picturesLocations = QStandardPaths::standardLocations(QStandardPaths::PicturesLocation);
        dialog.setDirectory(picturesLocations.isEmpty() ? QDir::currentPath() : picturesLocations.last());
    }

    QStringList mimeTypeFilters;
    const QByteArrayList supportedMimeTypes = acceptMode == QFileDialog::AcceptOpen
        ? QImageReader::supportedMimeTypes() : QImageWriter::supportedMimeTypes();
    foreach (const QByteArray &mimeTypeName, supportedMimeTypes)
        mimeTypeFilters.append(mimeTypeName);
    mimeTypeFilters.sort();
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("image/jpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("jpg");
}

ma_jpeg_viewer::ma_jpeg_viewer()
    :codec(new ma_jpeg_codec)
{

    connect(this, SIGNAL (sg_out_file(QByteArray *))
            ,this->codec, SLOT (sl_in_file(QByteArray *)));
    connect(this->codec,SIGNAL (sg_decode_complete(uint8_t *))
            ,this, SLOT (sl_decode_complete(uint8_t *)));
}

void ma_jpeg_viewer::setImageFile(QByteArray *imageFile)
{
    this->imageFile = imageFile;
}

void ma_jpeg_viewer::sl_decode_complete(uint8_t *data)
{
    emit this->sg_display_file(data);
}

void ma_jpeg_viewer::sl_open_file()
{
    QFileDialog dialog(this, tr("Open File"));
    initializeImageFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted && !loadFile(dialog.selectedFiles().first())) {}
}

bool ma_jpeg_viewer::loadFile(const QString &fileName)
{
    QFile file_read(fileName);
    if (!file_read.open(QIODevice::ReadWrite))
    {
        return false;
    }
    QByteArray arr = file_read.readAll();
    if (arr.isEmpty())
    {
        file_read.close();
        return false;
    }
    else
    {
        arr = arr.toHex();
        this->setImageFile(&arr);
        emit this->sg_out_file(this->imageFile);
    }
    file_read.close();
    return true;
}

