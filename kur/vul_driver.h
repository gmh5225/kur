#pragma once
#include "vul_driver.h"

#pragma comment(lib, "ntdll.lib")

constexpr auto SE_LOAD_DRIVER_PRIVILEGE = 10L;
#define SERVICE_PATH_COMMON std::wstring(L"SYSTEM\\CurrentControlSet\\Services\\")
constexpr auto VUL_DRIVER_INITIALISE_IOCTL = 0x9e6a0594;
constexpr auto VUL_DRIVER_COPY_IOCTL = 0x60A26124;
constexpr auto VUL_DRIVER_GET_HANDLE_IOCTL = 0xE6224248;

class vul_driver
{
  std::wstring driver_name;
  std::wstring device_name;

  auto get_full_driver_path() const -> std::wstring;

public:
  HANDLE h_device = INVALID_HANDLE_VALUE;

  vul_driver(std::wstring name, std::wstring device_name)
    : driver_name(std::move(name)), device_name(std::move(device_name))
  {
  }

  auto ioctl_initialize_driver() const -> BOOL;
  auto ioctl_get_process_handle(DWORD pid, ACCESS_MASK access_mask) -> HANDLE;
  auto ioctl_mm_copy_virtual_memory(void* from_address, void* to_address, size_t len, HANDLE h_target_process) -> BOOL;
  auto get_device_handle() -> BOOL;

  auto load() const -> BOOL;
  auto install() const -> BOOL;
  auto setup_reg_key() const -> BOOL;

  // cleanup functions
  auto uninstall() const -> BOOL;
  auto delete_reg_key() const -> BOOL;
  auto unload() const -> BOOL;
};

struct get_handle_buffer_t
{
  DWORD pid;
  ACCESS_MASK access;
  HANDLE h_process;
};

struct copy_buffer_t
{
  HANDLE h_target_process;
  void* from_address;
  void* to_address;
  size_t buffer_size;
  size_t* number_of_bytes_copied;
  int status;
  size_t maybe_size;
};
