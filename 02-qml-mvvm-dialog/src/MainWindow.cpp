#include "MainWindow.h"

#include "PersonDialog.h"
#include "PersonViewModel.h"

#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) : QWidget(parent)
{
    setWindowTitle(tr("02 - QML MVVM Dialog"));

    m_summary = new QLabel(this);
    auto *editButton = new QPushButton(tr("Edit…"), this);
    connect(editButton, &QPushButton::clicked, this, &MainWindow::editPerson);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_summary);
    layout->addWidget(editButton);

    showPerson();
}

void MainWindow::editPerson()
{
    PersonViewModel viewModel;
    viewModel.load(m_person);

    PersonDialog dialog(&viewModel, this);
    if (dialog.exec() == QDialog::Accepted) {
        m_person = viewModel.toPerson();
        showPerson();
    }
}

void MainWindow::showPerson()
{
    m_summary->setText(tr("<b>%1</b><br>%2<br>%3 years, %4")
                           .arg(m_person.name.toHtmlEscaped(), m_person.email.toHtmlEscaped())
                           .arg(m_person.age)
                           .arg(PersonViewModel::displayName(m_person.role)));
}
