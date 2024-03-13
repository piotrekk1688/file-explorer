#include "fileexplorer.h"
#include "./ui_fileexplorer.h"

FileExplorer::FileExplorer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FileExplorer)
    , model(new QFileSystemModel(this))
{
    ui->setupUi(this);
    ui->filesTree->setModel(model);

    connect(ui->filesTree, &QTreeView::doubleClicked,
            this, &FileExplorer::UpdateIndex);
    connect(ui->path, &QLineEdit::returnPressed,
            this, static_cast<void(FileExplorer::*)()>(&FileExplorer::UpdatePath));
    connect(ui->backButton, &QPushButton::released,
            this, &FileExplorer::BackButtonPressed);
    connect(ui->forwardButton, &QPushButton::released,
            this, &FileExplorer::ForwardButtonPressed);
    connect(ui->filesTree, &QTreeView::customContextMenuRequested,
            this, &FileExplorer::HandleCustomContextMenuRequest);

    UpdatePath(previousPath, false);
}

FileExplorer::~FileExplorer()
{
    delete ui;
}

void FileExplorer::ShowPath(const QString &path) {
    ui->filesTree->setModel(nullptr);

    model->setRootPath(path);
    ui->filesTree->setModel(model);
    ui->filesTree->setRootIndex(model->index(path));
    ui->path->setText(path);
}

void FileExplorer::UpdatePath() {
    QString path = ui->path->text();
    UpdatePath(path);
}

void FileExplorer::UpdatePath(const QString &path, bool pushToBackPaths) {
    if (!IsValidPath(path)) {
        return;
    }

    if (pushToBackPaths) {
        backPaths.push(previousPath);

        while (!forwardPaths.empty()) {
            forwardPaths.pop();
        }
    }

    previousPath = path;
    ShowPath(path);
}

void FileExplorer::UpdateIndex(const QModelIndex &index) {
    QString path = model->filePath(index);
    UpdatePath(path);
}

void FileExplorer::BackButtonPressed() {
    if (backPaths.empty()) {
        return;
    }

    forwardPaths.push(ui->path->text());
    UpdatePath(backPaths.top(), false);
    backPaths.pop();
}

void FileExplorer::ForwardButtonPressed() {
    if (forwardPaths.empty()) {
        return;
    }

    backPaths.push(ui->path->text());
    UpdatePath(forwardPaths.top(), false);
    forwardPaths.pop();
}

void FileExplorer::HandleCustomContextMenuRequest(const QPoint &pos) {
    QModelIndex index = ui->filesTree->indexAt(pos);

    if (!index.isValid()) {
        return;
    }

    QMenu menu(ui->filesTree);
    QAction renameAction("Zmień nazwę pliku", &menu);

    connect(&renameAction, &QAction::triggered,
        this, [&]() {
        QString oldFilePath = model->filePath(index);
        QString oldFileName = model->fileName(index);
        QString newFileName = QInputDialog::getText(ui->filesTree, "Zmień nazwę pliku", "Nowa nazwa pliku:", QLineEdit::Normal, oldFileName);
        QFile file(oldFilePath);

        if (file.exists()) {
            QString newPath = QDir(model->filePath(index)).filePath(newFileName);
            if (file.rename(newPath)) {
                qDebug() << "Plik został pomyślnie zmieniony na" << newPath;
            } else {
                qDebug() << "Nie można zmienić nazwy pliku." << file.errorString();
            }
        }
    });

    menu.addAction(&renameAction);
    menu.exec(ui->filesTree->mapToGlobal(pos));
}

bool FileExplorer::IsValidPath(const QString &path) {
    QDir dir(path);
    return dir.exists();
}

