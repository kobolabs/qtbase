/****************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (qt-info@nokia.com)
**
** This file is part of the QtGui module of the Qt Toolkit.
**
** $QT_BEGIN_LICENSE:LGPL$
** GNU Lesser General Public License Usage
** This file may be used under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation and
** appearing in the file LICENSE.LGPL included in the packaging of this
** file. Please review the following information to ensure the GNU Lesser
** General Public License version 2.1 requirements will be met:
** http://www.gnu.org/licenses/old-licenses/lgpl-2.1.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt LGPL Exception
** version 1.1, included in the file LGPL_EXCEPTION.txt in this package.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU General
** Public License version 3.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of this
** file. Please review the following information to ensure the GNU General
** Public License version 3.0 requirements will be met:
** http://www.gnu.org/copyleft/gpl.html.
**
** Other Usage
** Alternatively, this file may be used in accordance with the terms and
** conditions contained in a signed written agreement between you and Nokia.
**
**
**
**
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef QACCESSIBLE_H
#define QACCESSIBLE_H

#include <QtCore/qglobal.h>
#include <QtCore/qobject.h>
#include <QtCore/qrect.h>
#include <QtCore/qset.h>
#include <QtCore/qvector.h>
#include <QtCore/qvariant.h>
#include <QtGui/qcolor.h>
#include <QtGui/qevent.h>

QT_BEGIN_HEADER

QT_BEGIN_NAMESPACE

QT_MODULE(Gui)

#ifndef QT_NO_ACCESSIBILITY

class QAccessibleInterface;
class QWindow;

// We need to inherit QObject to expose the enums to QML.
class Q_GUI_EXPORT QAccessible
#ifndef qdoc
        :public QObject
#endif
{
    Q_OBJECT
    Q_ENUMS(Role Event State)
public:

    enum Event {
        SoundPlayed          = 0x0001,
        Alert                = 0x0002,
        ForegroundChanged    = 0x0003,
        MenuStart            = 0x0004,
        MenuEnd              = 0x0005,
        PopupMenuStart       = 0x0006,
        PopupMenuEnd         = 0x0007,
        ContextHelpStart     = 0x000C,
        ContextHelpEnd       = 0x000D,
        DragDropStart        = 0x000E,
        DragDropEnd          = 0x000F,
        DialogStart          = 0x0010,
        DialogEnd            = 0x0011,
        ScrollingStart       = 0x0012,
        ScrollingEnd         = 0x0013,

        MenuCommand          = 0x0018,

        // Values from IAccessible2
        ActionChanged                    = 0x0101,
        ActiveDescendantChanged          = 0x0102,
        AttributeChanged                 = 0x0103,
        DocumentContentChanged           = 0x0104,
        DocumentLoadComplete             = 0x0105,
        DocumentLoadStopped              = 0x0106,
        DocumentReload                   = 0x0107,
        HyperlinkEndIndexChanged         = 0x0108,
        HyperlinkNumberOfAnchorsChanged  = 0x0109,
        HyperlinkSelectedLinkChanged     = 0x010A,
        HypertextLinkActivated           = 0x010B,
        HypertextLinkSelected            = 0x010C,
        HyperlinkStartIndexChanged       = 0x010D,
        HypertextChanged                 = 0x010E,
        HypertextNLinksChanged           = 0x010F,
        ObjectAttributeChanged           = 0x0110,
        PageChanged                      = 0x0111,
        SectionChanged                   = 0x0112,
        TableCaptionChanged              = 0x0113,
        TableColumnDescriptionChanged    = 0x0114,
        TableColumnHeaderChanged         = 0x0115,
        TableModelChanged                = 0x0116,
        TableRowDescriptionChanged       = 0x0117,
        TableRowHeaderChanged            = 0x0118,
        TableSummaryChanged              = 0x0119,
        TextAttributeChanged             = 0x011A,
        TextCaretMoved                   = 0x011B,
        // TextChanged = 0x011C, is deprecated in IA2, use TextUpdated
        TextColumnChanged                = 0x011D,
        TextInserted                     = 0x011E,
        TextRemoved                      = 0x011F,
        TextUpdated                      = 0x0120,
        TextSelectionChanged             = 0x0121,
        VisibleDataChanged               = 0x0122,

        ObjectCreated        = 0x8000,
        ObjectDestroyed      = 0x8001,
        ObjectShow           = 0x8002,
        ObjectHide           = 0x8003,
        ObjectReorder        = 0x8004,
        Focus                = 0x8005,
        Selection            = 0x8006,
        SelectionAdd         = 0x8007,
        SelectionRemove      = 0x8008,
        SelectionWithin      = 0x8009,
        StateChanged         = 0x800A,
        LocationChanged      = 0x800B,
        NameChanged          = 0x800C,
        DescriptionChanged   = 0x800D,
        ValueChanged         = 0x800E,
        ParentChanged        = 0x800F,
        HelpChanged          = 0x80A0,
        DefaultActionChanged = 0x80B0,
        AcceleratorChanged   = 0x80C0
    };


    enum StateFlag {
        Normal          = 0x00000000,
        Unavailable     = 0x00000001,
        Selected        = 0x00000002,
        Focused         = 0x00000004,
        Pressed         = 0x00000008,
        Checked         = 0x00000010,
        Mixed           = 0x00000020,
        ReadOnly        = 0x00000040,
        HotTracked      = 0x00000080,
        DefaultButton   = 0x00000100,
        Expanded        = 0x00000200,
        Collapsed       = 0x00000400,
        Busy            = 0x00000800,
        // Floating        = 0x00001000,
        Expandable      = 0x00001000,
        Marqueed        = 0x00002000,
        Animated        = 0x00004000,
        Invisible       = 0x00008000,
        Offscreen       = 0x00010000,
        Sizeable        = 0x00020000,
        Movable         = 0x00040000,
        SelfVoicing     = 0x00080000,
        Focusable       = 0x00100000,
        Selectable      = 0x00200000,
        Linked          = 0x00400000,
        Traversed       = 0x00800000,
        MultiSelectable = 0x01000000,
        ExtSelectable   = 0x02000000,
        //AlertLow        = 0x04000000,
        //AlertMedium     = 0x08000000,
        //AlertHigh       = 0x10000000, /* reused for HasInvokeExtension */
        Protected       = 0x20000000,
        HasPopup        = 0x40000000,
        Modal           = 0x80000000

    };
    Q_DECLARE_FLAGS(State, StateFlag)


    enum Role {
        NoRole         = 0x00000000,
        TitleBar       = 0x00000001,
        MenuBar        = 0x00000002,
        ScrollBar      = 0x00000003,
        Grip           = 0x00000004,
        Sound          = 0x00000005,
        Cursor         = 0x00000006,
        Caret          = 0x00000007,
        AlertMessage   = 0x00000008,
        Window         = 0x00000009,
        Client         = 0x0000000A,
        PopupMenu      = 0x0000000B,
        MenuItem       = 0x0000000C,
        ToolTip        = 0x0000000D,
        Application    = 0x0000000E,
        Document       = 0x0000000F,
        Pane           = 0x00000010,
        Chart          = 0x00000011,
        Dialog         = 0x00000012,
        Border         = 0x00000013,
        Grouping       = 0x00000014,
        Separator      = 0x00000015,
        ToolBar        = 0x00000016,
        StatusBar      = 0x00000017,
        Table          = 0x00000018,
        ColumnHeader   = 0x00000019,
        RowHeader      = 0x0000001A,
        Column         = 0x0000001B,
        Row            = 0x0000001C,
        Cell           = 0x0000001D,
        Link           = 0x0000001E,
        HelpBalloon    = 0x0000001F,
        Assistant      = 0x00000020,
        List           = 0x00000021,
        ListItem       = 0x00000022,
        Tree           = 0x00000023,
        TreeItem       = 0x00000024,
        PageTab        = 0x00000025,
        PropertyPage   = 0x00000026,
        Indicator      = 0x00000027,
        Graphic        = 0x00000028,
        StaticText     = 0x00000029,
        EditableText   = 0x0000002A,  // Editable, selectable, etc.
        Button         = 0x0000002B,
#ifndef qdoc
        PushButton     = Button, // deprecated
#endif
        CheckBox       = 0x0000002C,
        RadioButton    = 0x0000002D,
        ComboBox       = 0x0000002E,
        // DropList       = 0x0000002F,
        ProgressBar    = 0x00000030,
        Dial           = 0x00000031,
        HotkeyField    = 0x00000032,
        Slider         = 0x00000033,
        SpinBox        = 0x00000034,
        Canvas         = 0x00000035,
        Animation      = 0x00000036,
        Equation       = 0x00000037,
        ButtonDropDown = 0x00000038,
        ButtonMenu     = 0x00000039,
        ButtonDropGrid = 0x0000003A,
        Whitespace     = 0x0000003B,
        PageTabList    = 0x0000003C,
        Clock          = 0x0000003D,
        Splitter       = 0x0000003E,
        // Additional Qt roles where enum value does not map directly to MSAA:
        LayeredPane    = 0x0000003F,
        UserRole       = 0x0000ffff
    };

    enum Text {
        Name         = 0,
        Description,
        Value,
        Help,
        Accelerator,
        DebugDescription,
        UserText     = 0x0000ffff
    };

    enum RelationFlag {
        Unrelated     = 0x00000000,
        Self          = 0x00000001,
        Ancestor      = 0x00000002,
        Child         = 0x00000004,
        Descendent    = 0x00000008,
        Sibling       = 0x00000010,
        HierarchyMask = 0x000000ff,

        Up            = 0x00000100,
        Down          = 0x00000200,
        Left          = 0x00000400,
        Right         = 0x00000800,
        Covers        = 0x00001000,
        Covered       = 0x00002000,
        GeometryMask  = 0x0000ff00,

        FocusChild    = 0x00010000,
        Label         = 0x00020000,
        Labelled      = 0x00040000,
        Controller    = 0x00080000,
        Controlled    = 0x00100000,
        LogicalMask   = 0x00ff0000
    };
    Q_DECLARE_FLAGS(Relation, RelationFlag)

    enum Method {
        ListSupportedMethods      = 0,
        SetCursorPosition         = 1,
        GetCursorPosition         = 2
    };

    enum InterfaceType
    {
        TextInterface,
        EditableTextInterface,
        ValueInterface,
        ActionInterface,
        ImageInterface,
        TableInterface,
        TableCellInterface
    };

    typedef QAccessibleInterface*(*InterfaceFactory)(const QString &key, QObject*);
    typedef void(*UpdateHandler)(QObject*, int who, Event reason);
    typedef void(*RootObjectHandler)(QObject*);

    static void installFactory(InterfaceFactory);
    static void removeFactory(InterfaceFactory);
    static UpdateHandler installUpdateHandler(UpdateHandler);
    static RootObjectHandler installRootObjectHandler(RootObjectHandler);

    static QAccessibleInterface *queryAccessibleInterface(QObject *);
    static void updateAccessibility(QObject *, int who, Event reason);
    static bool isActive();
    static void setRootObject(QObject*);

    static void cleanup();

