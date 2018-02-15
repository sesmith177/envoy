#pragma once

#include "envoy/common/socket_fd.h"
#if !defined(WIN32)
#include <sys/mman.h>   // for mode_t
#include <sys/socket.h> // for sockaddr
#else
#include <ws2tcpip.h>
#endif
#include <sys/stat.h>

#include <memory>
#include <string>

#include "envoy/common/pure.h"
#include "envoy/common/ssize.h"

namespace Envoy {
namespace Api {

class OsSysCalls {
public:
  virtual ~OsSysCalls() {}

  /**
   * @see bind (man 2 bind)
   */
  virtual int bind(SOCKET_FD_TYPE sockfd, const sockaddr* addr, socklen_t addrlen) PURE;

  /**
   * Open file by full_path with given flags and mode.
   * @return file descriptor.
   */
  virtual int open(const std::string& full_path, int flags, int mode) PURE;

  /**
   * Write num_bytes to fd from buffer.
   * @return number of bytes written if non negative, otherwise error code.
   */
  virtual ssize_t write(int fd, const void* buffer, size_t num_bytes) PURE;

  /**
   * Release all resources allocated for fd.
   * @return zero on success, -1 returned otherwise.
   */
  virtual int close(int fd) PURE;

#if !defined(WIN32)
  /**
   * @see shm_open (man 3 shm_open)
   */
  virtual int shmOpen(const char* name, int oflag, mode_t mode) PURE;

  /**
   * @see shm_unlink (man 3 shm_unlink)
   */
  virtual int shmUnlink(const char* name) PURE;
#endif

  /**
   * @see man 2 ftruncate
   */
  virtual int ftruncate(int fd, off_t length) PURE;

#if !defined(WIN32)
  /**
   * @see man 2 mmap
   */
  virtual void* mmap(void* addr, size_t length, int prot, int flags, int fd, off_t offset) PURE;
#endif

  /**
   * @see man 2 stat
   */
  virtual int stat(const char* pathname, struct stat* buf) PURE;
};

typedef std::unique_ptr<OsSysCalls> OsSysCallsPtr;

} // namespace Api
} // namespace Envoy