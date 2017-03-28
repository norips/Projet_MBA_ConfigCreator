#include "modified.h"

Modified::Modified()
{   mod = true;
}

bool Modified::modified() {
    return mod;
}

void Modified::setModified(bool val) {
    mod = val;
}
