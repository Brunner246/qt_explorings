#include "PersonDialog.h"

#include "PersonViewModel.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QPushButton>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWidget>
#include <QQuickWindow>
#include <QVBoxLayout>
#include <QtMath>

PersonDialog::PersonDialog(PersonViewModel *viewModel, QWidget *parent) : QDialog(parent), m_viewModel(viewModel)
{
    setWindowTitle(tr("Edit Person"));

    m_quickWidget = new QQuickWidget(this);

    m_buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset, this);
    connect(m_buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(m_buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);
    connect(m_buttons->button(QDialogButtonBox::Reset), &QPushButton::clicked, m_viewModel, &PersonViewModel::revert);
    connect(m_viewModel, &PersonViewModel::validationChanged, this, &PersonDialog::updateOkButton);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_quickWidget);
    layout->addWidget(m_buttons);

    loadForm();
    updateOkButton();
}

PersonDialog::~PersonDialog() = default;

void PersonDialog::loadForm()
{
    QQmlComponent component(m_quickWidget->engine());
    component.loadFromModule("MvvmQml", "PersonForm");

    const QVariantMap initialProperties{{QStringLiteral("viewModel"), QVariant::fromValue(m_viewModel)}};
    m_form.reset(qobject_cast<QQuickItem *>(component.createWithInitialProperties(initialProperties)));
    if (!m_form) {
        qWarning().noquote() << component.errorString();
        return;
    }

    m_form->setParentItem(m_quickWidget->quickWindow()->contentItem());
    m_quickWidget->setMinimumSize(qCeil(m_form->implicitWidth()), qCeil(m_form->implicitHeight()));
}

void PersonDialog::updateOkButton() { m_buttons->button(QDialogButtonBox::Ok)->setEnabled(m_viewModel->isValid()); }
