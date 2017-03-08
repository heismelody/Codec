#include <QtWidgets>

#include "fileloader.h"

FileLoader::FileLoader()
{

}

static void initFileDialog(QFileDialog &dialog, QFileDialog::AcceptMode acceptMode)
{
    static bool firstDialog = true;

    if (firstDialog) {
        firstDialog = false;
        const QStringList videoLocations = QStandardPaths::standardLocations(QStandardPaths::MoviesLocation);
        dialog.setDirectory(videoLocations.isEmpty() ? QDir::currentPath() : videoLocations.last());
    }

    QStringList mimeTypeFilters;
    mimeTypeFilters << "video/mpeg";
    dialog.setMimeTypeFilters(mimeTypeFilters);
    dialog.selectMimeTypeFilter("video/mpeg");
    if (acceptMode == QFileDialog::AcceptSave)
        dialog.setDefaultSuffix("mpg");
}

void FileLoader::sl_open_file()
{
    QFileDialog dialog(this, tr("Open File"));
    initFileDialog(dialog, QFileDialog::AcceptOpen);

    while (dialog.exec() == QDialog::Accepted
           && !loadFile(dialog.selectedFiles().first())) {}
}

bool FileLoader::loadFile(QString& fileName)
{
    if (fileName == "")
    {
        return false;
    }
    else
    {
        emit this->sg_opened_mpg_file(fileName);
        return true;
    }
}

