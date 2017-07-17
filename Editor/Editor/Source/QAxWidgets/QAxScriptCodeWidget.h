#ifndef QMWSCRIPTCODEWIDGET_H
#define QMWSCRIPTCODEWIDGET_H

#include <QWidget>

#include <QPlainTextEdit>

#include "QAxEditorWidget.h"

class QAxScriptCodeWidget :
        public QWidget,
        public QAxEditorWidget
{
    Q_OBJECT
private:
    QPlainTextEdit *sourceCodeEdit, *compilationMessagesEdit;

public:
    AxScript *script;

    explicit QAxScriptCodeWidget(QWidget *parent = 0);

    void SetResource(AxResource *resource);

signals:

public slots:
    void BuildClicked();
    void RunClicked();

    void SourceCodeChanged();
};

#endif // QMWSCRIPTCODEWIDGET_H
