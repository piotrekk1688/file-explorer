#ifndef MENUWIDGET_H
#define MENUWIDGET_H

#include <QWidget>

class MenuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit MenuWidget(QWidget *parent = nullptr);

public slots:
    void CustomContextMenuRequested(QPoint &pos);
};

#endif // MENUWIDGET_H