private:
    static UpdateHandler updateHandler;
    static RootObjectHandler rootObjectHandler;

    /*! @internal
      This class is purely a collection of enums and static functions,
      it is not supposed to be instantiated.
    */
    QAccessible() {}
};

Q_DECLARE_OPERATORS_FOR_FLAGS(QAccessible::State)
Q_DECLARE_OPERATORS_FOR_FLAGS(QAccessible::Relation)
QT_END_NAMESPACE
Q_DECLARE_METATYPE(QSet<QAccessible::Method>)
QT_BEGIN_NAMESPACE

class QAccessible2Interface;
class QAccessibleTextInterface;
class QAccessibleEditableTextInterface;
class QAccessibleValueInterface;
class QAccessibleActionInterface;
class QAccessibleImageInterface;
class QAccessibleTableInterface;
class QAccessibleTableCellInterface;

class Q_GUI_EXPORT QAccessibleInterface
{
public:
    virtual ~QAccessibleInterface() {}
    // check for valid pointers
    virtual bool isValid() const = 0;
    virtual QObject *object() const = 0;
    virtual QWindow *window() const;

    // relations
    virtual QAccessible::Relation relationTo(const QAccessibleInterface *other) const;
    virtual QVector<QPair<QAccessibleInterface*, QAccessible::Relation> > relations() const;

