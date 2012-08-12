#ifndef LOADER_H
#define LOADER_H

#include "shared.h"

namespace Loader
{
    volDim* loadHeader(char *path);
    volume* loadVolume(char* path);
}

#endif // LOADER_H
