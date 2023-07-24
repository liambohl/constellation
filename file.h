#pragma once
#include <shobjidl.h>

bool open_cst_file(PWSTR* file_path, IShellItem* folder);

bool save_as_cst_file(PWSTR* file_path, IShellItem* folder);
