#include "ContactDelegate.h"

#include "Contact.h"
#include "ContactListModel.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>
#include <QtMath>

namespace
{
constexpr int cardHeight = 68;
constexpr int margin = 6;
constexpr int avatarSize = 44;
constexpr int starSize = 24;

QRect cardRect(const QRect &itemRect) { return itemRect.adjusted(margin, margin / 2, -margin, -margin / 2); }

QRect avatarRect(const QRect &card)
{
    return {card.left() + margin * 2, card.center().y() - avatarSize / 2, avatarSize, avatarSize};
}

QRect starRect(const QRect &card)
{
    return {card.right() - margin * 2 - starSize, card.center().y() - starSize / 2, starSize, starSize};
}

QString initials(const QString &name)
{
    const auto parts = name.split(u' ', Qt::SkipEmptyParts);
    if (parts.isEmpty())
        return {};
    QString result = parts.first().left(1);
    if (parts.size() > 1)
        result += parts.last().left(1);
    return result.toUpper();
}

QColor presenceColor(const Presence presence)
{
    switch (presence) {
    case Presence::Online:
        return QColor(0x2e, 0xcc, 0x71);
    case Presence::Away:
        return QColor(0xf3, 0x9c, 0x12);
    case Presence::Busy:
        return QColor(0xe7, 0x4c, 0x3c);
    case Presence::Offline:
        return QColor(0x95, 0xa5, 0xa6);
    }
    return Qt::gray;
}

QPainterPath starPath(const QRectF &rect)
{
    QPainterPath path;
    const auto center = rect.center();
    const qreal outer = rect.width() / 2.0;
    const qreal inner = outer * 0.45;
    for (int i = 0; i < 10; ++i) {
        const qreal radius = i % 2 == 0 ? outer : inner;
        const qreal angle = qDegreesToRadians(-90.0 + i * 36.0);
        const QPointF point(center.x() + radius * qCos(angle), center.y() + radius * qSin(angle));
        i == 0 ? path.moveTo(point) : path.lineTo(point);
    }
    path.closeSubpath();
    return path;
}
} // namespace

void ContactDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    const bool selected = option.state.testFlag(QStyle::State_Selected);
    const bool hovered = option.state.testFlag(QStyle::State_MouseOver);
    const QRect card = cardRect(option.rect);

    painter->save();
    painter->setRenderHint(QPainter::Antialiasing);

    const QColor background = selected ? option.palette.color(QPalette::Highlight)
        : hovered                      ? option.palette.color(QPalette::AlternateBase)
                                       : option.palette.color(QPalette::Base);
    painter->setPen(option.palette.color(QPalette::Mid));
    painter->setBrush(background);
    painter->drawRoundedRect(QRectF(card).adjusted(0.5, 0.5, -0.5, -0.5), 8, 8);

    paintAvatar(painter, avatarRect(card), index);
    paintTexts(painter, card, option, index);
    paintStar(painter, starRect(card), index);

    painter->restore();
}

QSize ContactDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &) const
{
    return {option.rect.width(), cardHeight};
}

bool ContactDelegate::editorEvent(QEvent *event,
                                  QAbstractItemModel *model,
                                  const QStyleOptionViewItem &option,
                                  const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonRelease) {
        const auto *mouseEvent = static_cast<QMouseEvent *>(event);
        if (starRect(cardRect(option.rect)).contains(mouseEvent->position().toPoint())) {
            const bool favorite = index.data(ContactListModel::FavoriteRole).toBool();
            return model->setData(index, !favorite, ContactListModel::FavoriteRole);
        }
    }
    return QStyledItemDelegate::editorEvent(event, model, option, index);
}

void ContactDelegate::paintAvatar(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    const auto name = index.data(ContactListModel::NameRole).toString();
    const auto presence = index.data(ContactListModel::PresenceRole).value<Presence>();

    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor::fromHsv(static_cast<int>(qHash(name) % 360), 110, 190));
    painter->drawEllipse(rect);

    QFont font = painter->font();
    font.setBold(true);
    font.setPixelSize(rect.height() / 3);
    painter->setFont(font);
    painter->setPen(Qt::white);
    painter->drawText(rect, Qt::AlignCenter, initials(name));

    const QRect badge(rect.right() - 11, rect.bottom() - 11, 12, 12);
    painter->setPen(QPen(Qt::white, 2));
    painter->setBrush(presenceColor(presence));
    painter->drawEllipse(badge);
}

void ContactDelegate::paintTexts(QPainter *painter,
                                 const QRect &card,
                                 const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    const bool selected = option.state.testFlag(QStyle::State_Selected);
    const int left = avatarRect(card).right() + margin * 2;
    const int right = starRect(card).left() - margin * 2;
    const QRect textRect(left, card.top(), right - left, card.height());

    QFont nameFont = option.font;
    nameFont.setBold(true);
    QFont detailFont = option.font;
    detailFont.setPointSizeF(option.font.pointSizeF() * 0.9);

    const QFontMetrics nameMetrics(nameFont);
    const QFontMetrics detailMetrics(detailFont);
    const int spacing = 2;
    const int blockHeight = nameMetrics.height() + spacing + detailMetrics.height();
    const QRect nameRect(
        textRect.left(), textRect.center().y() - blockHeight / 2, textRect.width(), nameMetrics.height());
    const QRect detailRect(textRect.left(), nameRect.bottom() + spacing, textRect.width(), detailMetrics.height());

    const auto name = index.data(ContactListModel::NameRole).toString();
    const auto details = QStringLiteral("%1 · %2").arg(index.data(ContactListModel::TitleRole).toString(),
                                                       index.data(ContactListModel::EmailRole).toString());

    painter->setPen(option.palette.color(selected ? QPalette::HighlightedText : QPalette::Text));
    painter->setFont(nameFont);
    painter->drawText(
        nameRect, Qt::AlignLeft | Qt::AlignVCenter, nameMetrics.elidedText(name, Qt::ElideRight, nameRect.width()));

    painter->setPen(option.palette.color(selected ? QPalette::HighlightedText : QPalette::PlaceholderText));
    painter->setFont(detailFont);
    painter->drawText(detailRect,
                      Qt::AlignLeft | Qt::AlignVCenter,
                      detailMetrics.elidedText(details, Qt::ElideRight, detailRect.width()));
}

void ContactDelegate::paintStar(QPainter *painter, const QRect &rect, const QModelIndex &index) const
{
    const bool favorite = index.data(ContactListModel::FavoriteRole).toBool();
    const QColor gold(0xf1, 0xc4, 0x0f);

    painter->setPen(QPen(favorite ? gold.darker(120) : QColor(0x95, 0xa5, 0xa6), 1.5));
    painter->setBrush(favorite ? gold : Qt::transparent);
    painter->drawPath(starPath(QRectF(rect).adjusted(2, 2, -2, -2)));
}