    virtual QAccessibleInterface *childAt(int x, int y) const = 0;

    // navigation, hierarchy
    virtual QAccessibleInterface *parent() const = 0;
    virtual QAccessibleInterface *child(int index) const = 0;
    virtual int childCount() const = 0;
    virtual int indexOfChild(const QAccessibleInterface *) const = 0;
    virtual int navigate(QAccessible::RelationFlag relation, int index, QAccessibleInterface **iface) const = 0;

    // properties and state
    virtual QString text(QAccessible::Text t) const = 0;
    virtual void setText(QAccessible::Text t, const QString &text) = 0;
    virtual QRect rect() const = 0;
    virtual QAccessible::Role role() const = 0;
    virtual QAccessible::State state() const = 0;
    // FIXME virtual QSet<QAccessible::State> states() const = 0;

    virtual QColor foregroundColor() const;
    virtual QColor backgroundColor() const;

    virtual QVariant invokeMethod(QAccessible::Method method, const QVariantList &params = QVariantList());

    inline QSet<QAccessible::Method> supportedMethods()
    { return qvariant_cast<QSet<QAccessible::Method> >(invokeMethod(QAccessible::ListSupportedMethods)); }

    inline QAccessibleTextInterface *textInterface()
    { return reinterpret_cast<QAccessibleTextInterface *>(interface_cast(QAccessible::TextInterface)); }

