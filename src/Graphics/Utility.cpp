#include "pch_dx_11.h"
#include "Utility.h"

using namespace std;

string Utility::ToString(wstring value)
{
    string temp = "";
    temp.assign(value.begin(), value.end());

    return temp;
}

wstring Utility::ToWstring(string value)
{
    wstring temp = L"";
    temp.assign(value.begin(), value.end());

    return temp;
}

std::wstring Utility::ToWstring(const char* value)//���� �ڵ�
{
    const int _stringLength = static_cast<int>(std::strlen(value));
    const int _bufferLength = _stringLength + 1;
    wchar_t* _buffer = new wchar_t[_bufferLength];

    // MultiByteToWideChar �Լ��� ���ڿ��� ���� �ڵ����� null ���� ('\0')�� �־����� �ʽ��ϴ�.
    // ���� ���ڿ��� ��ȯ�� ��ģ �� �� �ڿ��� �������� null���ڸ� �־��־�� �մϴ�.
    int _end = MultiByteToWideChar(CP_UTF8, MB_PRECOMPOSED, value, _stringLength,
        _buffer, _bufferLength);
    _buffer[_end] = '\0';

    auto _retVal = std::wstring{ _buffer };

    delete[] _buffer;
    return _retVal;
}

void Utility::SplitString(std::vector<std::string>& dest, string origin, string tok)
{
    vector<string> result;

    size_t cutAt = 0;

    while ((cutAt = origin.find_first_of(tok)) != origin.npos)
    {
        if (cutAt > 0)
            result.emplace_back(origin.substr(0, cutAt));

        origin = origin.substr(cutAt + 1);
    }
    dest = result;
}

void Utility::Replace(string* str, string comp, string rep)
{
    string temp = *str;

    size_t at = 0;
    while ((at = temp.find(comp, at)) != string::npos)
    {
        temp.replace(at, comp.length(), rep);
        at += rep.length();
    }
    *str = temp;
}

wstring Utility::GetExtension(wstring path)
{
    size_t index = path.find_last_of('.');

    return path.substr(index + 1, path.length());
}

string Utility::GetDirectoryName(string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('/');

    return path.substr(0, index + 1);
}

string Utility::GetFileName(string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('/');

    return path.substr(index + 1, path.length());
}

string Utility::GetFileNameWithoutExtension(string path)
{
    string fileName = GetFileName(path);

    size_t index = fileName.find_last_of('.');

    return fileName.substr(0, index);
}

std::string Utility::GetPathNameWithoutExtension(std::string path)
{
    Replace(&path, "\\", "/");

    size_t index = path.find_last_of('.');

    return path.substr(0, index);
}

void Utility::ToUpper(string& value)
{
    for (int i = 0; i < value.length(); i++)
    {
        value[i] = toupper(value[i]);
    }
}

bool Utility::ExistFile(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());

    return fileValue < 0xffffffff;
}

bool Utility::ExistFileW(wstring path)
{
    return ExistFile(ToString(path));
}

bool Utility::ExistDirectory(string path)
{
    DWORD fileValue = GetFileAttributesA(path.c_str());

    BOOL temp = (fileValue != INVALID_FILE_ATTRIBUTES &&
        (fileValue & FILE_ATTRIBUTE_DIRECTORY));

    return temp == TRUE;
}

void Utility::CreateFolders(string path)
{
    vector<string> folders;
    SplitString(folders, path, "/");

    string temp = "";
    for (string folder : folders)
    {
        temp += folder + "/";

        if (!ExistDirectory(temp))
            CreateDirectoryA(temp.c_str(), 0);
    }
}

std::vector<char> Utility::GetBinary(std::string path)
{
    std::ifstream fin(path, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    assert(size >= 0);//file not Found

    fin.seekg(0, std::ios_base::beg);
    std::vector<char> binary(size);

    fin.read(binary.data(), size);
    fin.close();

    return binary;
}

std::vector<char> Utility::GetBinaryW(std::wstring path)
{
    std::ifstream fin(path, std::ios::binary);

    fin.seekg(0, std::ios_base::end);
    int size = (int)fin.tellg();
    fin.seekg(0, std::ios_base::beg);
    std::vector<char> binary(size);

    fin.read(binary.data(), size);
    fin.close();

    return binary;
}
