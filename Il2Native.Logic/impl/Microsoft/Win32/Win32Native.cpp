#include "CoreLib.h"
#if _MSC_VER
#include <windows.h>
#else
#include <unistd.h>
#include <limits.h>
#include <stdlib.h>
#endif

#define O_RDONLY 0x0000	/* open for reading only */
#define O_WRONLY 0x0001	/* open for writing only */
#define O_RDWR 0x0002	/* open for reading and writing */
#define O_CREAT 0x0100		/* create if nonexistant */
#define O_TRUNC 0x0200		/* truncate to zero length */
#define O_EXCL 0x0040		/* error if already exists */
#define F_SETFD 2		/* set file descriptor flags */
#define LOCK_SH 0x01		/* shared file lock */
#define LOCK_EX 0x02		/* exclusive file lock */
#define LOCK_NB 0x04		/* don't block when locking */
#define LOCK_UN 0x08		/* unlock file */
/* access function */
#define F_OK 0	/* test for existence of file */
#define O_DIRECT 00040000
#define O_BINARY 0x8000
#define S_IRUSR 0000400			/* R for owner */
#define S_IWUSR 0000200			/* W for owner */
#define S_IROTH 0000004			/* R for other */
#define S_IRGRP 0000040			/* R for group */
#if !_MSC_VER
#define GENERIC_READ 0x80000000
#define GENERIC_WRITE 0x40000000
#endif
#define FILE_ATTRIBUTE_HIDDEN 0x00000002
#define FILE_ATTRIBUTE_SYSTEM 0x00000004
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_ARCHIVE 0x00000020
#define FILE_ATTRIBUTE_DEVICE 0x00000040
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define FILE_FLAG_NO_BUFFERING 0x20000000

struct stat_data
{
	int32_t st_dev;     /* ID of device containing file */
	int32_t st_ino;     /* inode number */
	uint8_t st_mode;    /* protection */
	int8_t st_nlink;   /* number of hard links */
	int8_t st_uid;     /* user ID of owner */
	int8_t st_gid;     /* group ID of owner */
	int32_t st_rdev;    /* device ID (if special file) */
	int32_t st_size;    /* total size, in bytes */
	int32_t st_atime;   /* time of last access */
	int32_t st_mtime;   /* time of last modification */
	int32_t st_ctime;   /* time of last status change */
	int32_t reserved0;
	int32_t reserved1;
	int32_t reserved2;
	int32_t reserved3;
	int32_t reserved4;
	int32_t reserved5;
	int32_t reserved6;
};

// Method : Microsoft.Win32.Win32Native.SetEvent(Microsoft.Win32.SafeHandles.SafeWaitHandle)
bool CoreLib::Microsoft::Win32::Win32Native::SetEvent(CoreLib::Microsoft::Win32::SafeHandles::SafeWaitHandle* handle)
{
	throw 0xC000C000;
}

// Method : Microsoft.Win32.Win32Native.ResetEvent(Microsoft.Win32.SafeHandles.SafeWaitHandle)
bool CoreLib::Microsoft::Win32::Win32Native::ResetEvent(CoreLib::Microsoft::Win32::SafeHandles::SafeWaitHandle* handle)
{
	throw 0xC000C000;
}

// Method : Microsoft.Win32.Win32Native.CreateEvent(Microsoft.Win32.Win32Native.SECURITY_ATTRIBUTES, bool, bool, string)
CoreLib::Microsoft::Win32::SafeHandles::SafeWaitHandle* CoreLib::Microsoft::Win32::Win32Native::CreateEvent(CoreLib::Microsoft::Win32::Win32Native_SECURITY_ATTRIBUTES* lpSecurityAttributes, bool isManualReset, bool initialState, string* name)
{
	throw 0xC000C000;
}

// Method : Microsoft.Win32.Win32Native.GetFullPathName(char*, int, char*, System.IntPtr)
int32_t CoreLib::Microsoft::Win32::Win32Native::GetFullPathName(wchar_t* path, int32_t numBufferChars, wchar_t* buffer, CoreLib::System::IntPtr mustBeZero)
{
	if (static_cast<void*>(path) == (void*)nullptr)
	{
		throw __new<CoreLib::System::ArgumentNullException>(L"path"_s, L"path"_s);
	}

#if _MSC_VER
	return GetFullPathNameW(path, numBufferChars, buffer, nullptr);
#elif _WIN32 || _WIN64
	return std::wcslen(_wfullpath(buffer, path, numBufferChars));
#else
	auto path_length = std::wcslen(path);
	auto utf8Enc = CoreLib::System::Text::Encoding::get_UTF8();
	auto byteCount = utf8Enc->GetByteCount(path, path_length);
	auto relative_path_utf8 = reinterpret_cast<uint8_t*>(alloca(byteCount + 1));
	auto bytesReceived = utf8Enc->GetBytes(path, path_length, relative_path_utf8, byteCount);
	auto resolved_path_utf8 = reinterpret_cast<uint8_t*>(alloca(numBufferChars));
	auto result = realpath(relative_path_utf8, resolved_path_utf8);
	if (result != 0)
	{
		utf8Enc->GetChars(resolved_path_ascii, numBufferChars, buffer, numBufferChars);
		return static_cast<int32_t>(std::wcslen(buffer));
	}

	return result;
#endif
}

