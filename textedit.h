#ifndef TEXTEDIT_H
#define TEXTEDIT_H
#include <QTextEdit>
class TextEdit : public QTextEdit
{
    Q_OBJECT

public:
    TextEdit(const QString& text, QWidget* parent = 0)
        : QTextEdit(text, parent)
    {
        // make it look and feel like an ordinary label
        setReadOnly(true);
        setEnabled(false);
        setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        setFrameStyle(QFrame::NoFrame);
        QPalette pal = palette();
        pal.setColor(QPalette::Base, Qt::transparent);
        setPalette(pal);

        // wrap anywhere, adjust minimum height on the fly
        setLineWrapMode(QTextEdit::WidgetWidth);
        setWordWrapMode(QTextOption::WrapAtWordBoundaryOrAnywhere);
        document()->setDocumentMargin(0);
        /*QObject::connect(document()->documentLayout(),
            SIGNAL(documentSizeChanged(QSizeF)),
            this, SLOT(adjustMinimumSize(QSizeF)));*/
    }

private slots:
    void adjustMinimumSize(const QSizeF& size)
    {
        setMinimumHeight(size.height() + 2 * frameWidth());
    }
};

#endif // TEXTEDIT_H
