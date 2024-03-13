#ifndef FILEEXPLORER_H
#define FILEEXPLORER_H

#include <QFile>
#include <QMainWindow>
#include <QInputDialog>
#include <QFileSystemModel>
#include <stack>

QT_BEGIN_NAMESPACE
namespace Ui {
class FileExplorer;
}
QT_END_NAMESPACE

class FileExplorer : public QMainWindow
{
    Q_OBJECT

public:
    FileExplorer(QWidget *parent = nullptr);
    ~FileExplorer();

private:
    QFileSystemModel *model;
    Ui::FileExplorer *ui;
    std::stack<QString> backPaths;
    std::stack<QString> forwardPaths;
    QString previousPath;

    void ShowPath(const QString &);
    bool IsValidPath(const QString &);

private slots:
    void UpdatePath();
    void UpdatePath(const QString&, bool = true);
    void UpdateIndex(const QModelIndex &);
    void BackButtonPressed();
    void ForwardButtonPressed();
    void HandleCustomContextMenuRequest(const QPoint&);
};
#endif // FILEEXPLORER_H
