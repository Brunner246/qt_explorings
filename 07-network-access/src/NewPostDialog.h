#pragma once

#include "Post.h"

#include <QDialog>

class QLineEdit;
class QPlainTextEdit;

class NewPostDialog: public QDialog
{
    Q_OBJECT

public:
    explicit NewPostDialog(QWidget *parent = nullptr);

    [[nodiscard]] Post post() const;

private:
    QLineEdit *m_titleEdit = nullptr;
    QPlainTextEdit *m_bodyEdit = nullptr;
};
