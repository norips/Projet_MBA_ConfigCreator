#include "modified.h"
#include <QDebug>

Modified::Modified()
{   mod = true;
}

bool Modified::modified() {
    return mod;
}

void Modified::setModified(bool val) {
    mod = val;
    qDebug()<< "Modified" << val;
}
