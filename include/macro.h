//
// Created by lan on 2022/5/23.
//

#ifndef NOTEBOOK_INCLUDE_MACRO_H_
#define NOTEBOOK_INCLUDE_MACRO_H_

#define READ_UINT32(addr) *reinterpret_cast<const uint32_t*>(addr)
#define WRITE_UINT32(addr) *reinterpret_cast<uint32_t*>(addr)

#endif //NOTEBOOK_INCLUDE_MACRO_H_
