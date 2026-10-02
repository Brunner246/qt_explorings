#pragma once

#include <QStyledItemDelegate>

class ContactDelegate: public QStyledItemDelegate
{
    Q_OBJECT

public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const override;
    [[nodiscard]] QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const override;

protected:
    bool editorEvent(QEvent *event,
                     QAbstractItemModel *model,
                     const QStyleOptionViewItem &option,
                     const QModelIndex &index) override;

private:
    void paintAvatar(QPainter *painter, const QRect &rect, const QModelIndex &index) const;
    void paintTexts(QPainter *painter,
                    const QRect &rect,
                    const QStyleOptionViewItem &option,
                    const QModelIndex &index) const;
    void paintStar(QPainter *painter, const QRect &rect, const QModelIndex &index) const;
};
