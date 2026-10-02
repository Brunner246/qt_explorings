#include "PersonDialog.h"

#include "PersonViewModel.h"

#include <QComboBox>
#include <QDialogButtonBox>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSpinBox>
#include <QVBoxLayout>

PersonDialog::PersonDialog(PersonViewModel *viewModel, QWidget *parent) : QDialog(parent), m_viewModel(viewModel)
{
    setWindowTitle(tr("Edit Person"));
    createWidgets();
    bindViewModel();
    updateFromViewModel();
}

void PersonDialog::createWidgets()
{
    m_nameEdit = new QLineEdit(this);
    m_emailEdit = new QLineEdit(this);
    m_ageSpin = new QSpinBox(this);
    m_ageSpin->setRange(m_viewModel->minimumAge(), m_viewModel->maximumAge());
    m_roleCombo = new QComboBox(this);
    m_roleCombo->addItems(m_viewModel->roleNames());

    m_validationLabel = new QLabel(this);
    m_validationLabel->setStyleSheet(QStringLiteral("color: #c0392b;"));

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset, this);

    auto *form = new QFormLayout;
    form->addRow(tr("&Name:"), m_nameEdit);
    form->addRow(tr("&Email:"), m_emailEdit);
    form->addRow(tr("&Age:"), m_ageSpin);
    form->addRow(tr("&Role:"), m_roleCombo);

    auto *layout = new QVBoxLayout(this);
    layout->addLayout(form);
    layout->addWidget(m_validationLabel);
    layout->addWidget(m_buttons);
}

void PersonDialog::bindViewModel()
{
    connect(m_nameEdit, &QLineEdit::textEdited, m_viewModel, &PersonViewModel::setName);
    connect(m_emailEdit, &QLineEdit::textEdited, m_viewModel, &PersonViewModel::setEmail);
    connect(m_ageSpin, &QSpinBox::valueChanged, m_viewModel, &PersonViewModel::setAge);
    connect(m_roleCombo, &QComboBox::currentIndexChanged, m_viewModel, &PersonViewModel::setRoleIndex);

    for (auto signal : {&PersonViewModel::nameChanged,
                        &PersonViewModel::emailChanged,
                        &PersonViewModel::ageChanged,
                        &PersonViewModel::roleIndexChanged,
                        &PersonViewModel::validationChanged})
        connect(m_viewModel, signal, this, &PersonDialog::updateFromViewModel);

    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_buttons->button(QDialogButtonBox::Reset), &QPushButton::clicked, m_viewModel, &PersonViewModel::revert);
}

void PersonDialog::updateFromViewModel()
{
    // Only touch line edits whose text differs, otherwise the cursor would jump while typing.
    if (m_nameEdit->text() != m_viewModel->name())
        m_nameEdit->setText(m_viewModel->name());
    if (m_emailEdit->text() != m_viewModel->email())
        m_emailEdit->setText(m_viewModel->email());

    m_ageSpin->setValue(m_viewModel->age());
    m_roleCombo->setCurrentIndex(m_viewModel->roleIndex());
    m_validationLabel->setText(m_viewModel->validationMessage());
    m_buttons->button(QDialogButtonBox::Ok)->setEnabled(m_viewModel->isValid());
}
