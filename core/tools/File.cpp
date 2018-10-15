// Copyright (c) 2016-2018 Easy2D - Nomango
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

#include "..\e2dtool.h"
#include "..\e2dmodule.h"
#include <shobjidl.h> 

std::list<e2d::String>	e2d::File::search_paths_;

e2d::File::File()
	: file_path_()
	, attributes_(0)
{
}

e2d::File::File(const String & file_name)
	: file_path_(file_name)
	, attributes_(0)
{
	this->Open(file_name);
}

e2d::File::~File()
{
}

bool e2d::File::Open(const String & file_name)
{
	if (file_name.IsEmpty())
		return false;

	auto FindFile = [=](const String & path) -> bool
	{
		if (::_waccess((const wchar_t*)path, 0) == 0)
		{
			attributes_ = ::GetFileAttributes((LPCTSTR)path);
			return true;
		}
		return false;
	};

	if (FindFile(file_name))
	{
		file_path_ = file_name;
		return true;
	}
	else
	{
		for (const auto& path : search_paths_)
		{
			if (FindFile(path + file_name))
			{
				file_path_ = path + file_name;
				return true;
			}
		}
	}
	return false;
}

bool e2d::File::Exists() const
{
	return ::_waccess((const wchar_t*)file_path_, 0) == 0;
}

bool e2d::File::IsFolder() const
{
	return (attributes_ & FILE_ATTRIBUTE_DIRECTORY) != 0;
}

const e2d::String& e2d::File::GetPath() const
{
	return file_path_;
}

e2d::String e2d::File::GetExtension() const
{
	String file_ext;
	// �ҵ��ļ����е����һ�� '.' ��λ��
	size_t pos = file_path_.operator std::wstring().find_last_of(L'.');
	// �ж� pos �Ƿ�����Чλ��
	if (pos != std::wstring::npos)
	{
		// ��ȡ��չ��
		file_ext = file_path_.Subtract(static_cast<int>(pos));
		// ת��ΪСд��ĸ
		file_ext = file_ext.ToLower();
	}
	return std::move(file_ext);
}

bool e2d::File::Delete()
{
	if (::DeleteFile((LPCWSTR)file_path_))
		return true;
	return false;
}

e2d::File e2d::File::Extract(int resource_name, const String & resource_type, const String& dest_file_name)
{
	// �����ļ�
	HANDLE file = ::CreateFile(
		static_cast<LPCWSTR>(dest_file_name),
		GENERIC_WRITE,
		NULL,
		NULL,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_TEMPORARY,
		NULL
	);

	if (file == INVALID_HANDLE_VALUE)
		return std::move(File());

	// ������Դ�ļ��С�������Դ���ڴ桢�õ���Դ��С
	HRSRC res = ::FindResource(NULL, MAKEINTRESOURCE(resource_name), (LPCWSTR)resource_type);
	HGLOBAL res_data = ::LoadResource(NULL, res);
	DWORD res_size = ::SizeofResource(NULL, res);

	if (res && res_data && res_size)
	{
		// д���ļ�
		DWORD written_bytes = 0;
		::WriteFile(file, res_data, res_size, &written_bytes, NULL);
		::CloseHandle(file);
		return File(dest_file_name);
	}
	else
	{
		::CloseHandle(file);
		::DeleteFile(static_cast<LPCWSTR>(dest_file_name));
		return std::move(File());
	}
}

void e2d::File::AddSearchPath(const String & path)
{
	String tmp = path;
	tmp.Replace(L"/", L"\\");
	if (tmp.At(tmp.GetLength() - 1) != L'\\')
	{
		tmp << L"\\";
	}
	auto iter = std::find(search_paths_.cbegin(), search_paths_.cend(), tmp);
	if (iter == search_paths_.cend())
	{
		search_paths_.push_front(path);
	}
}

bool e2d::File::CreateFolder(const String & dir_path)
{
	if (dir_path.IsEmpty() || dir_path.GetLength() >= MAX_PATH)
		return false;

	wchar_t tmp_dir_path[MAX_PATH] = { 0 };
	int length = dir_path.GetLength();

	for (int i = 0; i < length; ++i)
	{
		tmp_dir_path[i] = dir_path.At(i);
		if (tmp_dir_path[i] == L'\\' || tmp_dir_path[i] == L'/' || i == (length - 1))
		{
			if (::_waccess(tmp_dir_path, 0) != 0)
			{
				if (::_wmkdir(tmp_dir_path) != 0)
				{
					return false;
				}
			}
		}
	}
	return true;
}

e2d::File e2d::File::ShowOpenDialog(const String & title, const String & filter)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);

	if (SUCCEEDED(hr))
	{
		IFileOpenDialog *file_open;

		hr = ::CoCreateInstance(CLSID_FileOpenDialog, NULL, CLSCTX_ALL,
			IID_IFileOpenDialog, reinterpret_cast<void**>(&file_open));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				file_open->SetTitle(LPCWSTR(title));
			}

			if (!filter.IsEmpty())
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", LPCWSTR(filter) }
				};
				file_open->SetFileTypes(1, spec);
			}
			else
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"�����ļ�", L"*.*" }
				};
				file_open->SetFileTypes(1, spec);
			}

			hr = file_open->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem *item;
				hr = file_open->GetResult(&item);
				if (SUCCEEDED(hr))
				{
					PWSTR str_file_path;
					hr = item->GetDisplayName(SIGDN_FILESYSPATH, &str_file_path);

					if (SUCCEEDED(hr))
					{
						file_path = str_file_path;
						::CoTaskMemFree(str_file_path);
					}
					item->Release();
				}
			}
			file_open->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}

e2d::File e2d::File::ShowSaveDialog(const String & title, const String& def_file, const String & def_ext)
{
	String file_path;
	HRESULT hr = ::CoInitializeEx(NULL, COINIT_APARTMENTTHREADED | COINIT_DISABLE_OLE1DDE);
	
	if (SUCCEEDED(hr))
	{
		IFileSaveDialog *file_save;

		hr = ::CoCreateInstance(CLSID_FileSaveDialog, NULL, CLSCTX_ALL,
			IID_IFileSaveDialog, reinterpret_cast<void**>(&file_save));

		if (SUCCEEDED(hr))
		{
			if (!title.IsEmpty())
			{
				file_save->SetTitle(LPCWSTR(title));
			}

			if (!def_file.IsEmpty())
			{
				file_save->SetFileName(LPCWSTR(def_file));
			}

			if (!def_ext.IsEmpty())
			{
				file_save->SetDefaultExtension(LPCWSTR(def_ext));

				String ext = L"*." + def_ext;
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"", LPCWSTR(ext) }
				};
				file_save->SetFileTypes(1, spec);
			}
			else
			{
				COMDLG_FILTERSPEC spec[] =
				{
					{ L"�����ļ�", L"*.*" }
				};
				file_save->SetFileTypes(1, spec);
			}

			hr = file_save->Show(nullptr);

			if (SUCCEEDED(hr))
			{
				IShellItem *item;
				hr = file_save->GetResult(&item);
				if (SUCCEEDED(hr))
				{
					PWSTR str_file_path;
					hr = item->GetDisplayName(SIGDN_FILESYSPATH, &str_file_path);

					if (SUCCEEDED(hr))
					{
						file_path = str_file_path;
						::CoTaskMemFree(str_file_path);
					}
					item->Release();
				}
			}
			file_save->Release();
		}
		::CoUninitialize();
	}
	return std::move(File(file_path));
}