    inline QAccessibleEditableTextInterface *editableTextInterface()
    { return reinterpret_cast<QAccessibleEditableTextInterface *>(interface_cast(QAccessible::EditableTextInterface)); }

    inline QAccessibleValueInterface *valueInterface()
    { return reinterpret_cast<QAccessibleValueInterface *>(interface_cast(QAccessible::ValueInterface)); }

    inline QAccessibleActionInterface *actionInterface()
    { return reinterpret_cast<QAccessibleActionInterface *>(interface_cast(QAccessible::ActionInterface)); }

    inline QAccessibleImageInterface *imageInterface()
    { return reinterpret_cast<QAccessibleImageInterface *>(interface_cast(QAccessible::ImageInterface)); }

    inline QAccessibleTableInterface *tableInterface()
    { return reinterpret_cast<QAccessibleTableInterface *>(interface_cast(QAccessible::TableInterface)); }

    inline QAccessibleTableCellInterface *tableCellInterface()
    { return reinterpret_cast<QAccessibleTableCellInterface *>(interface_cast(QAccessible::TableCellInterface)); }

    // FIXME
    virtual QVariant virtual_hook(const QVariant &data);
    virtual void *interface_cast(QAccessible::InterfaceType)
    { return 0; }
private:
};

class QAccessibleEvent : public QEvent
{
public:
    inline QAccessibleEvent(Type type);
    inline QString value() const { return val; }
    inline void setValue(const QString &aText) { val = aText; }

private:
    QString val;
};

inline QAccessibleEvent::QAccessibleEvent(Type atype)
    : QEvent(atype) {}

#define QAccessibleInterface_iid "com.trolltech.Qt.QAccessibleInterface"
Q_DECLARE_INTERFACE(QAccessibleInterface, QAccessibleInterface_iid)

Q_GUI_EXPORT const char *qAccessibleRoleString(QAccessible::Role role);
Q_GUI_EXPORT const char *qAccessibleEventString(QAccessible::Event event);

#ifndef QT_NO_DEBUG_STREAM
Q_GUI_EXPORT QDebug operator<<(QDebug d, const QAccessibleInterface *iface);
#endif

#endif // QT_NO_ACCESSIBILITY

QT_END_NAMESPACE

QT_END_HEADER

#endif // QACCESSIBLE_H
