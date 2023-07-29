#include "Resource.hpp"
#include <array>
#include <codecvt>
#include <exception>
#include <iostream>

const char16_t ResourceLocation::DOMAIN_SEPARATOR = u':';

ResourceLocation::ResourceLocation(const std::u16string key)
    : ResourceLocation(getDomain(key), getPath(key))
{
}

ResourceLocation::ResourceLocation(const std::u16string domain, const std::u16string path) noexcept
    : domain(domain)
    , path(path)
{
}

bool ResourceLocation::operator==(const ResourceLocation& b) const noexcept
{
    return this->domain == b.domain && this->path == b.path;
}

bool ResourceLocation::operator<(const ResourceLocation& b) const noexcept
{
    return this->domain < b.domain || (this->domain == b.domain && this->path < b.path);
}

std::u16string ResourceLocation::getDomain(const std::u16string key)
{
    auto i = key.find_first_of(DOMAIN_SEPARATOR);
    if (i < 0) throw std::invalid_argument("missing domain");
    auto domain = key.substr(0, i);
    return domain;
}

std::u16string ResourceLocation::getPath(const std::u16string key)
{
    auto i = key.find_first_of(DOMAIN_SEPARATOR);
    if (i < 0) throw std::invalid_argument("missing domain");
    auto path = key.substr(i + 1);
    return path;
}

const std::u16string ResourceManager::DEFAULT_ASSETS_DIR = u"assets";
const std::u16string ResourceManager::DEFAULT_DOMAIN     = u"default";
const std::u16string ResourceManager::EMBED_DOMAIN       = u"embed";

extern "C" const uint8_t _embed_indexAssets[];
extern "C" const uint8_t _embed_blockAssets[];

ResourceManager::ResourceManager()
{
    auto embed_res = std::make_unique<EmbedResource>(_embed_indexAssets, _embed_blockAssets);
    providers.emplace(EMBED_DOMAIN, std::move(embed_res));

    auto default_res = std::make_unique<FileResource>(DEFAULT_ASSETS_DIR);
    providers.emplace(DEFAULT_DOMAIN, std::move(default_res));
}

ResourceStream ResourceManager::get(const ResourceLocation& loc)
{
    auto it = providers.find(loc.domain);
    if (it != providers.end())
    {
        return it->second->get(loc);
    }

    return {};
}

FileResource::FileResource(const std::u16string root)
    : root(root)
{
}

ResourceStream FileResource::get(const ResourceLocation& loc)
{
    auto stream = std::make_unique<std::ifstream>(root / loc.path, std::ios_base::binary);
    if (stream->fail()) return {};
    return stream;
}

EmbedResource::membuf::membuf(const uint8_t* base, size_t size)
{
    char* p(reinterpret_cast<char*>(const_cast<uint8_t*>(base)));
    this->setg(p, p, p + size);
}

EmbedResource::membuf::pos_type
EmbedResource::membuf::seekoff(off_type _Off, std::ios_base::seekdir _Way, std::ios_base::openmode _Mode)
{
    if (_Mode & std::ios_base::in)
    {
        char* _Next;
        switch (_Way)
        {
        case std::ios_base::beg: _Next = eback() + _Off; break;
        case std::ios_base::cur: _Next = gptr() + _Off; break;
        case std::ios_base::end: _Next = egptr() + _Off; break;
        default: return pos_type(off_type(-1));
        }

        if (_Next >= eback() && _Next <= egptr())
        {
            setg(eback(), _Next, egptr());
            return static_cast<pos_type>(_Next - eback());
        }
    }
    return pos_type(off_type(-1));
}

EmbedResource::membuf::pos_type EmbedResource::membuf::seekpos(pos_type _Pos, std::ios_base::openmode _Mode)
{
    return seekoff(_Pos, std::ios_base::beg, _Mode);
}

EmbedResource::imstream::imstream(const uint8_t* base, size_t size)
    : membuf(base, size)
    , std::istream(static_cast<std::streambuf*>(this))
{
}

EmbedResource::EmbedResource(const Index index, const uint8_t* block)
    : index(index)
    , block(block)
{
}

EmbedResource::EmbedResource(const uint8_t* index, const uint8_t* block)
    : EmbedResource(_make_index(index), block)
{
}

ResourceStream EmbedResource::get(const ResourceLocation& loc)
{
    auto it = index.find(loc.path);
    if (it != index.end())
    {
        auto offset = it->second.first;
        auto size   = it->second.second;

        auto stream = std::make_unique<imstream>(&block[offset], size);
        return stream;
    }
    return {};
}

EmbedResource::Index EmbedResource::_make_index(const uint8_t* index)
{
    EmbedResource::Index _index;

    size_t offset = 0;
    auto   count  = _get<size_t>(index, offset);
    for (size_t i = 0; i < count; i++)
    {
        auto e_path   = _get_path(index, offset);
        auto e_size   = _get<size_t>(index, offset);
        auto e_offset = _get<size_t>(index, offset);
        auto e_hash   = std::hash<std::u16string>{}(e_path);

        _index.emplace(e_path, std::pair(e_offset, e_size));
    }

    return _index;
}

std::u16string EmbedResource::_get_path(const uint8_t* index, size_t& offset)
{
    // String size
    auto str_s = _get<size_t>(index, offset);

    std::u16string str((std::u16string::value_type*)&index[offset], str_s);
    offset += str_s * sizeof(std::u16string::value_type);

    return str;
}