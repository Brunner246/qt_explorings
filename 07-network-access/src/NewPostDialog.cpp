#include "NewPostDialog.h"

#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLineEdit>
#include <QPlainTextEdit>
#include <QPushButton>

NewPostDialog::NewPostDialog(QWidget *parent) : QDialog(parent)
{
    setWindowTitle(tr("New Post"));

    m_titleEdit = new QLineEdit(this);
    m_bodyEdit = new QPlainTextEdit(this);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    buttons->button(QDialogButtonBox::Ok)->setText(tr("Publish"));
    buttons->button(QDialogButtonBox::Ok)->setEnabled(false);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_titleEdit,
            &QLineEdit::textChanged,
            this,
            [buttons](const QString &text)
            { buttons->button(QDialogButtonBox::Ok)->setEnabled(!text.trimmed().isEmpty()); });

    auto *layout = new QFormLayout(this);
    layout->addRow(tr("&Title:"), m_titleEdit);
    layout->addRow(tr("&Body:"), m_bodyEdit);
    layout->addRow(buttons);
}

Post NewPostDialog::post() const
{
    return Post{.title = m_titleEdit->text().trimmed(), .body = m_bodyEdit->toPlainText().trimmed()};
}
