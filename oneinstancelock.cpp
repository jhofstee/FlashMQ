#include "oneinstancelock.h"

#include <stdexcept>

#include "utils.h"

OneInstanceLock::OneInstanceLock()
{
    std::string dir("/tmp");

    char *d = getenv("HOME");
    if (d != NULL && d[0] == '/')
    {
        dir = std::string(d);
    }
    lockFilePath = dir + "/.FlashMQ.lock";
}

OneInstanceLock::~OneInstanceLock()
{
    unlock();
}

void OneInstanceLock::lock()
{
    fd = open(lockFilePath.c_str(), O_RDWR | O_CREAT, 0600);
    if (fd < 0)
         throw std::runtime_error(formatString("Can't create '%s': %s", lockFilePath.c_str(), strerror(errno)));

    struct flock fl;
    fl.l_start = 0;
    fl.l_len = 0;
    fl.l_type = F_WRLCK;
    fl.l_whence = SEEK_SET;
    if (fcntl(fd, F_SETLK, &fl) < 0)
    {
        throw std::runtime_error("Can't acquire lock: another instance is already running?");
    }
}

void OneInstanceLock::unlock()
{
    if (fd > 0)
    {
        close(fd);
        fd = 0;
        if (!lockFilePath.empty())
        {
            if (unlink(lockFilePath.c_str()) < 0)
            {
                logger->logf(LOG_ERR, "Can't delete '%': %s", lockFilePath.c_str(), strerror(errno));
            }
            lockFilePath.clear();
        }
    }
}
