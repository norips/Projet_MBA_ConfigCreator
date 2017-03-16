#include "modified.h"

Modified::Modified()
{   mod = false;
}

bool Modified::modified() {
    return mod;
}

void Modified::setModified(bool val) {
    mod = val;
}