// Method : Microsoft.Win32.Win32Native.GetStdHandle(int)
CoreLib::System::IntPtr CoreLib::Microsoft::Win32::Win32Native::GetStdHandle(int32_t nStdHandle)
{
	return __init<CoreLib::System::IntPtr>(nStdHandle);
}

// Method : Microsoft.Win32.Win32Native.CreateFile(string, int, System.IO.FileShare, Microsoft.Win32.Win32Native.SECURITY_ATTRIBUTES, System.IO.FileMode, int, System.IntPtr)
CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle* CoreLib::Microsoft::Win32::Win32Native::CreateFile(string* lpFileName, int32_t dwDesiredAccess, CoreLib::System::IO::enum_FileShare dwShareMode, CoreLib::Microsoft::Win32::Win32Native_SECURITY_ATTRIBUTES* securityAttrs, CoreLib::System::IO::enum_FileMode dwCreationDisposition, int32_t dwFlagsAndAttributes, CoreLib::System::IntPtr hTemplateFile)
{
#if _MSC_VER
	auto hFile = CreateFileW(&lpFileName->m_firstChar,    // name of the write
		(int32_t)dwDesiredAccess,						 // open for writing
		(int32_t)dwShareMode,							 // do not share
		(LPSECURITY_ATTRIBUTES )securityAttrs,			 // default security
		(int32_t)dwCreationDisposition,					 // create new file only
		(int32_t)dwFlagsAndAttributes,					 // normal file
		(void*)hTemplateFile);							 // no attr. template

	if (hFile == (HANDLE)-1) 
	{ 
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(((CoreLib::System::IntPtr)CoreLib::Microsoft::Win32::Win32Native::INVALID_HANDLE_VALUE), false);
	}

	return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(__init<CoreLib::System::IntPtr>(hFile), false);
#else
	int32_t filed = -1;
	int32_t create_flags = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
	int32_t open_flags = 0;
	bool fFileExists = false;

	if (lpFileName == nullptr)
	{
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(((CoreLib::System::IntPtr)CoreLib::Microsoft::Win32::Win32Native::INVALID_HANDLE_VALUE), false);
	}

	auto path = &lpFileName->m_firstChar;
	auto path_length = std::wcslen(path);
	auto utf8Enc = CoreLib::System::Text::Encoding::get_UTF8();
	auto byteCount = utf8Enc->GetByteCount(path, path_length);
	auto path_urf8 = reinterpret_cast<uint8_t*>(alloca(byteCount + 1));
	auto bytesReceived = utf8Enc->GetBytes(path, path_length, path_urf8, byteCount);

	switch ((uint32_t)dwDesiredAccess)
	{
	case GENERIC_READ:
		open_flags |= O_RDONLY;
		break;
	case GENERIC_WRITE:
		open_flags |= O_WRONLY;
		break;
	case GENERIC_READ | GENERIC_WRITE:
		open_flags |= O_RDWR;
		break;
	default:
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(((CoreLib::System::IntPtr)CoreLib::Microsoft::Win32::Win32Native::INVALID_HANDLE_VALUE), false);
	}

	switch ((CoreLib::System::IO::enum_FileMode)dwCreationDisposition)
	{
	case CoreLib::System::IO::enum_FileMode::c_Create:
		// check whether the file exists
		if (access(path_urf8, F_OK) == 0)
		{
			fFileExists = true;
		}

		open_flags |= O_CREAT | O_TRUNC;
		break;
	case CoreLib::System::IO::enum_FileMode::c_CreateNew:
		open_flags |= O_CREAT | O_EXCL;
		break;
	case CoreLib::System::IO::enum_FileMode::c_Open:
		/* don't need to do anything here */
		break;
	case CoreLib::System::IO::enum_FileMode::c_OpenOrCreate:
		if (access(path_urf8, F_OK) == 0)
		{
			fFileExists = true;
		}

		open_flags |= O_CREAT;
		break;
	case CoreLib::System::IO::enum_FileMode::c_Truncate:
		open_flags |= O_TRUNC;
		break;
	default:
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(((CoreLib::System::IntPtr)CoreLib::Microsoft::Win32::Win32Native::INVALID_HANDLE_VALUE), false);
	}

	if ((dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING) > 0)
	{
		open_flags |= O_DIRECT;
	}

	open_flags |= O_BINARY;

	filed = open(path_urf8, open_flags, (open_flags & O_CREAT) > 0 ? create_flags : 0);
	if (filed < 0)
	{
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(__init<CoreLib::System::IntPtr>(0), false);
	}

#if flock
	auto lock_mode = (dwShareMode == 0 /* FILE_SHARE_NONE */) ? LOCK_EX : LOCK_SH;
	if (flock(filed, lock_mode | LOCK_NB) != 0)
	{
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(INVALID_HANDLE_VALUE, false);
	}
#endif

#if O_DIRECT
	if ((dwFlagsAndAttributes & FILE_FLAG_NO_BUFFERING) > 0)
	{
#if F_NOCACHE
		if (-1 == fcntl(filed, F_NOCACHE, 1))
		{
			return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(INVALID_HANDLE_VALUE, false);
		}
#else
		////#error Insufficient support for uncached I/O on this platform
#endif
	}
#endif

#if fcntl
	/* make file descriptor close-on-exec; inheritable handles will get
	"uncloseonexeced" in CreateProcess if they are actually being inherited*/
	auto ret = fcntl(filed, F_SETFD, 1);
	if (-1 == ret)
	{
		return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(INVALID_HANDLE_VALUE, false);
	}
#endif

	return __new<CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle>(__init<CoreLib::System::IntPtr>(filed), false);
#endif
}

