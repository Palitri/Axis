#ifndef QMWEDITORWIDGET_H
#define QMWEDITORWIDGET_H

#include "IAxEditor.h"

class QAxEditorWidget
{
public:
    IAxEditor *editor;

    QAxEditorWidget();

    virtual void SetEditorInterface(IAxEditor *editor);
};

#endif // QMWEDITORWIDGET_H
