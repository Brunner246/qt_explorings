#include "ContactDelegate.h"
#include "ContactListModel.h"

#include <QApplication>
#include <QListView>

namespace
{
QList<Contact> sampleContacts()
{
    return {
        {"Ada Lovelace", "Software Engineer", "ada@example.com", Presence::Online, true},
        {"Alan Turing", "Research Lead", "alan@example.com", Presence::Busy, false},
        {"Grace Hopper", "Compiler Architect", "grace@example.com", Presence::Away, true},
        {"Linus Torvalds", "Kernel Maintainer", "linus@example.com", Presence::Offline, false},
        {"Margaret Hamilton", "Flight Software Director", "margaret@example.com", Presence::Online, false},
        {"Dennis Ritchie", "Systems Programmer", "dennis@example.com", Presence::Offline, false},
        {"Barbara Liskov", "Professor", "barbara@example.com", Presence::Online, false},
        {"Bjarne Stroustrup", "Language Designer", "bjarne@example.com", Presence::Away, true},
        {"Frances Allen", "Optimization Researcher", "frances@example.com", Presence::Busy, false},
        {"Ken Thompson", "Unix Co-Creator", "ken@example.com", Presence::Online, false},
        {"Hedy Lamarr", "Inventor", "hedy@example.com", Presence::Offline, false},
        {"Donald Knuth", "Author", "don@example.com", Presence::Away, false},
    };
}
} // namespace

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    ContactListModel model(sampleContacts());
    ContactDelegate delegate;

    QListView view;
    view.setWindowTitle(QObject::tr("06 - Custom Delegate ListView"));
    view.setModel(&model);
    view.setItemDelegate(&delegate);
    view.setUniformItemSizes(true);
    view.setMouseTracking(true);
    view.setSelectionMode(QAbstractItemView::SingleSelection);
    view.setVerticalScrollMode(QAbstractItemView::ScrollPerPixel);
    view.resize(460, 560);
    view.show();

    return QApplication::exec();
}