// Method : Microsoft.Win32.Win32Native.CloseHandle(System.IntPtr)
bool CoreLib::Microsoft::Win32::Win32Native::CloseHandle(CoreLib::System::IntPtr handle)
{
#if _MSC_VER
	return ::CloseHandle((HANDLE)handle.ToInt32());
#else
	_close(handle.ToInt32());
	return true;
#endif
}

// Method : Microsoft.Win32.Win32Native.GetFileType(Microsoft.Win32.SafeHandles.SafeFileHandle)
int32_t CoreLib::Microsoft::Win32::Win32Native::GetFileType(CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle* handle)
{
	auto stdId = handle->DangerousGetHandle()->ToInt32();
	if (stdId == -11 || stdId == -12)
	{
		return FILE_TYPE_CHAR;
	}

	return FILE_TYPE_DISK;
}

// Method : Microsoft.Win32.Win32Native.GetFileSize(Microsoft.Win32.SafeHandles.SafeFileHandle, out int)
int32_t CoreLib::Microsoft::Win32::Win32Native::GetFileSize_Out(CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle* hFile, int32_t& highSize)
{
#if _MSC_VER
	return GetFileSize((HANDLE)hFile->DangerousGetHandle()->ToInt32(), (LPDWORD) highSize);
#else
	highSize = 0;
	auto data = new stat_data();
	auto returnCode = _fstat(hFile->DangerousGetHandle()->ToInt32(), &data.st_dev);
	if (returnCode != 0)
	{
		return 0;
	}

	return data.st_size;
#endif
}

// Method : Microsoft.Win32.Win32Native.ReadFile(Microsoft.Win32.SafeHandles.SafeFileHandle, byte*, int, out int, System.IntPtr)
int32_t CoreLib::Microsoft::Win32::Win32Native::ReadFile_Out(CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle* handle, uint8_t* bytes, int32_t numBytesToRead, int32_t& numBytesRead, CoreLib::System::IntPtr mustBeZero)
{
	auto fd = handle->DangerousGetHandle()->ToInt32();
#if _MSC_VER
	return (int32_t) ::ReadFile((HANDLE)fd, (LPVOID) bytes, numBytesToRead, (LPDWORD)&numBytesRead, nullptr);
#else
	auto r = _read(fd, bytes, numBytesToRead);
	if (r == -1)
	{
		numBytesRead = 0;
		return 0;
	}

	numBytesRead = r;
	return 1;
#endif
}

// Method : Microsoft.Win32.Win32Native.WriteFile(Microsoft.Win32.SafeHandles.SafeFileHandle, byte*, int, out int, System.IntPtr)
int32_t CoreLib::Microsoft::Win32::Win32Native::WriteFile_Out(CoreLib::Microsoft::Win32::SafeHandles::SafeFileHandle* handle, uint8_t* bytes, int32_t numBytesToWrite, int32_t& numBytesWritten, CoreLib::System::IntPtr mustBeZero)
{
	auto fd = handle->DangerousGetHandle()->ToInt32();
#if _MSC_VER
	return (int32_t) ::WriteFile((HANDLE)fd, (LPCVOID) bytes, numBytesToWrite, (LPDWORD)&numBytesWritten, nullptr);
#else
	if (fd == -11)
	{
		numBytesWritten = _write(STDOUT_FILENO, bytes, numBytesToWrite);
		return numBytesWritten < numBytesToWrite ? 0 : 1;
	}
	else if (fd == -12)
	{
		numBytesWritten = _write(STDERR_FILENO, bytes, numBytesToWrite);
		return numBytesWritten < numBytesToWrite ? 0 : 1;
	}
	else
	{
		auto r = _write(fd, bytes, numBytesToWrite);
		if (r != -1)
		{
			numBytesWritten = r;
			return 1;
		}
	}

	numBytesWritten = 0;
	return 0;
#endif
}

// Method : Microsoft.Win32.Win32Native.GetFileAttributesEx(string, int, ref Microsoft.Win32.Win32Native.WIN32_FILE_ATTRIBUTE_DATA)
bool CoreLib::Microsoft::Win32::Win32Native::GetFileAttributesEx_Ref(string* name, int32_t fileInfoLevel, CoreLib::Microsoft::Win32::Win32Native_WIN32_FILE_ATTRIBUTE_DATA& lpFileInformation)
{
	throw 0xC000C000;
}
