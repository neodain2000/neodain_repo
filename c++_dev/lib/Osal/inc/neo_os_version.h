#ifndef __NEO_OS_VERSION_H__
#define __NEO_OS_VERSION_H__

namespace neodain {
namespace neo {
namespace os {

#define OS_MAJOR_VERSION 1
#define OS_MINOR_VERSION 1
#define OS_REVISION      0
#define OS_MISSION_REV   0

// 10100
#define OSAL_API_VERSION ((OS_MAJOR_VERSION * 10000) + (OS_MINOR_VERSION * 100) + OS_REVISION)

} // namespace os
} // namespace neo
} // namespace neodain

#endif // __NEO_OS_VERSION_H__

