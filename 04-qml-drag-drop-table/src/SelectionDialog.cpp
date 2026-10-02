#include "SelectionDialog.h"

#include "SelectionViewModel.h"

#include <QDebug>
#include <QDialogButtonBox>
#include <QQmlComponent>
#include <QQuickItem>
#include <QQuickWidget>
#include <QQuickWindow>
#include <QVBoxLayout>

SelectionDialog::SelectionDialog(SelectionViewModel *viewModel, QWidget *parent)
    : QDialog(parent), m_viewModel(viewModel)
{
    setWindowTitle(tr("04 - QML Drag & Drop Table"));
    resize(720, 480);

    m_quickWidget = new QQuickWidget(this);

    auto *buttons = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel, this);
    connect(buttons, &QDialogButtonBox::accepted, this, &QDialog::accept);
    connect(buttons, &QDialogButtonBox::rejected, this, &QDialog::reject);

    auto *layout = new QVBoxLayout(this);
    layout->addWidget(m_quickWidget);
    layout->addWidget(buttons);

    loadView();
}

SelectionDialog::~SelectionDialog() = default;

void SelectionDialog::loadView()
{
    QQmlComponent component(m_quickWidget->engine());
    component.loadFromModule("DragDropQml", "SelectionView");

    const QVariantMap initialProperties{{QStringLiteral("viewModel"), QVariant::fromValue(m_viewModel)}};
    m_view.reset(qobject_cast<QQuickItem *>(component.createWithInitialProperties(initialProperties)));
    if (!m_view) {
        qWarning().noquote() << component.errorString();
        return;
    }

    m_view->setParentItem(m_quickWidget->quickWindow()->contentItem());
}
